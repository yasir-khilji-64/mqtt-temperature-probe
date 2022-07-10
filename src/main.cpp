#include <Arduino.h>
#include <Wire.h>

#include "Logger.h"
#include "SHT3x.h"

SHT3x sht;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  LOG_INFO("Current firmware version: %s", VERSION);
  LOG_INFO("Built on: %s", __DATE__);

  Wire.begin();
  if (!sht.begin(SHT_ADDR_t::SHT_ADDR_DEFAULT)) {
    LOG_ERROR("SHT3x init error");
    LOG_ERROR("Check SHT3x Address.");
    while (1) {
      yield();
    }
  } else {
    LOG_INFO("SHT3x initialized");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}