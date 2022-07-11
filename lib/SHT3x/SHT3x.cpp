/**
 * @file SHT3x.cpp
 * @author Yasir Hassan <yasir.hassan1993@hotmail.com>
 * @brief Function implementation for SHT3x class.
 * @version 0.1
 * @date 2022-07-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <Wire.h>

#include "SHT3x.h"
#include "Logger.h"

SHT3x::SHT3x() {
  this->_address = 0;
  this->_error = SHT_ERROR_CODES_t::SHT_ERR_OK;
  this->_mode = SHT_REP_MODES_t::SHT_REP_MODE_UNDEF;
  this->_rawHumidity = 0;
  this->_rawTemperature = 0;
}

uint8_t SHT3x::crc8(const uint8_t* data, uint8_t len) {
  const uint8_t POLY(0x31);
  uint8_t crc(0xFF);

  for (uint8_t j=len; j; --j) {
    crc ^= *data++;
    for (uint8_t i=8; i; --i) {
      crc = (crc & 0x80) ? (crc << 1) ^ POLY : (crc << 1);
    }
  }
  return crc;
}

bool SHT3x::writeRegister(uint16_t _cmd) {
  Wire.beginTransmission(this->_address);
  Wire.write(_cmd >> 8 );
  Wire.write(_cmd & 0xFF);
  if (Wire.endTransmission() != 0)
  {
    this->_error = SHT_ERROR_CODES_t::SHT_ERR_WRITE;
    return false;
  }
  return true;
}

bool SHT3x::readRegister(uint8_t* data, uint8_t count) {
  uint8_t _cnt = Wire.requestFrom(this->_address, count);
  if (_cnt == count) {
    for (uint8_t i=0; i<count; i++) {
      data[i] = Wire.read();
    }
    return true;
  }
  this->_error = SHT_ERROR_CODES_t::SHT_ERR_READ;
  return false;
}

uint8_t SHT3x::getErrorCode(void) {
  return this->_error;
}

bool SHT3x::setRepMode(uint16_t mode) {
  if ((mode != SHT_REP_MODES_t::SHT_REP_MODE_LOW) && (mode != SHT_REP_MODES_t::SHT_REP_MODE_MED) && (mode != SHT_REP_MODES_t::SHT_REP_MODE_HI)) {
    this->_mode = SHT_REP_MODES_t::SHT_REP_MODE_UNDEF;
    return false;
  }
  this->_mode = mode;
  return true;
}

bool SHT3x::begin(const uint8_t addr, const uint16_t mode) {
  if ((addr != SHT_ADDR_t::SHT_ADDR_DEFAULT) && (addr != SHT_ADDR_t::SHT_ADDR_ALT)) {
    return false;
  }
  this->_address = addr;
  if (!this->setRepMode(mode)) {
    LOG_ERROR("Wrong repeatability mode");
    return false;
  }
  if (!this->isConnected()) {
    return false;
  }
  return this->reset();
}

bool SHT3x::reset(void) {
  bool _ret = this->writeRegister(SHT_REGISTER_MAP_t::SHT_REG_SOFT_RESET);
  delay(10);
  return _ret;
}

uint16_t SHT3x::readStatus(void) {
  uint8_t status[3] = {0, 0, 0};
  if (this->writeRegister(SHT_REGISTER_MAP_t::SHT_REG_STATUS) == false) {
    return 0xFFFF;
  }

  if (this->readRegister((uint8_t*)&status[0], 3) == false) {
    return 0xFFFF;
  }

  if (status[2] != this->crc8(status, 2)) {
    this->_error = SHT_ERROR_CODES_t::SHT_ERR_CRC_STATUS;
    return 0xFFFF;
  }

  return (uint16_t) (status[0] << 8) + status[1];
}

bool SHT3x::isConnected(void) {
  Wire.beginTransmission(this->_address);
  uint8_t _ret = Wire.endTransmission();
  if (_ret != 0) {
    this->_error = SHT_ERROR_CODES_t::SHT_ERR_NOT_CONNECT;
  }
  return (_ret == 0);
}

bool SHT3x::readSensorData(void) {
  if (this->_mode == SHT_REP_MODES_t::SHT_REP_MODE_UNDEF) {
    LOG_ERROR("Error! Set correct repeatability mode");
    return false;
  }
  if (this->writeRegister(this->_mode) == false) {
    return false;
  }
  delay(20);
  uint8_t buffer[6] = {0};
  if (this->readRegister((uint8_t*)&buffer[0], 6) == false) {
    return false;
  }
  if (buffer[2] != this->crc8(buffer, 2)) {
    this->_error = SHT_ERROR_CODES_t::SHT_ERR_CRC_TEMP;
    return false;
  }
  if (buffer[5] != this->crc8(buffer + 3, 2)) {
    this->_error = SHT_ERROR_CODES_t::SHT_ERR_CRC_HUM;
    return false;
  }
  this->_rawTemperature = (buffer[0] << 8) | (buffer[1]);
  this->_rawHumidity = (buffer[3] << 8) | (buffer[4]);
  return true;
}

float SHT3x::getTemperature(void) {
  return (float)(this->_rawTemperature * (175.0f / 65535.0f) - 45.0f);
}

float SHT3x::getHumidity(void) {
  return (float)(this->_rawHumidity * (100.0f / 65535.0f));
}