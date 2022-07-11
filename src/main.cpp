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
  if (!sht.begin((uint8_t)(SHT_ADDR_t::SHT_ADDR_DEFAULT), (uint16_t)(SHT_REP_MODES_t::SHT_REP_MODE_HI))) {
    LOG_ERROR("SHT3x init error");
    LOG_ERROR("Check serial console for error descriptions");
    while (1) {
      yield();
    }
  } else {
    LOG_INFO("SHT3x initialized");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!sht.readSensorData()) {
    LOG_ERROR("Error! Fatal error while reading sensor data");
  } else {
    LOG_DEBUG("Temperature: %.2f℃", sht.getTemperature());
    LOG_DEBUG("Humidity: %.2f%%", sht.getHumidity());
  }
  delay(500);
}