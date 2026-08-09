#include "ThingBootSDK.h"

ThingBootDevice device;

//继电器驱动
#include "relay.h"

//调试信息打印回调
void debug(const char* category, const char* message)
{
    Serial.print("[");
    Serial.print(millis());
    Serial.print("] ");
    Serial.print(category);
    Serial.print(" | ");
    Serial.println(message);
}

//命令回调
void order(const char* mid, JSONVar data)
{
	JSONVar ret;

	if (data.hasOwnProperty("power"))
	{
		relay_state(1, CN(data["power"]) > 0);
		ret["power"] = relay_state(1) ? "1" : "0";
	}

	device.Message.publishMessage(TOPIC_ORDER, mid, C(ret));
}

void setup()
{
	Serial.begin(115200);
	
	//调试信息打印
	device.onDebug(debug);
	
	//产品信息，请到芯步产品中心定义和查看
	device.setProduct(
		"xxx", //产品代号[Key]
		"xxxx", //产品密码[Secret]
		"demo", //适配PCB板，自由定义
		"esp8266",  //适配MCU
		"xxx.xxxxx.xx.xxx"  //当前固件版本号，先在控制台登记新版本，末段xxx为子版本号（自由定义）
	);

	//在开发阶段，请打开下面的链接免费获取设备接入激活码
	//在量产阶段，烧录工具软件将自动写入激活码。所以量产后可注释下方代码（或者不理会）
	//please open the link to generate a active code for this device
	//https://www.thingboot.com/developer/center/access/
	device.setActiveCode("xxxxxxxxxxxxx");

	//设备配置
	device.Config.addConfig("relay", 0, 1);
	device.Config.addConfig("btn_action", 1, 3);

	//设置命令回调函数
	device.Order.onOrder(order);

	//外设初始化
	device.Peripheral.onInit([]() {
		relay_setup();
	});

	//设备按钮
	device.Peripheral.setDeviceBtn(1, 13, false, [](uint8_t num, BTN_OPER oper, uint16_t keep) {
		if (oper != BTN_UP) return;

		JSONVar state;

		if (device.Config.readConfig("btn_action", "rev") == "rev")
		{
			relay_state(1, !relay_state(1));

			state["power"] = relay_state(1);
		}

		device.Event.reportEvent("btn", state);
	});

	device.Peripheral.setSystemBtn(1);

	//设定LED指示灯
	device.Peripheral.setDeviceLed(1, 2, false);
	device.Peripheral.setSystemLed(1);

	//在setup()最后调用
    device.setup();
}

void loop()
{
	//主循环
	device.loop();
}