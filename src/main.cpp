#include <Arduino.h>

#include "Logger.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  LOG_INFO("Current firmware version: %s", VERSION);
  LOG_INFO("Built on: %s", __DATE__);
}

void loop() {
  // put your main code here, to run repeatedly:
}