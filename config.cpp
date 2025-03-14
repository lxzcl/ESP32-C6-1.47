#include "config.h"

//const char* ssid = "HZH_office";  // 定义 ssid
//const char* password = "hzh@211229#";  // 定义 password
const char* ssid = "xxx";  // 定义wifi ssid
const char* password = "password";  // 定义wifi password
const char* API = "https://www.xxx.com";//面板地址
const char* USERNAME="user";//面板用户名
const char* PASSWORD="password";//面板密码
const char* TARGET_SERVICE_KEYS[] = {"1", "4", "5"};//想要展示的那个services？https://www.xxx.com/api/v1/service查看
const int TARGET_SERVICE_KEY_COUNT = sizeof(TARGET_SERVICE_KEYS) / sizeof(TARGET_SERVICE_KEYS[0]);//我写Java的,C++挺菜,的这句应该可以去掉的？