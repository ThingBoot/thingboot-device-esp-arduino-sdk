<p align="right">
  <a href="./README_EN.md">English</a> | 中文
</p>

<h1 align="center">芯步设备 SDK</h1>
<p align="center">ThingBoot Device SDK — 预编译嵌入式物联网开发库</p>

---

> **授权与计费说明**
>
> - 本 SDK **本身免费**向开发者提供，以预编译闭源库（`.a` + 头文件）形式分发。
> - SDK 不收取任何授权费用、不限开发人数、不限编译次数。
> - 当设备通过本 SDK **连接芯步（ThingBoot）云平台**时，按接入设备数量收取平台接入服务费。
> - 未经授权，禁止以任何形式再分发、逆向工程、反编译本 SDK，或基于本 SDK 开发同类产品/平台。

---

## 特性

- **全栈网络接入**：内置 WiFi、以太网、4G Cat.1 及 WiFi Mesh 组网
- **平台直连**：开箱即用的 MQTT/HTTPS 平台连接，支持自动重连与 OTA 升级
- **局域网服务**：纯局域网模式下开放 HTTP/TCP 接口，支持无网本地控制
- **硬件极简**：开发者仅需编写 LED、继电器、传感器等硬件驱动，平台端定义物模型即可完成功能闭环
- **预编译闭源**：以 `.a` 静态库 + 头文件分发，源码不开放

## 支持平台

| 平台 | 芯片 | 状态 |
|------|------|------|
| ESP8266 | ESP8266EX | ✅ |
| ESP32 | ESP32-WROOM / WROVER | ✅ |
| ESP32-S3 | ESP32-S3-WROOM | ✅ |
| ESP32-C6 | ESP32-C6 | ✅ |

## 快速开始

### 方式一：PlatformIO（推荐）

#### 1. 添加依赖

在项目的 `platformio.ini` 中：

```ini
[env:esp8266]
platform = espressif8266
board = esp12e
framework = arduino

; 可选功能开关，extra_script.py 会根据开关自动选择对应预编译库
; build_flags = -DTBD_ETHER          ; 启用以太网支持
; build_flags = -DTBD_GSM            ; 启用 GSM 支持
; build_flags = -DTBD_DEBUG          ; 启用 SDK 调试日志输出
; build_flags = -DTBD_ETHER -DTBD_GSM -DTBD_DEBUG

lib_deps = https://github.com/ThingBoot/thingboot-device-sdk.git#v1.0.1
```

#### 2. 包含头文件

```cpp
#include <Arduino.h>
#include "ThingBootSDK.h"
```

#### 3. 编写应用

```cpp
ThingBootDevice device;

void setup() {
    Serial.begin(115200);

    // 设置产品信息（在芯步控制台获取）
    device.setProduct("your-product-key", "your-product-secret",
                      "your-board", "your-mcu", "your-firmware-version");

    // 注册平台命令回调
    device.Order.setOrderCallback([](const char* name, JSONVar data) {
        Serial.print("收到命令: ");
        Serial.println(name);
        device.Order.replyMessage("{\"result\":\"ok\"}");
    });

    // 初始化 SDK（连接网络、登录平台等）
    device.setup();
}

void loop() {
    device.loop();  // 维持 SDK 主循环、心跳、重连等
}
```

### 方式二：Arduino IDE

1. 从 [Release 页面](https://github.com/ThingBoot/thingboot-device-sdk/releases) 下载对应版本的 `dist.zip`。
2. 解压后得到 `include/`、`lib/`、`examples/` 等目录。
3. 在 Arduino IDE 中选择 **项目 → 加载库 → 添加 .ZIP 库...**，或手动将文件夹复制到 `文档/Arduino/libraries/ThingBootSDK/`。
4. 根据目标板选择对应架构的 `.a` 文件：
   - ESP8266：`lib/libthingboot_device_esp8266.a`
   - ESP32：`lib/libthingboot_device_esp32.a`
5. 将选中的 `.a` 文件重命名为 `libThingBootSDK.a`，放入库目录的 `src/` 下。
6. 在库目录下创建 `library.properties`：

```properties
name=ThingBootSDK
version=1.0.1
author=ThingBoot
maintainer=support@thingboot.com
sentence=ThingBoot Device SDK
paragraph=Precompiled closed-source SDK for connecting ESP devices to ThingBoot IoT platform.
category=Communication
url=https://github.com/ThingBoot/thingboot-device-sdk
architectures=esp8266,esp32
precompiled=true
```

7. 打开示例 **文件 → 示例 → ThingBootSDK → BasicUsage**，编译上传。

> Arduino IDE 仅建议用于快速体验；功能开关（以太网/GSM/Debug）需要配合 `platformio.ini` 才能灵活切换。

## 目录结构

```plain
lib/thingboot-device-sdk/
├── include/
│   └── ThingBootSDK.h            # 公共 API 头文件
│   └── ThingBootSDK/             # 按模块拆分的公共头文件
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
│   └── ...                       # ESP32 各变体
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

## 功能开关说明

| 宏 | 说明 | 生成的库名后缀 |
|---|---|---|
| `TBD_ETHER` | 启用以太网（W5500）支持 | `_ether` |
| `TBD_GSM` | 启用 4G Cat.1（TinyGSM）支持 | `_gsm` |
| `TBD_DEBUG` | 启用 SDK 内部调试日志输出 | `_debug` |

多个开关可组合使用，例如 `-DTBD_ETHER -DTBD_GSM -DTBD_DEBUG` 对应 `libthingboot_device_esp8266_ether_gsm_debug.a`。

## 第三方开源软件声明

本 SDK 基于以下开源项目构建（除 Arduino 核心库外），其授权协议独立于本 SDK 的商业条款：

| 项目 | 仓库 | 许可证 |
|---|---|---|
| ArduinoHttpClient | https://github.com/arduino-libraries/ArduinoHttpClient | Apache-2.0 |
| Arduino_JSON (ArduinoJson) | https://github.com/bblanchon/ArduinoJson | MIT |
| PubSubClient | https://github.com/knolleary/pubsubclient | MIT |
| Ethernet3 | https://github.com/sstaub/Ethernet3 | LGPL-2.1 |
| TinyGSM | https://github.com/vshymanskyy/TinyGSM | LGPL-3.0 |

用户需确保最终固件符合所使用开源组件的许可证要求。芯步不对第三方开源组件的许可证合规性承担责任。

## 授权与计费

- **SDK 本身免费**：开发者可免费下载、编译、开发，不限制开发人数和编译次数。
- **按设备接入计费**：设备通过本 SDK 连接芯步云平台时，按接入设备数量收取平台服务费。
- **设备接入方式**：设备在 [芯步控制台](https://console.thingboot.com) 注册并接入。
- **购买与咨询**：联系 `license@thingboot.com` 或通过芯步控制台购买。

## 技术支持

- 技术支持：`support@thingboot.com`
- 销售授权：`license@thingboot.com`
- 官方文档：[docs.thingboot.com](https://www.thingboot.com/document/fb9d5f99e09c8673/)

---

**版权所有 © 2026 芯步（ThingBoot）科技有限公司。保留所有权利。**
