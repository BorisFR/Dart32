#pragma once
#ifndef MYWIFI_H
#define MYWIFI_H

#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>

#include "configuration.h"
#include "myTools.h"

class MyWifi
{
public:
    MyWifi();
    void setupWifi();
    String getIP();
    static void OnEvent(WiFiEvent_t id, WiFiEventInfo_t event);

private:
};

extern String theIP;

#endif