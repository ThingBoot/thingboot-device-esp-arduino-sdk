#ifndef THINGBOOT_SDK_TYPES_H
#define THINGBOOT_SDK_TYPES_H

#include <Arduino.h>
#include <functional>
#include <Arduino_JSON.h>
#include <EEPROM.h>

#ifdef ESP32
#include <ESP.h>
#include <MD5Builder.h>
#include <esp_task_wdt.h>
#endif

#ifdef ESP32
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <esp_wifi.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#endif
#ifdef TBD_ETHER
#include <Ethernet.h>
#endif
#include <ArduinoHttpClient.h>
#include <PubSubClient.h>

#ifdef ESP32
#include <Update.h>
#else
#include <Updater.h>
#endif

#include <Ticker.h>

#define THINGBOOT_SDK_VERSION "1.0.1"

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
