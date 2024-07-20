#include "wifi.h"

WiFiMulti wifiMulti;
TaskHandle_t *HandlerWifi;

void WifiMain(void *parameter)
{
    wifiMulti.addAP(WIFI_SSID, WIFI_PWD);
    while (wifiMulti.run() != WL_CONNECTED) delay(100);
    Serial.println("Connectado no Wifi!!");

    while (1)
    {
        delay(1000);
    }
    vTaskDelete(HandlerWifi);
}

// funcoes
void InitWifi(void)
{
    xTaskCreate(WifiMain, "WifiMain", 10000, NULL, 1, HandlerWifi);
}

