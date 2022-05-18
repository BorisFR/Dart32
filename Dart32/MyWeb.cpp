#include "MyWeb.hpp"

MyWeb myWeb = MyWeb();

void onWsEventStatic(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    MyWeb *thisPtr = myWeb.thisPtr;
    MY_DEBUG(".");
    thisPtr->onWsEvent(server, client, type, &arg, data, len);
}

MyWeb::MyWeb()
{
    thisPtr = this;
    //server = AsyncWebServer(80);
    //browserWebSocket = AsyncWebSocket("/dart32");
}

// ************************************************************************************

void MyWeb::begin(void (*receivedCommand)(String command))
{
    _receivedCommand = receivedCommand;
}

// ************************************************************************************

// to be call from outside
void MyWeb::sendToAll(String text)
{
    browserWebSocket.textAll(text);
}

// ************************************************************************************

// we received some info, technical thing for the protocol, no need to touch
void MyWeb::onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    MY_DEBUG("!");
    if (type == WS_EVT_CONNECT)
    {
        MY_DEBUG_LN("ws[" + String(server->url()) + "][" + String(client->id()) + "] connect");
        //Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
        //client->printf("Hello Client %u :)", client->id());
        client->ping();
    }
    else if (type == WS_EVT_DISCONNECT)
    {
        MY_DEBUG_LN("ws[" + String(server->url()) + "][" + String(client->id()) + "] disconnect");
        //Serial.printf("ws[%s][%u] disconnect\n", server->url(), client->id());
    }
    else if (type == WS_EVT_ERROR)
    {
        MY_DEBUG_LN("ws[" + String(server->url()) + "][" + String(client->id()) + "] error");
        //Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t *)arg), (char *)data);
    }
    else if (type == WS_EVT_PONG)
    {
        MY_DEBUG_LN("ws[" + String(server->url()) + "][" + String(client->id()) + "] pong");
        //Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len) ? (char *)data : "");
    }
    else if (type == WS_EVT_DATA)
    {
        MY_DEBUG_LN("ws[" + String(server->url()) + "][" + String(client->id()) + "] data");
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        String msg = "";
        if (info->final && info->index == 0 && info->len == len)
        {
            //the whole message is in a single frame and we got all of it's data
            //Serial.printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT) ? "text" : "binary", info->len);

            if (info->opcode == WS_TEXT)
            {
                for (size_t i = 0; i < info->len; i++)
                {
                    msg += (char)data[i];
                }
            }
            else
            {
                //char buff[3];
                for (size_t i = 0; i < info->len; i++)
                {
                    //sprintf(buff, "%02x ", (uint8_t)data[i]);
                    //msg += buff;
                    msg += (char)((uint8_t)data[i]);
                }
            }
            _receivedCommand("AA" + msg);
            //Serial.printf("%s\n", msg.c_str());

            //if (info->opcode == WS_TEXT)
            //  client->text("I got your text message");
            //else
            //  client->binary("I got your binary message");
        }
        else
        {
            //message is comprised of multiple frames or the frame is split into multiple packets
            /*if (info->index == 0)
            {
                if (info->num == 0)
                    Serial.printf("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
                Serial.printf("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
            }

            Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT) ? "text" : "binary", info->index, info->index + len);
            */
            if (info->opcode == WS_TEXT)
            {
                for (size_t i = 0; i < len; i++)
                {
                    msg += (char)data[i];
                }
                _receivedCommand("BB" + msg);
            }
            else
            {
                //char buff[3];
                for (size_t i = 0; i < len; i++)
                {
                    //Serial.print((uint8_t)data[i], DEC);
                    //Serial.print(" ");
                    msg += (char)((uint8_t)data[i]);
                    //sprintf(buff, "%02x ", (uint8_t)data[i]);
                    //msg += buff;
                }
                _receivedCommand(msg);
            }
            //Serial.printf("%s\n", msg.c_str());

            if ((info->index + len) == info->len)
            {
                //Serial.printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
                if (info->final)
                {
                    //Serial.printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
                    //if (info->message_opcode == WS_TEXT)
                    //  client->text("I got your text message");
                    //else
                    //  client->binary("I got your binary message");
                    _receivedCommand("DD" + String(msg));
                }
            }
        }
    }
}

// ************************************************************************************

// Start the webserver, need the fileSystem where stuff are, normaly : SPIFF
void MyWeb::setupWeb(fs::FS &fileSystem)
{
    // the web socket
    browserWebSocket.onEvent(onWsEventStatic);
    server.addHandler(&browserWebSocket);
    // for debug, give the free heap space
    server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(ESP.getFreeHeap()));
    });
    // delivery of statics files
    server.serveStatic("/", fileSystem, "/")
        .setDefaultFile("index.html")
        .setLastModified("Mon, 26 Jun 2020 16:00:00 GMT")
        .setCacheControl("max-age=600");
    // when something from a request is not found
    server.onNotFound([](AsyncWebServerRequest *request) {
        MY_DEBUG("NOT_FOUND: ");
        if (request->method() == HTTP_GET)
            MY_DEBUG("GET");
        else if (request->method() == HTTP_POST)
            MY_DEBUG("POST");
        else if (request->method() == HTTP_DELETE)
            MY_DEBUG("DELETE");
        else if (request->method() == HTTP_PUT)
            MY_DEBUG("PUT");
        else if (request->method() == HTTP_PATCH)
            MY_DEBUG("PATCH");
        else if (request->method() == HTTP_HEAD)
            MY_DEBUG("HEAD");
        else if (request->method() == HTTP_OPTIONS)
        {
            MY_DEBUG("OPTIONS");
            request->send(200);
            return;
        }
        else
            MY_DEBUG("UNKNOWN");
        MY_DEBUG_LN(" http://" + String(request->host()) + String(request->url()));
        //Serial.printf(" http://%s%s\n", request->host().c_str(), request->url().c_str());
        request->send(404);
    });

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    server.begin();
    MY_DEBUG_LN("HTTP server started");
}
