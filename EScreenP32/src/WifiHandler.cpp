#include <WiFi.h>
#include <ArduinoOTA.h>
#include "main.h"

TaskHandle_t HandlerWifi;
extern Tipo_Parametros Parametros;

//private prototypes
static void WifiMain(void *parameter);
static void setupOTA(const char* ssid, const char* password);


// Tasks
static void WifiMain(void *parameter)
{
  setupOTA(WIFI_SSID, WIFI_PWD);
  Serial.println("OTA Readyyy");

  while (1)
  {
    ArduinoOTA.handle();
    delay(2000);
    
    /*Serial.print("Stack WIfi high water mark: ");
    Serial.println(uxTaskGetStackHighWaterMark(HandlerWifi));*/
  }
  vTaskDelete(HandlerWifi);
}


// Functions
void InitWifi(void)
{
  xTaskCreate(WifiMain, "WifiMain", 8192, NULL, 1, &HandlerWifi);
}

static void setupOTA(const char *ssid, const char *password)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Retrying in 5 seconds...");
    Parametros.WifiStatus = false;
    delay(5000);
    WiFi.begin(ssid, password);
  }
  Parametros.WifiStatus = true;
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Setting up OTA...");
 
  
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
      } else {  // U_SPIFFS
        type = "filesystem";
      }

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
      } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
      } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
      } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
      } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
      }
    });

  ArduinoOTA.begin();
}
