#include <Arduino.h>
#include "main.h"




// put function declarations here:

void setup() {
  Serial.begin(115200);

  pinMode(LCD_ON, OUTPUT);
  digitalWrite(LCD_ON, HIGH);

  InitWifi(); 
  InitDisplay();
}

void loop() {
  Serial.println("Main Loop");
  delay(1000);
}
