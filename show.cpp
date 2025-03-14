#include "show.h"
static lv_obj_t* page = nullptr;
static lv_obj_t* pixel = nullptr;
static lv_obj_t* label = nullptr;
static lv_obj_t* State = nullptr;
ServiceBlockUI service_blocks[MAX_SERVICE_BLOCKS]; // 全局定义
void page1() {
    if (page == nullptr) {
        // 创建页面
        page = lv_obj_create(lv_scr_act());
        lv_obj_set_size(page, LV_HOR_RES, LV_VER_RES);
        lv_obj_set_style_bg_color(page, lv_color_hex(0x121212), LV_PART_MAIN);
        lv_obj_set_style_border_width(page, 0, LV_PART_MAIN);
        lv_obj_set_style_outline_width(page, 0, LV_PART_MAIN);
        lv_obj_set_scrollbar_mode(page, LV_SCROLLBAR_MODE_OFF);

        // IP地址标签
        label = lv_label_create(page);
        lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_12, LV_PART_MAIN);  // 使用12号字体
        //lv_obj_set_style_transform_zoom(label, 666, LV_PART_MAIN);  // 缩放为8像素（12 * 0.666 ≈ 8）
        lv_obj_align(label, LV_ALIGN_BOTTOM_LEFT, 13, 13);  // 对齐到左下角，并设置偏移量

        // WiFi状态指示块
        pixel = lv_obj_create(page);
        lv_obj_set_size(pixel, 5, 5);
        lv_obj_align(pixel, LV_ALIGN_BOTTOM_RIGHT, -10, 13);
        lv_obj_set_style_border_width(pixel, 0, LV_PART_MAIN);
        lv_obj_set_style_outline_width(pixel, 0, LV_PART_MAIN);
        lv_obj_set_style_radius(pixel, 0, LV_PART_MAIN);

        // 抓取数据状态指示块
        State = lv_obj_create(page);
        lv_obj_set_size(State, 5, 5);
        lv_obj_set_style_border_width(State, 0, LV_PART_MAIN);
        lv_obj_set_style_outline_width(State, 0, LV_PART_MAIN);
        lv_obj_set_style_radius(State, 0, LV_PART_MAIN);
        lv_obj_align_to(State, pixel, LV_ALIGN_OUT_LEFT_MID, 0, 0); // 参考对象为 pixel
        lv_obj_set_style_bg_color(State, lv_color_hex(0x808080), LV_PART_MAIN);

                // 创建4个服务块（2x2布局）
        const int block_width = 150;
        const int block_height = 68;
        const int margin = 10; // 块间距

        // 块1（左上角）
        service_blocks[0].block = lv_obj_create(page);
        lv_obj_set_size(service_blocks[0].block, block_width, block_height);
        lv_obj_align(service_blocks[0].block, LV_ALIGN_TOP_LEFT, margin-18, margin-14);
        init_service_block_ui(&service_blocks[0]);

        // 块2（右上角）
        service_blocks[1].block = lv_obj_create(page);
        lv_obj_set_size(service_blocks[1].block, block_width, block_height);
        lv_obj_align_to(service_blocks[1].block, service_blocks[0].block, LV_ALIGN_OUT_RIGHT_MID, margin, 0);
        init_service_block_ui(&service_blocks[1]);

        // 块3（左下角）
        service_blocks[2].block = lv_obj_create(page);
        lv_obj_set_size(service_blocks[2].block, block_width, block_height);
        lv_obj_align_to(service_blocks[2].block, service_blocks[0].block, LV_ALIGN_OUT_BOTTOM_MID, 0, margin);
        init_service_block_ui(&service_blocks[2]);

        // 块4（右下角）
        service_blocks[3].block = lv_obj_create(page);
        lv_obj_set_size(service_blocks[3].block, block_width, block_height);
        lv_obj_align_to(service_blocks[3].block, service_blocks[1].block, LV_ALIGN_OUT_BOTTOM_MID, 0, margin);
        init_service_block_ui(&service_blocks[3]);
      }
}

void mast(const char* message) {
    if (label != nullptr) {
        lv_label_set_text(label, message);
    }
}

void showWiFiStatus(int a) {
    switch (a) {
        case 1:
            lv_obj_set_style_bg_color(pixel, lv_color_hex(0xFFA500), LV_PART_MAIN); // 橙色（连接中）
            break;
        case 2:
            lv_obj_set_style_bg_color(pixel, lv_color_hex(0x00FF00), LV_PART_MAIN); // 绿色
            break;
        case 3:
            lv_obj_set_style_bg_color(pixel, lv_color_hex(0xFF0000), LV_PART_MAIN); // 红色
            break;
        default:
            lv_obj_set_style_bg_color(pixel, lv_color_hex(0x808080), LV_PART_MAIN); // 灰色（空闲）
    }
}

