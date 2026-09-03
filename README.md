<p align="right">
  <a href="./README_EN.md">English</a> | 中文
</p>

<h1 align="center">芯步设备 SDK</h1>
<p align="center">ThingBoot Device SDK</p>
<p align="center"><b>面向开发者的企业级免费 SDK —— 开箱即用，零学习成本</b></p>

---

> **授权与计费说明**
>
> - 本 SDK **免费**向开发者开放，以预编译闭源库（`.a` + 头文件）形式分发，不限开发人数、不限编译次数。
> - 每个开发者帐号每年享有 **100 台设备免费接入额度**——几乎覆盖一个开发者团队全年产品开发涉及的样机量；设备在芯步控制台注册即可接入。
> - 通过开放平台调用 API 管理和控制设备按量收费（该部分主要面向终端客户，开发者通常无需使用）。
> - 仅在产品进入**量产阶段**时，才向工厂收取平台接入费，以及开放平台接口调用费（可选）。
> - 我们希望通过这个措施，鼓励开发者使用芯步标准来开发和改造硬件产品。
> - 未经授权，禁止以任何形式再分发、逆向工程、反编译本 SDK，或基于本 SDK 开发同类产品/平台。

---

## 特性

- **全栈网络接入**：内置 WiFi、以太网、4G Cat.1 及 WiFi Mesh 组网
- **平台直连**：开箱即用的 MQTT/HTTPS 平台连接，支持自动重连与 OTA 升级
- **局域网服务**：纯局域网模式下开放 HTTP/TCP 接口，支持无网本地控制
- **硬件极简**：开发者仅需编写 LED、继电器、传感器等硬件驱动，平台端定义物模型即可完成功能闭环
- **预编译闭源**：以 `.a` 静态库 + 头文件分发，源码不开放

## 功能概览

### 接入与配网

- **WiFi 联网**：多候选连接（5 个候选 + 隐藏热点）、断线自动重连、僵尸热点（能连但无外网）识别与换网
- **AP 配网**：未配置时自动开放 `TBC-<设备ID>` 热点 + HTTP 配置接口，手机直连即配
- **工程热点**：safe / factory / test / debug / config 五类工程热点自动识别——工厂激活、产线测试、开发调试零代码接入

### 网络形态

- **WiFi**：base 库内置，开箱即用（ESP8266 / ESP32 全系）
- **以太网 / GSM**：addon 按需启用（W5500 SPI 以太网、ML307 4G Cat.1）
- **WiFi Mesh 组网**：ESP-NOW 主从 / 网关、发现绑定（find/bind）、消息桥接、信道管理

### 平台连接

- **注册与激活**：reg/v3 签名协议、激活码校验、工作台归属
- **MQTT**：8 个下行主题订阅、遗嘱消息、QoS1、断线自动重连、上下行报文组包与分发
- **物模型**：命令（Order）/ 事件（Event）/ 状态（State）/ 配置（Config）全链路
- **OTA 升级**：三段式流程（签名下载 → 魔数预检烧写 → 重启 → 注册时上报结果），失败自动重试
- **NTP 对时**：注册顺带 + MQTT 请求双通路，时区管理

### 本地与私有化

- **局域网服务**：纯局域网模式开放 HTTP/TCP 接口，**无网也可本地控制**
- **私有化部署**：私有 broker（MQTT 参数 / 主题 / 注册路径替换）、私有网关（HTTP 同步应答 / 心跳）

### 网关（addon）

- **子设备表**：ID / 键 / 活动时间维护，持久化存储，在线状态自动判定（可配时限）
- **消息转发**：平台命令按子设备查表转发到产品传输层（RF / RS485 / Zigbee 等自定），子设备消息经网关上行
- **平台一致性**：变更即上报（增删 / 上下线事件）、上线即对账、平台增删查管理原语，SDK 内置零实现

### 开发体验

