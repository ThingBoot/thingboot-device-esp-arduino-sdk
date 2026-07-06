#include "ThingBootSDK.h"

ThingBootDevice device;

//继电器驱动
#include "relay.h"

//调试信息打印回调
void onDebug(const char* category, const char* message)
{
    Serial.print("[");
    Serial.print(millis());
    Serial.print("] ");
    Serial.print(category);
    Serial.print(" | ");
    Serial.println(message);
}

//命令回调
void onOrder(const char* mid, JSONVar data)
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
	device.setDebugCallback(onDebug);
	
	//产品信息，请到芯步产品中心定义和查看
	device.setProduct(
		"wthkoqaFpw", //产品代号[Key]
		"19db80bfb2c5f300909f45658691dc0a", //产品密码[Secret]
		"demo", //适配PCB板，自由定义
		"esp8266",  //适配MCU
		"p199.20260705.v1.xxx"  //当前固件版本号，先在控制台登记新版本，末段xxx为子版本号（自由定义）
	);

	//设备配置
	device.Config.addConfig("relay", 0, 1);
	device.Config.addConfig("btn_action", 1, 3);

	//设置命令回调函数
	device.Order.setOrderCallback(onOrder);

	//外设初始化
	device.Peripheral.setup([]() {
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