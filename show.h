#ifndef SHOW_H
#define SHOW_H
#include "LVGL_Driver.h"
#include "config.h"
#define EXAMPLE1_LVGL_TICK_PERIOD_MS  1000
void mast(const char* message);  // 声明mast函数
void page1();
void showWiFiStatus(int a);
void showState(bool a);
// 定义服务块的最大数量
#define MAX_SERVICE_BLOCKS 4
// 服务块UI组件结构体
typedef struct {
    lv_obj_t* separator_label;  // 新增：分隔线标签 "|"
    lv_obj_t* block;
    lv_obj_t* status_dot;
    lv_obj_t* name_label;
    lv_obj_t* delay_label;
    lv_obj_t* reliability_label; 
    lv_obj_t* delay_bars;
    lv_obj_t* delay_blocks[30]; // 新增：存储30个延迟条的指针
} ServiceBlockUI;
extern ServiceBlockUI service_blocks[MAX_SERVICE_BLOCKS]; // 全局UI对象
void init_service_block_ui(ServiceBlockUI* ui); // 新增声明
#endif