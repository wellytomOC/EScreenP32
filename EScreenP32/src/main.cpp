#include <Arduino.h>
#include "main.h"


// put function declarations here:
int myFunction(int, int);

void setup() {
  Serial.begin(115200);

  InitWifi();  
}

void loop() {
  Serial.println("Teste");
  delay(1000);
}

