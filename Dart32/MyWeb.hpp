#pragma once
#ifndef MYWEB_H
#define MYWEB_H

#include "FreeRTOS.h"
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"

#include "configuration.h"
#include "myTools.h"

class MyWeb
{
public:
    MyWeb();
    MyWeb *thisPtr;
    void begin(void (*receivedCommand)(String command));
    void sendToAll(String text);
    void setupWeb(fs::FS &fileSystem);
    void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
    AsyncWebSocket browserWebSocket = AsyncWebSocket("/dart32");

private:
    void (*_receivedCommand)(String command);
    AsyncWebServer server = AsyncWebServer(80);
};

extern MyWeb myWeb;

#endif