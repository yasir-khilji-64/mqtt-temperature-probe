/**
 * @file Logger.h
 * @author Yasir Hassan <yasir.hassan1993@hotmail.com>
 * @brief Macro definitions for Serial Logging
 * @version 0.1
 * @date 2022-07-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef LOGGER_H
#define LOGGER_H

#if DEBUG
#define LOG_DEBUG(x, ...) Serial.printf("[DEBUG: %s:%d] " x "\r\n", __FILE__, __LINE__, ##__VA_ARGS__);
#define LOG_INFO(x, ...) Serial.printf("[INFO: %s:%d] " x "\r\n", __FILE__, __LINE__, ##__VA_ARGS__);
#define LOG_WARN(x, ...) Serial.printf("[WARN: %s:%d] " x "\r\n", __FILE__, __LINE__, ##__VA_ARGS__);
#define LOG_ERROR(x, ...) Serial.printf("[ERROR: %s:%d] " x "\r\n", __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define LOG_DEBUG(x, ...) do {} while(0);
#define LOG_INFO(x, ...) do {} while(0);
#define LOG_WARN(x, ...) do {} while(0);
#define LOG_ERROR(x, ...) do {} while(0);
#endif

#endif