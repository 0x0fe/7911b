#ifndef __LCD_TE_DRIVER_H__
#define __LCD_TE_DRIVER_H__

enum data_mode {
    camera,   //lcd只有摄像头数据模式
    ui,       //lcd只有ui显示模式
    ui_camera,//lcd显示为ui和摄像头合成数据模式
    ui_user,  //lcd显示为ui和用户只定义数据合成显示模式
};

extern u8 get_lcd_show_data_mode(void);
extern void set_lcd_show_data_mode(u8 choice_data_mode);
extern void te_send_data(u8 *data_buf, u32 data_size);
extern void get_lcd_ui_x_y(u16 xstart, u16 xend, u16 ystart, u16 yend);
extern void ui_send_data_ready(u8 *data_buf, u32 data_size);
extern void camera_send_data_ready(u8 *data_buf, u32 data_size);

#endif
