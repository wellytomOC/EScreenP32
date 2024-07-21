#pragma once
#include <WiFi.h>
#include <ArduinoOTA.h>


//wifi setup
#define WIFI_SSID "IGOR"
#define WIFI_PWD "991280556n"



//prototipos
void InitWifi(void);
void WifiMain(void *parameter);
void setupOTA(const char* ssid, const char* password);