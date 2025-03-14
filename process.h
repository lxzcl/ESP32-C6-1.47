#ifndef PROCESS_H
#define PROCESS_H
#include <ArduinoJson.h> // 添加 ArduinoJson 头文件
#include "show.h"
#include "LVGL_Driver.h"
#include "config.h"
void processJsonData(DynamicJsonDocument& doc);
void update_service_block_ui(int index, String* data, int* delay_array);
#endif