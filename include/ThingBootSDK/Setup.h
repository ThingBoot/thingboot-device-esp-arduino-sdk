#ifndef THINGBOOT_SDK_SETUP_H
#define THINGBOOT_SDK_SETUP_H

class ThingBootDevice;

class ThingBootSetup {
private:
    ThingBootDevice* _device;

public:
    ThingBootSetup(ThingBootDevice* device = nullptr) : _device(device) {}

    // 设备预初始化回调
    // Device pre-initialization callback
    uint16_t setDeviceSetupPreCallback(std::function<void()> setup_pre_callback);

    // 设备初始化回调
    // Device initialization callback
    uint16_t setDeviceSetupCallback(std::function<void()> setup_callback);

    // 设备状态查询回调
    // Device state query callback
    uint16_t setDeviceStateCallback(std::function<String()> device_state_callback);
};

#endif
