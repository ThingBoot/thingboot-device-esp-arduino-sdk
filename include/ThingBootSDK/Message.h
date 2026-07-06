#ifndef THINGBOOT_SDK_MESSAGE_H
#define THINGBOOT_SDK_MESSAGE_H

class ThingBootDevice;

class ThingBootMessage {
private:
    ThingBootDevice* _device;

public:
    ThingBootMessage(ThingBootDevice* device = nullptr) : _device(device) {}

    // 设置消息接收回调
    // Set message receive callback
    uint16_t setDeviceMessageReceiveCallback(
        std::function<bool(const char* type, SYS_TOPIC topic, const char* mid, const char* data, uint32_t from)> message_recv_callback);

    // 向平台发布消息
    // Publish message to platform
    bool publishMessage(SYS_TOPIC topic, const char* mid, String data, uint32_t device = 0);
    bool publishMessage(SYS_TOPIC topic, String data, uint32_t device);
    bool publishMessage(SYS_TOPIC topic, String data);
};

#endif
