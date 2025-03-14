#include "process.h"
// 保持 service_data 为全局矩阵，最多支持 4 个服务
static String service_data[4][8]; // [服务键][current_up][current_down][状态][服务器名称][最后延时][total_up][total_down]
static bool initialized = false;  // 初始化标志
static int service_delay_array[4][30];  // 每个服务一个独立的延迟数组
void processJsonData(DynamicJsonDocument& doc) {
    // 1. 检查JSON有效性
    if (!doc.containsKey("data") || !doc["data"].containsKey("services")) {
        Serial.println("[错误] JSON结构无效");
        return;
    }
    // 2. 提取services对象
    JsonObject services = doc["data"]["services"];

    // 3. 计算服务数量（动态根据 TARGET_SERVICE_KEYS 数组计算）
    int service_count = TARGET_SERVICE_KEY_COUNT;

    // 4. 首次运行初始化（只执行一次）
    if (!initialized) {
        for (int i = 0; i < service_count; i++) {
            const char* key = TARGET_SERVICE_KEYS[i];
            service_data[i][0] = key; // 服务键
            service_data[i][1] = "0"; // 初始 current_up
            service_data[i][2] = "0"; // 初始 current_down
            service_data[i][3] = "0"; // 初始状态
            service_data[i][4] = "未知服务"; // 初始服务器名称
            service_data[i][5] = "0.0"; // 初始最后延时
            service_data[i][6] = "0"; // 检测成功的次数
            service_data[i][7] = "0"; // 检测失败的次数
        }
        initialized = true;
    }
    // 5. 遍历配置中的服务键
    for (int i = 0; i < service_count; i++) {
        const char* key = TARGET_SERVICE_KEYS[i];
        // 跳过不存在键
        if (!services.containsKey(key)) {
            Serial.print("[警告] 服务键 ");
            Serial.print(key);
            Serial.println(" 不存在");
            continue;
        }
        // 6. 提取服务数据
        JsonObject service_data_json = services[key];
        const char* service_name = service_data_json["service_name"] | "未知服务";
        int current_up = service_data_json["current_up"] | -1;
        int current_down = service_data_json["current_down"] | -1;
        int total_up=service_data_json["total_up"] | -1;
        int total_down=service_data_json["total_down"] | -1;
        // 更新服务器名称
        service_data[i][4] = service_name;
        service_data[i][6] = String(total_up);    // total_up 存储到索引6
        service_data[i][7] = String(total_down);  // total_down 存储到索引7
        // 7. 提取delay最后一个元素，并更新service_data[i][5]（最后延时）
        JsonArray delay;
        if (service_data_json.containsKey("delay")) {
            delay = service_data_json["delay"];
            if (delay.size() > 0) {
                float last_delay = delay[delay.size() - 1].as<float>();
                service_data[i][5] = String(last_delay, 2); // 将最后延迟值保存到 service_data[i][5]
            }

              // 填充 delay_array（0 或 1）
            int j = 0;
            Serial.print("Delay状态: ");
            for (float value : delay) {
                  if (j < 30) {  // 确保不超过30个元素
                      service_delay_array[i][j] = (value == 0.0f) ? 0 : 1;  // 如果值为0.0f，填入0，否则填入1
                      Serial.print(service_delay_array[i][j] == 0 ? "0 " : "1 ");
                      j++;
                  }
                }
                Serial.println();
        } else {
            Serial.println("Delay字段无效或缺失");
        }
        // 8. 保存旧值
        int previous_up = service_data[i][1].toInt();
        int previous_down = service_data[i][2].toInt();
        // 9. 更新数组中的当前值
        service_data[i][1] = String(current_up);
        service_data[i][2] = String(current_down);
        // 10. 状态判断逻辑（优先级修复）
        if (current_up > previous_up) {
            service_data[i][3] = "1"; // 上行增加 → 状态1
        } else if (current_down > previous_down) {
            service_data[i][3] = "0"; // 下行增加 → 状态0
        } else if (current_down == 0) {
            service_data[i][3] = "1"; // 下行为0 → 状态1
        } else if (current_up == 0) {
            service_data[i][3] = "0"; // 上行为0 → 状态0
        }
        // 11. 输出完整数据表（保留原有功能）
        Serial.println("\n[完整数据表]");
        Serial.println("==============================================");
        for (int i = 0; i < service_count; i++) {
            Serial.print("| 服务键: "); Serial.print(service_data[i][0]);
            Serial.print(" | 上行: "); Serial.print(service_data[i][1]);
            Serial.print(" | 下行: "); Serial.print(service_data[i][2]);
            Serial.print(" | 状态: "); Serial.print(service_data[i][3]);
            Serial.print(" | 服务器名称: "); Serial.print(service_data[i][4]);
            Serial.print(" | 最后延时: "); Serial.println(service_data[i][5]);
            Serial.print(" | up: "); Serial.println(service_data[i][6]);
            Serial.print(" | down: "); Serial.println(service_data[i][7]);
            Serial.println(" |");
        }
        Serial.println("==============================================");
    }
    // 12. 输出最终状态（保留原有功能）
    Serial.println("\n[最终状态报告]");
    for (int i = 0; i < service_count; i++) {
        Serial.print("服务 ");
        Serial.print(service_data[i][0]);
        Serial.print(" 状态: ");
        Serial.println(service_data[i][3] == "1" ? "正常 ↑" : "异常 ↓");
    }
    // 更新UI显示
    for (int i = 0; i < MAX_SERVICE_BLOCKS; i++) {
        if (i < service_count) {
            lv_obj_clear_flag(service_blocks[i].block, LV_OBJ_FLAG_HIDDEN);
            update_service_block_ui(i, service_data[i], service_delay_array[i]);
        } else {
            lv_obj_add_flag(service_blocks[i].block, LV_OBJ_FLAG_HIDDEN);
        }
    }
}
// process.cpp
void update_service_block_ui(int index, String* data, int* delay_array) {
    ServiceBlockUI* ui = &service_blocks[index];

    // 状态点颜色（0: 红，1: 蓝）
    lv_obj_set_style_bg_color(ui->status_dot, 
        (data[3] == "1") ? lv_color_hex(0x00FF00) : lv_color_hex(0xFF0000), 
        LV_PART_MAIN
    );
    // 服务名称
    lv_label_set_text(ui->name_label, data[4].c_str());
    // 1. 计算可靠性（需处理除零错误）
    float reliability = 0.0f;
    int total_up = data[6].toInt();
    int total_down = data[7].toInt();
    if (total_up > 0 && total_down >= 0) {
        reliability = (1.0f - (float)total_down / (total_up - 1)) * 100; // 根据公式计算
        reliability = constrain(reliability, 0.0f, 100.0f); // 限制在0-100%
    }
    // 2. 更新可靠性标签
    String reliability_text = (reliability >= 0) ? String(reliability, 1) + "%" : "N/A";
    lv_label_set_text(ui->reliability_label, reliability_text.c_str());
    Serial.print(total_up);
    // 设置可靠性标签颜色
    lv_color_t reliability_color = (reliability >= 98.0f) ? lv_color_hex(0x00FF00) : lv_color_hex(0xFFFF00);
    lv_obj_set_style_text_color(ui->reliability_label, reliability_color, LV_PART_MAIN);
    // 3. 更新延迟标签颜色
    float last_delay = data[5].toFloat();
    lv_color_t delay_color = (last_delay < 90.0f) ? lv_color_hex(0x00FF00) : lv_color_hex(0xFFFF00);
    lv_obj_set_style_text_color(ui->delay_label, delay_color, LV_PART_MAIN);
    // 延迟标签
    String delay_text = data[5] + "ms";
    lv_label_set_text(ui->delay_label, delay_text.c_str());
    // 输出delay_array中的值到串口调试
    Serial.print("当前 delay_array: ");
    for (int i = 0; i < 30; i++) {
        Serial.print(delay_array[i]);
        Serial.print(" ");
    }
    Serial.println();  // 换行
    // 更新延迟条颜色（直接修改预创建的对象）
    for (int i = 0; i < 30; i++) {
        lv_obj_set_style_bg_color(
            ui->delay_blocks[i], 
            (delay_array[i] == 0) ? lv_color_hex(0xFF0000) : lv_color_hex(0x00FF00), 
            LV_PART_MAIN
        );
    }
}