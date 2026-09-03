#ifndef THINGBOOT_SDK_TYPES_H
#define THINGBOOT_SDK_TYPES_H

#include <Arduino.h>
#include <functional>
#include "JSON.h"
#include <EEPROM.h>

#ifdef ESP32
#include <ESP.h>
#include <MD5Builder.h>
#include <esp_task_wdt.h>
#endif

#ifdef ESP32
#include <WiFi.h>
#include <WiFiMulti.h>
#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
#include <NetworkClientSecure.h>
#else
#include <WiFiClientSecure.h>
#endif
#include <esp_wifi.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#endif
// Ethernet.h 不再由公共头引入：以太网实现已迁入 addon（common/net_ether.h 自行包含）
#include <ArduinoHttpClient.h>
#include <PubSubClient.h>

#ifdef ESP32
#include <Update.h>
#else
#include <Updater.h>
#endif

#include <Ticker.h>

#define THINGBOOT_SDK_VERSION "1.5.0"

#include <ThingBootSDK/Enums.h>
#include <ThingBootSDK/Errors.h>

// 外设最大数量（ESP32 下为 8，ESP8266 下为其一半 4）
#ifdef ESP32
#define BTN_MAX     8
#define LED_MAX     8
#else
#define BTN_MAX     4
#define LED_MAX     4
#endif

// ROM 配置映射最大项数（ESP32 下 64 个，ESP8266 下 32 个）
#ifdef ESP32
#define ROM_DEVICE_MAX 64
#else
#define ROM_DEVICE_MAX 32
#endif

class ThingBootDeviceImpl;

#endif
