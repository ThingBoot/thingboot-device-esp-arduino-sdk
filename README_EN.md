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

## Supported Platforms

| Platform | MCU | Status |
|----------|-----|--------|
| ESP8266 | ESP8266EX | ✅ |
| ESP32 | ESP32-WROOM / WROVER | ✅ |
| ESP32-S3 | ESP32-S3-WROOM | ✅ |
| ESP32-C6 | ESP32-C6 | ✅ |

## Quick Start

### Option 1: PlatformIO (Recommended)

#### 1. Add Dependency

In your project's `platformio.ini`:

```ini
[env:esp8266]
platform = espressif8266
board = esp12e
framework = arduino

; Optional feature switches. extra_script.py will automatically pick the matching precompiled library.
; build_flags = -DTBD_ETHER          ; Enable Ethernet support
; build_flags = -DTBD_GSM            ; Enable GSM support
; build_flags = -DTBD_DEBUG          ; Enable SDK debug log output
; build_flags = -DTBD_ETHER -DTBD_GSM -DTBD_DEBUG

lib_deps = https://github.com/ThingBoot/thingboot-device-esp-arduino-sdk.git
```

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
    device.Order.onOrder([](const char* name, JSONVar data) {
        Serial.print("Received order: ");
        Serial.println(name);
        device.Order.replyMessage("{\"result\":\"ok\"}");
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
version=1.0.1
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
│   ├── libthingboot_device_esp8266.a
│   ├── libthingboot_device_esp8266_debug.a
│   ├── libthingboot_device_esp8266_ether.a
│   ├── libthingboot_device_esp8266_ether_debug.a
│   ├── libthingboot_device_esp8266_gsm.a
│   ├── libthingboot_device_esp8266_gsm_debug.a
│   ├── libthingboot_device_esp8266_ether_gsm.a
│   ├── libthingboot_device_esp8266_ether_gsm_debug.a
│   ├── libthingboot_device_esp32.a
│   └── ...                       # ESP32 variants
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

## Feature Switches

| Macro | Description | Library filename suffix |
|---|---|---|
| `TBD_ETHER` | Enable Ethernet (W5500) support | `_ether` |
| `TBD_GSM` | Enable 4G Cat.1 (ML307) support | `_gsm` |
| `TBD_DEBUG` | Enable SDK internal debug log output | `_debug` |

Multiple switches can be combined, e.g. `-DTBD_ETHER -DTBD_GSM -DTBD_DEBUG` selects `libthingboot_device_esp8266_ether_gsm_debug.a`.

## Third-Party Open-Source Software Notice

This SDK is built upon the following open-source projects (in addition to the Arduino core libraries). Their licenses are independent of this SDK's commercial terms:

| Project | Repository | License |
|---|---|---|
| ArduinoHttpClient | https://github.com/arduino-libraries/ArduinoHttpClient | Apache-2.0 |
| Arduino_JSON (ArduinoJson) | https://github.com/bblanchon/ArduinoJson | MIT |
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
