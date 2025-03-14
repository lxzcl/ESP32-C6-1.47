#include "show.h"
#include "connectWIFI.h"
#include "config.h"
#include "GetJson.h"
#include "process.h"
DynamicJsonDocument doc(2048);
void setup() {
    Serial.begin(9600);
    LCD_Init();
    Lvgl_Init();
    Set_Backlight(8);
    page1();                // 初始化界面
    connectToWiFi(ssid, password); // 启动非阻塞连接
}

void loop() {
    static uint32_t lastUpdate = 0;    // 记录 LVGL 上次更新时间
    static uint32_t lastJsonUpdate = 0; // 记录 JSON 上次更新时间
    static uint32_t lastWifiState = 0; // 记录 WIFI 状态上次更新时间
    static bool jsonFetchSuccess = false; // 记录 JSON 获取是否成功
    // 每5ms处理一次LVGL任务（非阻塞）
    if (millis() - lastUpdate >= 5) {
        lv_timer_handler();
        lastUpdate = millis();
    }
    // 每1秒检查一次WiFi状态
    if (millis() - lastWifiState >= 1500) {
        lastWifiState = millis(); // 更新记录时间

        if (WiFi.status() != WL_CONNECTED) {
            showWiFiStatus(3); // 显示红色（未连接）
            mast("Connecting...");
            connectToWiFi(ssid, password); // 非阻塞连接
        } else {
            showWiFiStatus(2); // 显示绿色（已连接）
            mast(WiFi.localIP().toString().c_str());
        }
    }
    // 每1000ms尝试获取一次JSON数据（非阻塞）
    if (millis() - lastJsonUpdate >= 3000) {
      lastJsonUpdate = millis(); // 更新记录时间
        showState(true); 
        bool success = fetchJsonFromURL(doc);
        if (success) {
               // 可以调用json预处理模块
               processJsonData(doc); // 传入解析后的JSON数据
               //serializeJsonPretty(doc, Serial);
               //Serial.println("抓取");
        } else {
            //抓取失败不更新数据
            //或者尝试一定次数显示归零
        }
    }else{
      showState(false); 
    }
}