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

bool SHT3x::begin(const uint8_t _addr) {
  if ((_addr != SHT_ADDR_t::SHT_ADDR_DEFAULT) && (_addr != SHT_ADDR_t::SHT_ADDR_ALT)) {
    return false;
  }
  this->_address = _addr;
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