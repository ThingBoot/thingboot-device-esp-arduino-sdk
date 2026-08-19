#ifndef THINGBOOT_SDK_NETWORK_H
#define THINGBOOT_SDK_NETWORK_H

class ThingBootDevice;

class ThingBootNetwork {
private:
    ThingBootDevice* _device;

public:
    ThingBootNetwork(ThingBootDevice* device = nullptr) : _device(device) {}

    // 获取当前网络信息（JSON 字符串）
    // Get current network information as JSON string
    String getNetworkInfo();

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
    String getWiFiConfig();

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
};

// ---- 联网 addon 安装函数（链接对应 addon 后，在 device.setup() 前调用）----
// 返回 ERR_OK 成功；ERR_NETWORK_ABI_MISMATCH 表示 addon 与 base 版本不一致
uint16_t tb_addon_net_ether_install();
uint16_t tb_addon_net_gsm_install();

#endif
