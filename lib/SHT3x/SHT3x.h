/**
 * @file SHT3x.h
 * @author Yasir Hassan <yasir.hassan1993@hotmail.com>
 * @brief Class definition for SHT3x Sensor. This class
 *        implements general I2C commands for SHT3x Sensor
 * @version 0.1
 * @date 2022-07-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef SHT3X_H
#define SHT3X_H

#include <Arduino.h>

typedef enum {
  SHT_ADDR_DEFAULT  = 0x44,
  SHT_ADDR_ALT      = 0x45,
} SHT_ADDR_t;

typedef enum {
  SHT_REG_SOFT_RESET    = 0x30A2,
  SHT_REG_STATUS        = 0xF32D,
} SHT_REGISTER_MAP_t;

typedef enum {
  SHT_ERR_OK            = 0x00,
  SHT_ERR_WRITE         = 0x81,
  SHT_ERR_READ          = 0x82,
  SHT_ERR_NOT_CONNECT   = 0x84,
  SHT_ERR_CRC_STATUS    = 0x87,
} SHT_ERROR_CODES_t;

class SHT3x {
  public:
    SHT3x();

    bool begin(const uint8_t _addr = SHT_ADDR_t::SHT_ADDR_DEFAULT);
    bool reset(void);
    uint16_t readStatus(void);
    uint8_t getErrorCode(void);
  private:
    uint8_t _address;
    uint8_t _error;

    uint8_t crc8(const uint8_t* data, uint8_t len);
    bool writeRegister(uint16_t cmd);
    bool readRegister(uint8_t* data, uint8_t count);
};

#endif