 #ifndef JSON_FETCHER_H
#define JSON_FETCHER_H
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"
// 声明JSON数据获取函数
bool fetchJsonFromURL(DynamicJsonDocument& doc);
bool authenticate(HTTPClient& http, String& token);
#endif