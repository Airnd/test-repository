#ifndef __LYRICS_H__
#define __LYRICS_H__

#include "lrc_time.h"
#include "dlist.h"

// 单句歌词信息结构体
typedef struct _Lyric_data_t
{
    Lrc_time_t time;
    char *lyric;
} Lyric_t;

// 创建歌词结构体
Lyric_t *lrc_lyric_create(Lrc_time_t time, char *lyric);

// 歌词结构体自定义输出
int my_print(Lyric_t *lyric);

// 歌词结构体自定义销毁
int my_destroy(Lyric_t *lyric);

// 歌词结构体自定义比较歌词函数
int my_cmp_by_lyric(Lyric_t *a, Lyric_t *b);

// 歌词结构体自定义比较时间函数
int my_cmp_by_time(Lyric_t *a, Lyric_t *b);

// 加载歌词文件
char *lrc_read_file(const char *filename);

// 使用\r\n切割歌词 返回歌词的行数
int lrc_strtok_buf(char *lrc_text[], char *ptext);

// 解析lrc文件前4行 歌手 歌名 专辑 制作等等
int lrc_parse_head(char *lrc_text[], char *song_msg[]);

// 释放song_head内存
int lrc_song_head_free(char *song_msg[]);

// 显示歌曲信息 前4行
int lrc_song_head_print(char *song_msg[]);

// 指定字体颜色输出字符串
int lrc_color_print(char *str, int x, int y, int color);

// 显示歌词的时间
int lrc_time_print(Lrc_time_t *t);

// 解析歌词信息 歌词和时间 然后插入到链表中
int lrc_lyric_parse(Dlist_t *d, int lrc_lines, int start_line, char *lrc_text[]);

// 打印链表中5个节点的信息到屏幕上
int lrc_lyric_print(Dlist_t *head, Note_t *cur_note);

// 将秒数转化为分和秒 lrc_time_t类型
int lrc_add_time(Lrc_time_t *t);

#endif /*__LYRICS_H__*/