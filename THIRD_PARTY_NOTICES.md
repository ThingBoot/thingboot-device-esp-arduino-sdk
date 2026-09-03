# Third-Party Notices

ThingBoot Device SDK （以下简称“本 SDK”）在编译和运行过程中依赖若干第三方开源项目。
本文件用于说明这些依赖及其许可证信息，方便最终开发者在固件中遵守相应的开源许可义务。

> **免责声明**：本文件仅作技术信息汇总，不构成法律意见。具体的许可证合规要求请咨询专业律师。

---

## 1. 平台/框架依赖

本 SDK 面向 ESP8266 / ESP32 Arduino 平台开发，最终固件会链接到以下 LGPL 许可的平台核心库。

| 项目 | 仓库 | 许可证 | 用途 |
|---|---|---|---|
| ESP8266 Arduino Core | https://github.com/esp8266/Arduino | LGPL-2.1 | ESP8266 平台 Arduino 框架 |
| arduino-esp32 | https://github.com/espressif/arduino-esp32 | LGPL-2.1 | ESP32 平台 Arduino 框架 |

> **说明**：本 SDK 本身没有复制或修改上述 Arduino Core 的源代码，仅通过 `#include` 使用其公共头文件，并在编译时由 PlatformIO 链接到对应平台框架。

---

## 2. 本 SDK 直接依赖的库

下表列出了本 SDK 在内部使用并可能随 SDK 一起分发的第三方库。

| 项目 | 仓库 | 许可证 | 是否打包进 `.a` |
|---|---|---|---|
| cJSON | https://github.com/DaveGamble/cJSON | MIT | 是（JSON 兼容层基础） |
| ArduinoHttpClient | https://github.com/arduino-libraries/ArduinoHttpClient | Apache-2.0 | 是 |
| PubSubClient | https://github.com/knolleary/pubsubclient | MIT | 是 |

> **“条件性”** 表示：只有构建启用了对应功能（如 `-DTBD_ETHER`、`-DTBD_GSM`）的变体时，这些库的对象文件才会被打包进 `.a`。

---

## 3. LGPL 重新链接说明

本 SDK 以**静态库（`.a`）**形式分发。SDK 库本身**不包含任何 LGPL 组件的对象文件**（直接依赖全部为 MIT/Apache-2.0）；LGPL 仅来自最终固件链接的平台核心库（ESP8266 Arduino Core / arduino-esp32，由 PlatformIO 提供并链接，本 SDK 不复制其源码）。

若最终产品固件包含上述 LGPL 平台组件，开发者应遵守 LGPL 关于源代码/对象文件提供、许可声明、修改告知等义务。ThingBoot 仅提供 SDK 层面的依赖信息，最终固件的合规责任由最终开发者承担。

---

## 4. 许可证原文链接

- LGPL-2.1: https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
- LGPL-3.0: https://www.gnu.org/licenses/lgpl-3.0.html
- Apache-2.0: https://www.apache.org/licenses/LICENSE-2.0
- MIT: https://opensource.org/licenses/MIT
- GPL-3.0: https://www.gnu.org/licenses/gpl-3.0.html
