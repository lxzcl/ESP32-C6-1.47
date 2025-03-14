#include "GetJson.h"
bool authenticate(HTTPClient& http, String& token) {
    bool success = false;
    http.begin((String(API) + "/api/v1/login").c_str());
    http.addHeader("Content-Type", "application/json");
    
    // 构建登录JSON
    DynamicJsonDocument loginDoc(200);
    loginDoc["username"] = USERNAME;
    loginDoc["password"] = PASSWORD;
    String payload;
    serializeJson(loginDoc, payload);

    int httpCode = http.POST(payload);
    if (httpCode == HTTP_CODE_OK) {
        String response = http.getString();
        DynamicJsonDocument respDoc(1024);
        DeserializationError error = deserializeJson(respDoc, response);
        if (!error && respDoc["success"]) {
            token = respDoc["data"]["token"].as<String>();
            success = true;
        }
    } else {
        Serial.print("[HTTP] 登录失败，错误码: ");
        Serial.println(httpCode);
    }
    http.end();
    return success;
}

bool fetchJsonFromURL(DynamicJsonDocument& doc) {
    HTTPClient http;
    bool success = false;
    String token;

    // 1. 认证获取Token
    if (!authenticate(http, token)) {
        Serial.println("认证失败");
        return false;
    }

    // 2. 发起带Token的HTTP GET请求
    http.begin((String(API) + "/api/v1/service").c_str());
    //http.begin((String(API) + "/api/v1/server").c_str());
    http.addHeader("Authorization", "Bearer " + token);

    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        DeserializationError error = deserializeJson(doc, payload);
        if (!error) {
            success = true;
        } else {
            Serial.print("[JSON] 解析失败: ");
            Serial.println(error.c_str());
        }
    } else {
        Serial.print("[HTTP] 请求失败，错误码: ");
        Serial.println(httpCode);
    }
    http.end();
    return success;
}