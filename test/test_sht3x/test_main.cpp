/**
 * @file test_sht3x/test_main.cpp
 * @author Yasir Hassan <yasir.hassan1993@hotmail.com>
 * @brief Unit tests accompanying SHT3x class
 * @version 0.1
 * @date 2022-07-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <Arduino.h>
#include <Wire.h>
#include <unity.h>

#include "SHT3x.h"

SHT3x sht;

void setUp(void) {
  // set stuff up here
  Wire.begin();
}

void tearDown(void) {
  // clean stuff up here
}

void test_shtBeginSensorWrongI2CAddress(void) {
  TEST_ASSERT_EQUAL(false, sht.begin((uint8_t)0x03));
}

void test_shtBeginSensorWrongRepMode(void) {
  TEST_ASSERT_EQUAL(false, sht.begin((uint8_t)SHT_ADDR_t::SHT_ADDR_DEFAULT, (uint16_t)0xAABB));
}

void test_shtBeginSensor(void) {
  TEST_ASSERT_EQUAL(true, sht.begin());
}

void test_shtRawDataToBeZero(void) {
  TEST_ASSERT_EQUAL((uint16_t)0, sht.getRawHumidity());
  TEST_ASSERT_EQUAL((uint16_t)0, sht.getRawTemperature());
}

void test_shtReadStatus(void) {
  TEST_ASSERT_EQUAL((uint16_t)0x8010, sht.readStatus());
}

void test_shtGetErrorCode(void) {
  TEST_ASSERT_EQUAL((uint8_t)SHT_ERROR_CODES_t::SHT_ERR_OK, sht.getErrorCode());
}

void test_readSensorData(void) {
  TEST_ASSERT_EQUAL(true, sht.readSensorData());
}

void test_shtRawDataToBeNonZero(void) {
  TEST_ASSERT_NOT_EQUAL((uint16_t)0, sht.getHumidity());
  TEST_ASSERT_NOT_EQUAL((uint16_t)0, sht.getRawTemperature());
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_shtBeginSensorWrongI2CAddress);
  RUN_TEST(test_shtBeginSensorWrongRepMode);
  RUN_TEST(test_shtBeginSensor);
  RUN_TEST(test_shtRawDataToBeZero);
  RUN_TEST(test_shtReadStatus);
  RUN_TEST(test_shtGetErrorCode);
  RUN_TEST(test_readSensorData);
  RUN_TEST(test_shtRawDataToBeNonZero);
  UNITY_END();
}

void loop() {}