#ifndef THINGBOOT_SDK_NETWORK_H
#define THINGBOOT_SDK_NETWORK_H

class ThingBootDevice;

// ---- 联网 addon 安装函数（链接对应 addon 后，在 device.setup() 前调用）----
// 返回 ERR_OK 成功；ERR_NETWORK_ABI_MISMATCH 表示 addon 与 base 版本不一致。
// 也可经成员转发版 Network.installEthernet()/installGSM() 调用（等价）
uint16_t tb_addon_net_ether_install();
uint16_t tb_addon_net_gsm_install();
uint16_t tb_addon_net_ble_install();

class ThingBootNetwork {
private:
    ThingBootDevice* _device;

public:
    ThingBootNetwork(ThingBootDevice* device = nullptr) : _device(device) {}

    // addon 安装（自由函数 tb_addon_net_*_install() 的成员转发版，完全等价，
    // 供编辑器提示发现；未链接对应 addon 库时不要调用）
    uint16_t installEthernet() { return tb_addon_net_ether_install(); }
    uint16_t installGSM() { return tb_addon_net_gsm_install(); }
    uint16_t installBLE() { return tb_addon_net_ble_install(); }

    // 获取当前网络信息
    // Get current network information
    JSONVar getNetworkInfo();

    // 设置首次联网延迟并持久化到 ROM（秒，0=不延迟，最大 3600，超出按 3600 处理）
    // 每次启动最多等待一次，后续重连仍保持默认 10 秒间隔；等待期间不阻塞主循环
    void setNetworkDelay(uint32_t seconds);

    // 网络变化回调（主循环上下文触发）
    // event：NETWORK_EVENT_OFFLINE 网络断开 / NETWORK_EVENT_ONLINE 网络就绪 /
    //        NETWORK_EVENT_TYPE_CHANGED 联网类型切换
    // type：网络类型 "wifi"/"ether"/"gsm"；OFFLINE 时为断开前的网络类型
    // 未注册不触发
    // Network change callback (fired in the main loop context)
    void onNetworkChange(std::function<void(NETWORK_EVENT event, const char* type)> callback);

    // Mesh 子设备上行消息回调（主循环上下文，与原始 WiFi Mesh 的 message_recv_callback 语义一致）
    // 返回 true：SDK 继续 sys_publish 上行平台；返回 false：由产品自行处理（不再自动上行）
    // from 为直接 BLE 对端（直连场景即原始子设备；多跳来源不承诺识别）
    // 未注册回调时直接上行平台
    void onMeshChildMessage(std::function<bool(const char*, SYS_TOPIC, const char*, const char*, uint32_t)> callback);

    // Mesh 设备发现回调（主循环上下文）：发现新的候选 Mesh 设备时触发（已绑定/未绑定均可）
    // id 来自 Mesh 广播设备 ID；address 为 BLE 地址字符串；rssi 为可选辅助信息
    // SDK 不自动 addChild，是否上报 device_join、何时绑定由产品决定
    void onMeshDeviceFound(std::function<void(uint32_t, const char*, int)> callback);

    // 向子设备发送完整 Mesh 业务消息（gateway→child，SDK 负责帧校验/分包/ACK/重试与发送队列）
    // 返回：ERR_OK 已入队（不代表子设备执行成功）；ERR_SYSTEM_VALUE_NULL 参数错误或数据过长；
    //       ERR_NETWORK_DRIVER_MISSING BLE addon 未安装；ERR_MESH_NOT_READY Mesh 未启用或暂无可用邻居；
    //       ERR_MESH_QUEUE_FULL 发送队列满
    uint16_t sendMeshChild(uint32_t id, SYS_TOPIC topic, const char* mid, const char* data);

    // 连接 WiFi
    // Connect to WiFi
    void connectWiFi();
    bool connectWiFi(const char* ssid, const char* psk);
    void disconnectWiFi();
    void clearWiFiConfig();
    bool addWiFiConfig(const char* ssid, const char* psk);
    bool addWiFiConfig(const char* ssid, const char* psk, uint8_t pos);
    bool updateWiFiConfig(const char* ssid, const char* psk);
    bool deleteWiFiConfig(const char* ssid);
    bool deleteWiFiConfig(uint8_t pos);
    JSONVar getWiFiConfig();

    // ---- 以太网（需链接 addon_net_ether 并调用 tb_addon_net_ether_install()；
    // 未注册驱动时方法为空操作/返回空串/false）----
    // 设置以太网 SPI 片选引脚和复位引脚
    // Set Ethernet SPI CS and reset pins
    void setEthernetPin(uint8_t cs, int8_t rst = -1);

    // 尝试连接以太网
    // Try to connect Ethernet
    String tryConnectEthernet();

    // 连接以太网
    // Connect Ethernet
    bool connectEthernet();

    // 断开以太网
    // Disconnect Ethernet
    void disconnectEthernet();

    // ---- GSM/4G（需链接 addon_net_gsm 并调用 tb_addon_net_gsm_install()；
    // 未注册驱动时方法为空操作/返回空串/false）----
    // 尝试连接 GSM
    // Try to connect GSM
    String tryConnectGSM();

    // 连接 GSM
    // Connect GSM
    bool connectGSM();

    // 断开 GSM
    // Disconnect GSM
    void disconnectGSM();

    // 设置 GSM 串口，支持软串口或硬串口
    // 开发者需先对串口执行 begin()，例如：
    //   Serial1.begin(115200);
    //   device.Network.setGSMSerialPort(&Serial1, 115200);
    // baud 为 0 时自动检测；大于 0 时使用固定波特率。
    // Set the GSM serial port (SoftwareSerial or HardwareSerial).
    // The caller must call begin() on the serial port before using GSM.
    void setGSMSerialPort(Stream* serial, uint32_t baud = 0);

    // 设置 GSM 使能引脚（低电平有效，-1 表示不控制模组电源，需在 setup 前设置）
    // Set the GSM enable pin (active low; -1 disables power control)
    void setGSMEnablePin(int8_t pin);
};

#endif
