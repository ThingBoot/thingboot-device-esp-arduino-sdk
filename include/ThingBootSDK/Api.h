#ifndef THINGBOOT_SDK_API_H
#define THINGBOOT_SDK_API_H

class ThingBootDevice;

class ThingBootApi {
private:
    ThingBootDevice* _device;

public:
    ThingBootApi(ThingBootDevice* device = nullptr) : _device(device) {}

    // 通过平台发起 HTTP 请求；同一时间仅允许一个请求
    uint16_t request(String method, String url);
    uint16_t request(String method, String url, JSONVar headers, JSONVar body,
        const char* content_type = "application/json", uint32_t timeout_ms = 10000);

    // 平台应答或本地超时回调
    uint16_t onResponse(std::function<void(uint16_t code, uint16_t status, JSONVar data, const char* msg)> callback);

    bool pending();
};

#endif
