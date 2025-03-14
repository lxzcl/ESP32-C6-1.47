#ifndef CONFIG_H
#define CONFIG_H
extern const char* ssid;  // 声明 ssid 为外部链接
extern const char* password;  // 声明 password 为外部链接
extern const char* API;
extern const char* USERNAME;
extern const char* PASSWORD;
// 声明需要处理的固定服务键数组
extern const char* TARGET_SERVICE_KEYS[];
extern const int TARGET_SERVICE_KEY_COUNT;
#endif