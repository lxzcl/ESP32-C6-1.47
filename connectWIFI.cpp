#include "connectWIFI.h"
void connectToWiFi(const char* ssid, const char* password) {
    static unsigned long startTime = 0;
    static bool isConnecting = false;
    static int retryCount = 0; // 重试次数
    if (!isConnecting) {
        mast("Connecting...");  // 初始显示连接中状态
        WiFi.begin(ssid, password);
        startTime = millis();
        isConnecting = true;
        retryCount++;
    } else {
        if (WiFi.status() == WL_CONNECTED) {
            isConnecting = false;
            retryCount = 0; // 重置重试次数
        } else if (millis() - startTime > 15000) {
            mast("Connection failed!");
            isConnecting = false;

            if (retryCount >= 3) {
                Serial.println("Too many retries. Restarting...");
                ESP.restart(); // 重启设备
            }
        }
    }
}