#include "myWifi.hpp"

String theIP; // = "0.0.0.0";

MyWifi::MyWifi() {}

/*

esp_err_t WiFiGenericClass::_eventCallback(void *arg, system_event_t *event)


if(event->event_id == SYSTEM_EVENT_ETH_GOT_IP) {
#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_DEBUG
        uint8_t * ip = (uint8_t *)&(event->event_info.got_ip.ip_info.ip.addr);
        uint8_t * mask = (uint8_t *)&(event->event_info.got_ip.ip_info.netmask.addr);
        uint8_t * gw = (uint8_t *)&(event->event_info.got_ip.ip_info.gw.addr);
        log_d("ETH IP: %u.%u.%u.%u, MASK: %u.%u.%u.%u, GW: %u.%u.%u.%u",
            ip[0], ip[1], ip[2], ip[3],
            mask[0], mask[1], mask[2], mask[3],
            gw[0], gw[1], gw[2], gw[3]);


// typedef enum {
// SYSTEM_EVENT_WIFI_READY = 0,           //< ESP32 WiFi ready
// SYSTEM_EVENT_SCAN_DONE,                  //< ESP32 finish scanning AP
//     SYSTEM_EVENT_STA_START,              //< ESP32 station start
//     SYSTEM_EVENT_STA_STOP,               //< ESP32 station stop
//     SYSTEM_EVENT_STA_CONNECTED,          //< ESP32 station connected to AP
//     SYSTEM_EVENT_STA_DISCONNECTED,       //< ESP32 station disconnected from AP
//     SYSTEM_EVENT_STA_AUTHMODE_CHANGE,    //< the auth mode of AP connected by ESP32 station changed
//     SYSTEM_EVENT_STA_GOT_IP,             //< ESP32 station got IP from connected AP
//     SYSTEM_EVENT_STA_LOST_IP,            //< ESP32 station lost IP and the IP is reset to 0
//     SYSTEM_EVENT_STA_WPS_ER_SUCCESS,     //< ESP32 station wps succeeds in enrollee mode
//     SYSTEM_EVENT_STA_WPS_ER_FAILED,      //< ESP32 station wps fails in enrollee mode
//     SYSTEM_EVENT_STA_WPS_ER_TIMEOUT,     //< ESP32 station wps timeout in enrollee mode
//     SYSTEM_EVENT_STA_WPS_ER_PIN,         //< ESP32 station wps pin code in enrollee mode
//     SYSTEM_EVENT_STA_WPS_ER_PBC_OVERLAP, //!< ESP32 station wps overlap in enrollee mode
//     SYSTEM_EVENT_AP_START,               //< ESP32 soft-AP start
//     SYSTEM_EVENT_AP_STOP,                //< ESP32 soft-AP stop
//     SYSTEM_EVENT_AP_STACONNECTED,        //< a station connected to ESP32 soft-AP
//     SYSTEM_EVENT_AP_STADISCONNECTED,     //< a station disconnected from ESP32 soft-AP
//     SYSTEM_EVENT_AP_STAIPASSIGNED,       //< ESP32 soft-AP assign an IP to a connected station
//     SYSTEM_EVENT_AP_PROBEREQRECVED,      //< Receive probe request packet in soft-AP interface
//     SYSTEM_EVENT_GOT_IP6,                //< ESP32 station or ap or ethernet interface v6IP addr is preferred
//     SYSTEM_EVENT_ETH_START,              //< ESP32 ethernet start
//     SYSTEM_EVENT_ETH_STOP,               //< ESP32 ethernet stop
//     SYSTEM_EVENT_ETH_CONNECTED,          //< ESP32 ethernet phy link up
//     SYSTEM_EVENT_ETH_DISCONNECTED,       //< ESP32 ethernet phy link down
//     SYSTEM_EVENT_ETH_GOT_IP,             //< ESP32 ethernet got IP from connected AP
//     SYSTEM_EVENT_MAX
// }
// system_event_id_t;

typedef union {
    system_event_sta_connected_t               connected;          //< ESP32 station connected to AP
system_event_sta_disconnected_t disconnected;          //< ESP32 station disconnected to AP
system_event_sta_scan_done_t scan_done;                //< ESP32 station scan (APs) done
system_event_sta_authmode_change_t auth_change;        //< the auth mode of AP ESP32 station connected to changed
system_event_sta_got_ip_t got_ip;                      //< ESP32 station got IP, first time got IP or when IP is changed
system_event_sta_wps_er_pin_t sta_er_pin;              //< ESP32 station WPS enrollee mode PIN code received
system_event_sta_wps_fail_reason_t sta_er_fail_reason; //< ESP32 station WPS enrollee mode failed reason code received
system_event_ap_staconnected_t sta_connected;          //< a station connected to ESP32 soft-AP
system_event_ap_stadisconnected_t sta_disconnected;    //< a station disconnected to ESP32 soft-AP
system_event_ap_probe_req_rx_t ap_probereqrecved;      //< ESP32 soft-AP receive probe request packet
system_event_got_ip6_t got_ip6;                        //< ESP32 station　or ap or ethernet ipv6 addr state change to preferred
}
system_event_info_t;

*/

// ************************************************************************************

void MyWifi::OnEvent(WiFiEvent_t id, WiFiEventInfo_t event)
{
    log_d("EVENT: %s", String(id));
    uint8_t *ip = (uint8_t *)&(event.got_ip.ip_info.ip.addr);
    uint8_t *mask = (uint8_t *)&(event.got_ip.ip_info.netmask.addr);
    uint8_t *gw = (uint8_t *)&(event.got_ip.ip_info.gw.addr);
    log_d("IP: %u.%u.%u.%u, MASK: %u.%u.%u.%u, GW: %u.%u.%u.%u",
          ip[0], ip[1], ip[2], ip[3],
          mask[0], mask[1], mask[2], mask[3],
          gw[0], gw[1], gw[2], gw[3]);
    theIP = WiFi.localIP().toString();
}

void MyWifi::setupWifi()
{
    log_d("Connecting to %s", String(THE_SSID));
    WiFi.onEvent(OnEvent, SYSTEM_EVENT_STA_GOT_IP);
    WiFi.onEvent(OnEvent, SYSTEM_EVENT_STA_LOST_IP);
    if (String(WiFi.SSID()) != String(THE_SSID))
    {
        WiFi.mode(WIFI_STA);
        WiFi.begin(THE_SSID, THE_PASSWORD);
    }
    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        log_d("STA: Failed!");
        WiFi.disconnect(false);
        delay(1000);
        WiFi.begin(THE_SSID, THE_PASSWORD);
    }

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(300);
        MY_DEBUG(".");
    }
    MY_DEBUG_LN("");
    MY_DEBUG("Connected! IP address: ");
    MY_DEBUG_LN(WiFi.localIP());
    theIP = WiFi.localIP().toString();

    MDNS.addService("http", "tcp", 80);
    MDNS.begin(THE_HOST);
    MY_DEBUG("Open http://");
    MY_DEBUG(THE_HOST);
    MY_DEBUG_LN(".local");
}

String MyWifi::getIP()
{
    return theIP;
}