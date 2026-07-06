#ifndef THINGBOOT_SDK_STATE_H
#define THINGBOOT_SDK_STATE_H

class ThingBootDevice;

class ThingBootState {
private:
    ThingBootDevice* _device;

public:
    ThingBootState(ThingBootDevice* device = nullptr) : _device(device) {}

    // 注册定时状态上报回调
    // interval：上报间隔，单位秒
    // Register periodic state report callback
    // interval: reporting interval in seconds
    uint16_t setStateCallback(const char* name, uint32_t interval,
        std::function<void(const char* name)> callback);

    // 立即上报某个状态
    // Report state immediately
    bool reportState(const char* name, const char* data);
};

#endif
