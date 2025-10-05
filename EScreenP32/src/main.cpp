#include <Arduino.h>
#include "main.h"


Tipo_Parametros Parametros;

void setup() {
  Serial.begin(115200);

  delay(3000); // Wait for serial to initialize
  InitWifi(); 
  InitDisplay();
  Serial.println("Setup Complete");

}

void loop() {
  Serial.print("Main Loop. Free heap: ");
  Serial.println(heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
  delay(1000);
}