void showState(bool a) {
  if(a == true){
    lv_obj_set_style_bg_color(State, lv_color_hex(0xFFA500), LV_PART_MAIN);
  }else{
    lv_obj_set_style_bg_color(State, lv_color_hex(0x808080), LV_PART_MAIN);
  }
}

void init_service_block_ui(ServiceBlockUI* ui) {
    // 主块样式
    lv_obj_set_style_radius(ui->block, 10, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui->block, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_border_width(ui->block, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(ui->block, lv_color_hex(0x404040), LV_PART_MAIN);
    lv_obj_clear_flag(ui->block, LV_OBJ_FLAG_SCROLLABLE);

    // 状态点（左上角）
    ui->status_dot = lv_obj_create(ui->block);
    lv_obj_set_size(ui->status_dot, 8, 8);
    lv_obj_set_style_radius(ui->status_dot, LV_RADIUS_CIRCLE, LV_PART_MAIN);
    lv_obj_align(ui->status_dot, LV_ALIGN_TOP_LEFT, -3, 0);
    lv_obj_set_style_border_width(ui->status_dot, 0, LV_PART_MAIN); // 无边框
    lv_obj_set_style_outline_width(ui->status_dot, 0, LV_PART_MAIN);// 无轮廓

    // 服务名称标签
    ui->name_label = lv_label_create(ui->block);
    lv_obj_set_style_text_color(ui->name_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(ui->name_label, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_align_to(ui->name_label, ui->status_dot, LV_ALIGN_OUT_RIGHT_MID, 5, -1);

    // 延迟标签（对齐到右上角）
    ui->delay_label = lv_label_create(ui->block);
    lv_obj_set_style_text_color(ui->delay_label, lv_color_hex(0x808080), LV_PART_MAIN);
    lv_obj_set_style_text_font(ui->delay_label, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_align(ui->delay_label, LV_ALIGN_TOP_RIGHT, -60, 15); // 初始右对齐偏移20像素

    // 分隔线标签 "|"
    ui->separator_label = lv_label_create(ui->block);
    lv_label_set_text(ui->separator_label, "|");
    lv_obj_set_style_text_color(ui->separator_label, lv_color_hex(0x404040), LV_PART_MAIN);
    lv_obj_set_style_text_font(ui->separator_label, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_align_to(ui->separator_label, ui->delay_label, LV_ALIGN_OUT_RIGHT_MID, 5, 0); // 在延时标签右侧5像素

    // 可靠性标签（对齐到分隔线右侧）
    ui->reliability_label = lv_label_create(ui->block);
    lv_obj_set_style_text_color(ui->reliability_label, lv_color_hex(0x808080), LV_PART_MAIN);
    lv_obj_set_style_text_font(ui->reliability_label, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_align_to(ui->reliability_label, ui->separator_label, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

    // 延迟条容器（底部）
    const int block_width = 145;
    ui->delay_bars = lv_obj_create(ui->block);
    lv_obj_set_size(ui->delay_bars, block_width - 10, 10);
    lv_obj_align(ui->delay_bars, LV_ALIGN_BOTTOM_MID, -5, 5);
    lv_obj_set_flex_flow(ui->delay_bars, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui->delay_bars, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui->delay_bars, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_border_width(ui->delay_bars, 0, LV_PART_MAIN);
    lv_obj_set_style_outline_width(ui->delay_bars, 0, LV_PART_MAIN);
    lv_obj_clear_flag(ui->delay_bars, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_column(ui->delay_bars, 1, LV_PART_MAIN); // 设置列间距为1像素

    // 预创建30个延迟条，并保存到 delay_blocks 数组
    for (int i = 0; i < 30; i++) {
        ui->delay_blocks[i] = lv_obj_create(ui->delay_bars);
        lv_obj_set_size(ui->delay_blocks[i], 3, 8); //必须设置尺寸
        lv_obj_set_style_bg_color(ui->delay_blocks[i], lv_color_hex(0xFF0000), LV_PART_MAIN);
        lv_obj_set_style_border_width(ui->delay_blocks[i], 0, LV_PART_MAIN); // 移除边框
        lv_obj_set_style_outline_width(ui->delay_blocks[i], 0, LV_PART_MAIN); // 移除轮廓
    }

    


}