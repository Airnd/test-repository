#ifndef __CONSOLE_H__
#define __CONSOLE_H__

/*
COLOR_RED              红
COLOR_BLACK            黑
COLOR_GREEN            绿
COLOR_BLUE             蓝
COLOR_YELLOW           黄
COLOR_WHITE            白
COLOR_CYAN             青
COLOR_MAGENTA          洋红
*/
#define COLOR_RED 31
#define COLOR_BLACK 30
#define COLOR_GREEN 32
#define COLOR_BLUE 34
#define COLOR_YELLOW 33
#define COLOR_WHITE 37
#define COLOR_CYAN 36
#define COLOR_MAGENTA 35

void cusor_moveto(int x, int y); // 光标跳转到 y行 x列
void cusor_save_pos(void);       // 保存光标位置
void cusor_recover_pos(void);    // 恢复光标位置
void cusor_hide(void);           // 隐藏光标
void cusor_show(void);           // 显示光标
void clear_screen(void);         // 清屏
void set_fg_color(int color);    // 设置字体前景色
void set_bg_color(int color);    // 设置字体背景色

#endif /*__CONSOLE_H__*/