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
  SHT_REG_SOFT_RESET                    = 0x30A2,
  SHT_REG_STATUS                        = 0xF32D,
} SHT_REGISTER_MAP_t;

typedef enum {
  SHT_REP_MODE_LOW    = 0x2416,
  SHT_REP_MODE_MED    = 0x240B,
  SHT_REP_MODE_HI     = 0x2400,
  SHT_REP_MODE_UNDEF  = 0xFFFF,
} SHT_REP_MODES_t;

typedef enum {
  SHT_ERR_OK            = 0x00,
  SHT_ERR_WRITE         = 0x81,
  SHT_ERR_READ          = 0x82,
  SHT_ERR_NOT_CONNECT   = 0x84,
  SHT_ERR_CRC_TEMP      = 0x85,
  SHT_ERR_CRC_HUM       = 0x86,
  SHT_ERR_CRC_STATUS    = 0x87,
} SHT_ERROR_CODES_t;

class SHT3x {
  public:
    SHT3x();

    bool begin(const uint8_t addr = SHT_ADDR_t::SHT_ADDR_DEFAULT, const uint16_t mode = SHT_REP_MODES_t::SHT_REP_MODE_HI);
    bool setRepMode(uint16_t mode);
    bool reset(void);
    bool isConnected(void);
    bool readSensorData();
    uint8_t getErrorCode(void);
    uint16_t readStatus(void);
    uint16_t getRawHumidity(void) { return this->_rawHumidity; }
    uint16_t getRawTemperature(void) { return this->_rawTemperature; }
    float getTemperature(void);
    float getHumidity(void);
  private:
    uint8_t _address;
    uint8_t _error;
    uint16_t _mode;
    uint16_t _rawTemperature;
    uint16_t _rawHumidity;
    uint32_t _lastRequestTime;

    uint8_t crc8(const uint8_t* data, uint8_t len);
    bool writeRegister(uint16_t cmd);
    bool readRegister(uint8_t* data, uint8_t count);
};

#endif