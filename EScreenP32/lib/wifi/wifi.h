#pragma once
#include <WiFi.h>
#include <WiFiMulti.h>


//wifi setup
#define WIFI_SSID "IGOR"
#define WIFI_PWD "991280556n"



//prototipos
void InitWifi(void);
void WifiMain(void *parameter);