- **addon 模型**：base 库必链 + addon 按需追加（ether / gsm / gateway），`install()` 即启用
- **调试日志**：全库恒带，`device.onDebug()` 注册即输出，按模块分类过滤，无需编译开关
- **外设框架**：按钮消抖 / 长按系统菜单（5s 配网 / 7s 安全模式）、LED 三态状态机
- **配置体系**：Config 声明式配置项（默认值 / 范围校验 / 配网与平台可读写）
- **工具链**：JSON 兼容层（cJSON 实现，Arduino_JSON 用法兼容）、Timer 定时器、全量错误码、异常解码友好的启动日志


## 支持平台

| 平台 | 芯片 | 状态 |
|------|------|------|
| ESP8266 | ESP8266EX | ✅ |
| ESP32 | ESP32-WROOM / WROVER | ✅ |
| ESP32-S2 | ESP32-S2 | ✅ |
| ESP32-S3 | ESP32-S3-WROOM | ✅ |
| ESP32-C3 | ESP32-C3 | ✅ |
| ESP32-C6 | ESP32-C6 | ✅ |
| ESP32-C2 / C61 | - | ❌ 需 ESP-IDF 组件方式 |

## 构建环境

### ESP8266
- 框架：[Arduino-ESP8266](https://github.com/esp8266/Arduino)
- PlatformIO 平台：`espressif8266`
- Framework 版本：`3.30102.0`（Arduino-ESP8266 3.1.2）

### ESP32
- 框架：[Arduino-ESP32](https://github.com/espressif/arduino-esp32)
- PlatformIO 平台：`espressif32`
- Framework 版本：`3.3.2`

> ESP32-C2 和 ESP32-C61 需通过 ESP-IDF 组件方式使用，不在 Arduino SDK 支持范围内。

## 快速开始

### 方式一：PlatformIO（推荐）

#### 1. 添加依赖

在项目的 `platformio.ini` 中：

```ini
[env:esp8266]
platform = espressif8266
board = esp12e
framework = arduino

lib_deps = https://github.com/ThingBoot/thingboot-device-esp-arduino-sdk.git
```

默认即为 WiFi 联网（base 库自动链接）。需要以太网 / GSM 时见下文「联网扩展（addon）」；调试日志无需开关，注册 `device.onDebug` 回调即输出。

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

    // 注册平台命令回调（首参为消息 ID mid，应答时原样传回）
    device.Order.onOrder([](const char* mid, JSONVar data) {
        Serial.print("收到命令: ");
        Serial.println(JSON.stringify(data));
        device.Order.replyMessage(mid, "{\"result\":\"ok\"}");
    });

    // 初始化 SDK（连接网络、登录平台等）
    device.setup();
}

void loop() {
    device.loop();  // 维持 SDK 主循环、心跳、重连等
}
```

### 方式二：Arduino IDE

1. 从 [Release 页面](https://github.com/ThingBoot/thingboot-device-esp-arduino-sdk/releases) 下载对应版本的 `dist.zip`。
2. 解压后得到 `include/`、`lib/`、`examples/` 等目录。
3. 在 Arduino IDE 中选择 **项目 → 加载库 → 添加 .ZIP 库...**，或手动将文件夹复制到 `文档/Arduino/libraries/ThingBootSDK/`。
4. 根据目标板选择对应架构的 `.a` 文件：
   - ESP8266：`lib/libthingboot_device_esp8266.a`
   - ESP32：`lib/libthingboot_device_esp32.a`
5. 将选中的 `.a` 文件重命名为 `libThingBootSDK.a`，放入库目录的 `src/` 下。
6. 在库目录下创建 `library.properties`：

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

7. 打开示例 **文件 → 示例 → ThingBootSDK → BasicUsage**，编译上传。

> Arduino IDE 仅建议用于快速体验；联网扩展（以太网/GSM addon）需要配合 `platformio.ini` 才能灵活启用。

## 目录结构

```plain
lib/thingboot-device-esp-arduino-sdk/
├── include/
│   └── ThingBootSDK.h            # 公共 API 头文件
│   └── ThingBootSDK/             # 按模块拆分的公共头文件
├── lib/
│   ├── libthingboot_device_esp8266.a            # base 库（按芯片区分，自动链接）
│   ├── libthingboot_device_esp32.a
│   ├── libthingboot_addon_net_ether_esp8266.a   # addon 库（以太网，按需）
│   ├── libthingboot_addon_net_gsm_esp8266.a     # addon 库（GSM，按需）
│   └── ...                       # 其余芯片的 base + addon
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

## 联网扩展（addon）

v1.3 起联网模块改为 addon 模型：base 库内含 WiFi 与全部平台能力，以太网 / GSM 为独立 addon 库，按需启用。两步：

1. 在 `platformio.ini` 的 `build_flags` 打开开关，`extra_script.py` 会追加链接对应 addon 库：
   - `-DTBD_ETHER` → `libthingboot_addon_net_ether_<芯片>.a`（以太网 W5500）
   - `-DTBD_GSM` → `libthingboot_addon_net_gsm_<芯片>.a`（4G Cat.1 ML307；另需在 `lib_deps` 添加 `GSM` 库（TinyGSM））
2. 在代码中 `device.setup()` 之前调用安装函数注册驱动（成员写法为自由函数 `tb_addon_net_*_install()` 的转发版，等价）：

```cpp
device.Network.installEthernet();  // 以太网
device.Network.installGSM();       // GSM/4G
```

未安装驱动时调用相关 `Network` 接口返回 `ERR_NETWORK_DRIVER_MISSING`（20002）。两个 addon 可同时启用。

> `TBD_DEBUG` 已废弃：v1.3 起调试日志恒带于库中，注册 `device.onDebug` 回调即可输出，无需任何编译宏。

## 网关（addon）

网关类产品（子设备表维护 + 平台消息转发，与传输层无关）：`build_flags` 加 `-DTBD_GATEWAY`，并在 `device.setup()` 前调用 `device.Gateway.install()`（自由函数 `tb_addon_gateway_install()` 的转发版；install 即角色），再用 `device.Gateway.onChildOrder()` 注册子设备命令回调。详见官方文档「网关」章节。

> **ESP8266 1MB Flash 机型注意**：网关 addon 依赖 LittleFS 存储子设备表，1MB 机型将无法支持 OTA，请勿用于需要 OTA 的产品。

## 第三方开源软件声明

本 SDK 基于以下开源项目构建（除 Arduino 核心库外），其授权协议独立于本 SDK 的商业条款：

| 项目 | 仓库 | 许可证 |
|---|---|---|
| ArduinoHttpClient | https://github.com/arduino-libraries/ArduinoHttpClient | Apache-2.0 |
| Arduino_JSON | https://github.com/arduino-libraries/Arduino_JSON | LGPL-2.1 |
| PubSubClient | https://github.com/knolleary/pubsubclient | MIT |

用户需确保最终固件符合所使用开源组件的许可证要求。芯步不对第三方开源组件的许可证合规性承担责任。

## 授权与计费

- **SDK 免费开放**：开发者可免费下载、编译、开发，不限开发人数和编译次数。
- **每年 100 台免费接入**：每个开发者帐号每年可免费接入 100 台设备（覆盖开发样机量），设备在 [芯步控制台](https://console.thingboot.com) 注册即可接入。
- **量产阶段计费**：产品进入量产阶段后，由工厂支付平台接入费；开放平台 API 调用（可选，主要面向终端客户）按量计费。
- **购买与咨询**：联系 `license@thingboot.com` 或通过芯步控制台。

## 技术支持

- 技术支持：`support@thingboot.com`
- 销售授权：`license@thingboot.com`
- 官方文档：[docs.thingboot.com](https://www.thingboot.com/document/fb9d5f99e09c8673/)

---

**版权所有 © 2026 芯步（ThingBoot）科技有限公司。保留所有权利。**
