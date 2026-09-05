<p align="right">
  English | <a href="./README.md">中文</a>
</p>

<h1 align="center">ThingBoot Device SDK</h1>
<p align="center"><b>Enterprise-grade free SDK for developers — works out of the box, zero learning curve</b></p>

---

> **License & Billing Notice**
>
> - This SDK is **free** for developers, distributed as precompiled closed-source libraries (`.a` + headers), with no per-developer or per-build limits.
> - Each developer account gets **100 free device connections per year** — enough to cover a dev team's annual prototyping volume. Devices simply register on the ThingBoot Console.
> - Managing and controlling devices via the Open Platform API is metered and billed (mainly used by end customers; developers typically don't need it).
> - Fees apply only when a product enters **mass production**: the factory pays the platform access fee, plus optional Open Platform API usage fees.
> - With this policy, we hope to encourage developers to build and retrofit hardware products on the ThingBoot standard.
> - Unauthorized redistribution, reverse engineering, decompilation, or development of competing products/platforms based on this SDK is prohibited.

---

## Features

- **Full-Stack Network Access**: Built-in WiFi, Ethernet, 4G Cat.1, and WiFi Mesh networking.
- **Cloud-Ready**: Out-of-the-box MQTT/HTTPS connection to the ThingBoot platform with auto-reconnect and OTA upgrades.
- **Local Network Service**: Pure LAN mode with HTTP/TCP endpoints for offline local control.
- **Hardware Minimalism**: Developers only write peripheral drivers (LED, relay, sensor, etc.); platform-side thing models close the feature loop.
- **Precompiled & Closed-Source**: Distributed as `.a` static libraries + headers. Source code is not available.

## Capability Overview

### Connectivity & Provisioning

- **WiFi**: multi-candidate connection (5 candidates + hidden APs), automatic reconnect, zombie-AP (connected but no Internet) detection and failover
- **AP provisioning**: automatic `TBC-<deviceID>` hotspot + HTTP config API when unconfigured — connect a phone and go
- **Engineering hotspots**: automatic recognition of safe / factory / test / debug / config hotspots — factory activation, production testing and debugging with zero code

### Network Modes

- **WiFi**: built into the base library, works out of the box (ESP8266 / all ESP32 variants)
- **Ethernet / GSM**: on-demand addons (W5500 SPI Ethernet, ML307 4G Cat.1)
- **WiFi Mesh**: ESP-NOW master/slave, gateway, discovery & binding (find/bind), message bridging, channel management

### Platform Connection

- **Registration & activation**: reg/v4 signing protocol, activation-code verification, workbench assignment
- **MQTT**: 8 downlink topic subscriptions, last will, QoS1, automatic reconnect, uplink/downlink packing and dispatch
- **Thing model**: full Order / Event / State / Config pipelines
- **OTA**: three-stage flow (signed download → magic pre-check & flash → reboot → report at registration) with automatic retries
- **NTP**: dual-channel sync (via registration + MQTT), timezone management

### Local & Private Deployment

- **LAN services**: HTTP/TCP interfaces in LAN-only mode — **local control even without Internet**
- **Private deployment**: private broker (MQTT credentials / topics / registration path override), private gateway (HTTP sync responses / heartbeat)

### Gateway (addon)

- **Child table**: child ID / key / active-time maintenance with persistence and automatic online detection (configurable timeout)
- **Message forwarding**: platform orders routed to children through the product's own transport (RF / RS485 / Zigbee, ...); child messages uplinked via the gateway
- **Platform consistency**: change events (add/remove/online/offline), reconcile-on-connect, platform management primitives — all built in, zero code

### Developer Experience

- **Addon model**: base library always linked + addons on demand (ether / gsm / gateway) — `install()` and go
- **Debug logs**: always compiled in; register `device.onDebug()` to receive category-tagged logs, no build flags
- **Peripheral framework**: button debounce / long-press system menu (5 s provisioning, 7 s safe mode), three-state LED machine
- **Config system**: declarative config items (defaults / range checks / readable & writable via provisioning and platform)
- **Tooling**: JSON compatibility layer (cJSON-based, Arduino_JSON-compatible usage), timers, full error-code set, boot logs that decode cleanly


## Supported Platforms

| Platform | MCU | Status |
|----------|-----|--------|
| ESP8266 | ESP8266EX | ✅ |
| ESP32 | ESP32-WROOM / WROVER | ✅ |
| ESP32-S2 | ESP32-S2 | ✅ |
| ESP32-S3 | ESP32-S3-WROOM | ✅ |
| ESP32-C3 | ESP32-C3 | ✅ |
| ESP32-C6 | ESP32-C6 | ✅ |
| ESP32-C2 / C61 | - | ❌ Requires ESP-IDF component mode |

## Build Environment

### ESP8266
- Framework: [Arduino-ESP8266](https://github.com/esp8266/Arduino)
- PlatformIO Platform: `espressif8266`
- Framework Version: `3.30102.0` (Arduino-ESP8266 3.1.2)

### ESP32
- Framework: [Arduino-ESP32](https://github.com/espressif/arduino-esp32)
- PlatformIO Platform: `espressif32`
- Framework Version: `3.3.2`

> ESP32-C2 and ESP32-C61 require ESP-IDF component mode and are not supported by the Arduino SDK.

## Quick Start

### Option 1: PlatformIO (Recommended)

#### 1. Add Dependency

In your project's `platformio.ini`:

```ini
[env:esp8266]
platform = espressif8266
board = esp12e
framework = arduino

lib_deps = https://github.com/ThingBoot/thingboot-device-esp-arduino-sdk.git
```

WiFi connectivity works out of the box (the base library is linked automatically). For Ethernet / GSM see "Connectivity Addons" below. Debug logs need no switch — register a `device.onDebug` callback to receive them.

#### 2. Include Header

```cpp
#include <Arduino.h>
#include "ThingBootSDK.h"
```

#### 3. Write Your Application

```cpp
ThingBootDevice device;

void setup() {
    Serial.begin(115200);

    // Set product information (obtain from the ThingBoot console)
    device.setProduct("your-product-key", "your-product-secret",
                      "your-board", "your-mcu", "your-firmware-version");

    // Register platform command callback
    device.Order.onOrder([](const char* mid, JSONVar data) {
        Serial.print("Received order: ");
        Serial.println(JSON.stringify(data));
        device.Order.replyMessage(mid, "{\"result\":\"ok\"}");
    });

    // Initialize SDK (connect network, log in to platform, etc.)
    device.setup();
}

void loop() {
    device.loop();  // Maintain SDK main loop, heartbeat, reconnection, etc.
}
```

### Option 2: Arduino IDE

1. Download `dist.zip` for the desired version from the [Releases page](https://github.com/ThingBoot/thingboot-device-esp-arduino-sdk/releases).
2. Extract to get `include/`, `lib/`, `examples/`, etc.
3. In Arduino IDE select **Sketch → Include Library → Add .ZIP Library...**, or manually copy the folder to `Documents/Arduino/libraries/ThingBootSDK/`.
4. Select the `.a` library for your target board:
   - ESP8266: `lib/libthingboot_device_esp8266.a`
   - ESP32: `lib/libthingboot_device_esp32.a`
5. Rename the selected `.a` file to `libThingBootSDK.a` and place it in the library's `src/` folder.
6. Create `library.properties` in the library folder:

```properties
name=ThingBootSDK
version=1.3.0
author=ThingBoot
maintainer=support@thingboot.com
sentence=ThingBoot Device SDK
paragraph=Precompiled closed-source SDK for connecting ESP devices to ThingBoot IoT platform.
category=Communication
url=https://github.com/ThingBoot/thingboot-device-esp-arduino-sdk
architectures=esp8266,esp32
precompiled=true
```

7. Open **File → Examples → ThingBootSDK → BasicUsage**, compile and upload.

> Arduino IDE is recommended only for quick evaluation. Feature switches (Ethernet/GSM/Debug) are best managed through `platformio.ini`.

## Directory Structure

```plain
lib/thingboot-device-esp-arduino-sdk/
├── include/
│   └── ThingBootSDK.h            # Public API header
│   └── ThingBootSDK/             # Public headers grouped by module
├── lib/
│   ├── libthingboot_device_esp8266.a            # Base libraries (per chip, linked automatically)
│   ├── libthingboot_device_esp32.a
│   ├── libthingboot_addon_net_ether_esp8266.a   # Addon library (Ethernet, on demand)
│   ├── libthingboot_addon_net_gsm_esp8266.a     # Addon library (GSM, on demand)
│   └── ...                       # Base + addons for the other chips
├── examples/
│   └── BasicUsage/
│       ├── BasicUsage.ino
│       └── platformio.ini
├── library.json
├── extra_script.py
├── README.md
├── README_EN.md
└── LICENSE.md
```

## Connectivity Addons

Since v1.3, connectivity modules use an addon model: the base library includes WiFi and all platform capabilities; Ethernet / GSM ship as separate addon libraries, enabled on demand. Two steps:

1. Add the switch to `build_flags` in `platformio.ini` — `extra_script.py` will link the matching addon library:
   - `-DTBD_ETHER` → `libthingboot_addon_net_ether_<chip>.a` (Ethernet, W5500)
   - `-DTBD_GSM` → `libthingboot_addon_net_gsm_<chip>.a` (4G Cat.1 ML307; also add the `GSM` library (TinyGSM) to `lib_deps`)
2. Call the install function in your code before `device.setup()` to register the driver (member-style calls are equivalent wrappers of the free functions `tb_addon_net_*_install()`):

```cpp
device.Network.installEthernet();  // Ethernet
device.Network.installGSM();       // GSM/4G
```

Calling the related `Network` APIs without an installed driver returns `ERR_NETWORK_DRIVER_MISSING` (20002). Both addons can be enabled together.

> `TBD_DEBUG` is deprecated: since v1.3 debug logs are always compiled in; register a `device.onDebug` callback to receive them — no build flag needed.

## Gateway Addon

For gateway products (child device table maintenance + platform message forwarding, transport-agnostic): add `-DTBD_GATEWAY` to `build_flags` and call `device.Gateway.install()` before `device.setup()` (a wrapper of the free function `tb_addon_gateway_install()`; install declares the role), then register the child order callback via `device.Gateway.onChildOrder()`. See the "Gateway" chapter of the official documentation.

> **ESP8266 with 1MB flash**: the gateway addon stores the child table in LittleFS — OTA will not be possible on 1MB parts. Do not use it on products that need OTA.

## Third-Party Open-Source Software Notice

This SDK is built upon the following open-source projects (in addition to the Arduino core libraries). Their licenses are independent of this SDK's commercial terms:

| Project | Repository | License |
|---|---|---|
| ArduinoHttpClient | https://github.com/arduino-libraries/ArduinoHttpClient | Apache-2.0 |
| cJSON | https://github.com/DaveGamble/cJSON | MIT |
| PubSubClient | https://github.com/knolleary/pubsubclient | MIT |

You are responsible for ensuring that your final firmware complies with the applicable open-source license requirements. ThingBoot assumes no liability for third-party open-source license compliance.

## Licensing & Billing

- **Free SDK**: free to download, compile and develop with; no per-developer or per-build limits.
- **100 free devices per year**: each developer account can connect 100 devices per year at no charge (covers prototyping volume); register devices on the [ThingBoot Console](https://console.thingboot.com).
- **Production-stage billing**: once a product enters mass production, the factory pays the platform access fee; Open Platform API usage (optional, mainly for end customers) is metered separately.
- **Purchase & inquiries**: contact `license@thingboot.com` or purchase via the ThingBoot Console.

## Support

- Technical Support: `support@thingboot.com`
- Sales & Licensing: `license@thingboot.com`
- Documentation: [docs.thingboot.com](https://docs.thingboot.com)

---

**Copyright © 2026 ThingBoot Technology Co., Ltd. All rights reserved.**
