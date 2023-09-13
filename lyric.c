#include "lyric.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "console.h"

// 创建歌词结构体
Lyric_t *lrc_lyric_create(Lrc_time_t time, char *lyric)
{

    Lyric_t *new = (Lyric_t *)malloc(sizeof(Lyric_t));
    if (NULL == new)
    {
        printf("lyr_lyric_create malloc failed\n");
        goto err0;
    }
    // memset(new, 0, sizeof(Lyric_t));
    new->lyric = NULL;

    // memcpy(&new->time, time, sizeof(Lrc_time_t));
    new->time.min = time.min;
    new->time.sec = time.sec;
    new->time.total = time.total;
    new->lyric = (char *)malloc(60 * sizeof(char));
    strcpy(new->lyric, lyric);

    return new;
err0:
    return NULL;
}

// 歌词结构体自定义输出
int my_print(Lyric_t *lyric)
{
    if (NULL == lyric)
    {
        printf("my_print failed\n"
               "parameter is NULL\n");
        goto err0;
    }

    printf("%02d:%02d  %s\n",
           lyric->time.min,
           lyric->time.sec,
           lyric->lyric);

    return 0;
err0:
    return -1;
}

// 歌词结构体自定义销毁
int my_destroy(Lyric_t *lyric)
{
    if (NULL == lyric)
    {
        printf("my_destroy failed\n"
               "parameter is NULL\n");
        goto err0;
    }

    // printf("%02d:%02d %s\n", lyric->time.min, lyric->time.sec, lyric->lyric);
    free(lyric->lyric);
    // printf("%02d:%02d  %d\n", lyric->time.min, lyric->time.sec, lyric->time.total);
    free(lyric);
    // printf("%02d:%02d\n", lyric->time.min, lyric->time.sec);

    return 0;
err0:
    return -1;
}

// 歌词结构体自定义比较歌词函数
int my_cmp_by_lyric(Lyric_t *a, Lyric_t *b)
{
    int ret = strcmp(a->lyric, b->lyric);

    return ret;
}

// 歌词结构体自定义比较时间函数
int my_cmp_by_time(Lyric_t *a, Lyric_t *b)
{
    int ret = a->time.total - b->time.total;

    return ret;
}

// 加载歌词文件
char *lrc_read_file(const char *filename)
{
    if (NULL == filename)
    {
        printf("lrc_read_file failed\n"
               "parameter is NULL\n");
        goto err0;
    }

    int file_size = 0;
    char *text = NULL;
    int ret;

    FILE *rf = fopen(filename, "r");

    fseek(rf, 0, SEEK_END);
    file_size = ftell(rf);

    text = (char *)malloc((size_t)file_size + 4);
    if (NULL == text)
    {
        printf("lrc_read_file malloc failed\n");
        goto err1;
    }
    memset(text, 0, (size_t)file_size + 4);

    rewind(rf);
    ret = fread(text, file_size, 1, rf);
    if (ferror(rf))
    {
        printf("lrc_read_file fread failed\n");
        goto err2;
    }
    strcat(text, "end");

    fclose(rf);
    return text;
err2:
    free(text);
err1:
    fclose(rf);
err0:
    return NULL;
}

// 使用\r\n切割歌词 返回歌词的行数
int lrc_strtok_buf(char *lrc_text[], char *text)
{
    if (NULL == lrc_text || NULL == text)
    {
        printf("lrc_strtok_buf failed\n"
               "parameter is NULL\n");
        goto err0;
    }

    int line = 0;

    lrc_text[line] = strtok(text, "\r\n");
    // 忽略文件开头的空白行
    while ('[' != lrc_text[line][0])
        lrc_text[line] = strtok(NULL, "\r\n");
    for (line = 1; '[' == lrc_text[line - 1][0]; line++)
        lrc_text[line] = strtok(NULL, "\r\n");

    return line - 1;
err0:
    return -1;
}

// 解析lrc文件前4行 歌手 歌名 专辑 制作等等
//[ti:简单爱]
//[ar:周杰伦]
//[al:范特西]
//[by:大脸猫]
int lrc_parse_head(char *lrc_text[], char *song_msg[])
{
    if (NULL == lrc_text)
    {
        printf("lrc_parse_head failed\n"
               "parameter is NULL\n");
        goto err0;
    }

    char *pre[4] = {"[ti", "[ar", "[al", "[by"};
    char flag[4] = {1, 1, 1, 1};
    int line, i, head_count = 0;

    while (lrc_text[head_count][1] > '9' || lrc_text[head_count][1] < '0')
        head_count++;

    for (line = 0; line < head_count; line++)
    {
        if (!(flag[0] || flag[1] || flag[2] || flag[3]))
            break;
        for (i = 0; i < 4; i++)
        {
            if (0 == strncmp(lrc_text[line], pre[i], 3))
            {
                song_msg[i] = (char *)malloc(strlen(lrc_text[line]) - 4);
                if (NULL == song_msg[i])
                {
                    printf("lrc_parse_head %s malloc failed\n", pre[i]);
                    goto err1;
                }
                sscanf(lrc_text[line], "%*4s%[^]]", song_msg[i]);
                flag[i] = 0;
                break;
            }
        }
    }

    for (i = 0; i < 4; i++)
    {
        if (flag[i])
        {
            song_msg[i] = NULL;
        }
    }

    return head_count;
err1:
    for (i = 0; i < 4; i++)
    {

        if (0 == flag[i])
        {
            printf("%s\n", song_msg[i]);
            free(song_msg[i]);
        }
    }
err0:
    return -1;
}

// 释放song_head内存
int lrc_song_head_free(char *song_msg[])
{
    if (NULL == song_msg)
    {
        printf("lrc_song_head_free failed\n"
               "parameter is NULL\n");
        goto err0;
    }

    int i = 0;

    for (i = 0; i < 4; i++)
    {
        if (NULL != song_msg[i])
            free(song_msg[i]);
    }

    return 0;
err0:
    return -1;
}

// 显示歌曲信息 前4行
int lrc_song_head_print(char *song_msg[])
{
    if (NULL == song_msg)
    {
        printf("lrc_song_head_print failed\n"
               "parameter is NULL\n");
        goto err0;
    }

    char *type[4] = {"歌名", "歌手", "专辑", "制作"};
    int i;

    clear_screen();

    set_fg_color(COLOR_CYAN);
    for (i = 0; i < 4; i++)
    {
        cusor_moveto(30, 1 + i);
        if (NULL == song_msg[i])
            printf("%s : %s", type[i], "未知");
        else
            printf("%s : %s", type[i], song_msg[i]);
    }
    printf("\n");

    return 0;
err0:
    return -1;
}

// 指定字体颜色输出字符串
int lrc_color_print(char *str, int x, int y, int color)
{
    if (NULL == str)
    {
        printf("lrc_color_print failed\n"
               "str is NULL\n");
        goto err0;
    }

    if (x < 0 || y < 0)
    {
        printf("lrc_color_print failed\n"
               "parameter is illegal\n");
        goto err0;
    }

    cusor_moveto(x, y);
    set_fg_color(color);
    printf("%s\n", str);

    return 0;
err0:
    return -1;
}

// 显示歌词的时间
int lrc_time_print(Lrc_time_t *t)
{
    if (NULL == t)
    {
        printf("lrc_time_print failed\n"
               "parameter is NULL\n");
        goto err0;
    }

    cusor_moveto(32, 5);
    set_fg_color(COLOR_MAGENTA);
    printf("%02d : %02d\n", t->min, t->sec);

    return 0;
err0:
    return -1;
}

// 解析歌词信息 歌词和时间 然后插入到链表中
int lrc_lyric_parse(Dlist_t *head, int lrc_lines, int start_line, char *lrc_text[])
{
    if (NULL == head || NULL == lrc_text || lrc_lines <= 0)
    {
        printf("lrc_lyric_parse failed\n"
               "parameter is illegal");
        goto err0;
    }

    int line;
    int time_count;
    Lrc_time_t times[10];
    int min, sec;
    char *src;
    Lyric_t *new;

    for (line = start_line; line < lrc_lines; line++)
    {
        src = lrc_text[line];

        time_count = 0;
        while ('[' == *src)
        {
            sscanf(src, "[%2d:%2d", &min, &sec);
            times[time_count].min = min;
            times[time_count].sec = sec;
            times[time_count++].total = min * 60 + sec;
            // printf("times[%d].total = %d\n", time_count - 1, times[time_count - 1].total);
            src += 10;
        }
        time_count--;

        for (; time_count >= 0; time_count--)
        {
            new = lrc_lyric_create(times[time_count], src);
            dlist_append(head, (void *)new);
            // printf("create head->count = %d\n", head->count);
            free(new);
        }
    }

    return 0;
err0:
    return -1;
}

// 打印链表中5个节点的信息到屏幕上
int lrc_lyric_print(Dlist_t *head, Note_t *cur_note)
{
    int i;

    Note_t *start = cur_note->prev->prev;
    Lyric_t *cur = (Lyric_t *)cur_note->data;
    Lyric_t *lyric = NULL;

    for (i = 0; i < 5; i++)
    {
        if ((cur_note != head->note || i > 1) && (cur_note != head->note->next || i > 0) && (cur_note != head->note->prev || i < 4) && (cur_note != head->note->prev->prev || i < 3))
        {
            lyric = (Lyric_t *)start->data;
            if (start->data == cur)
                lrc_color_print(lyric->lyric, 20, 6 + i, COLOR_WHITE);
            else
                lrc_color_print(lyric->lyric, 20, 6 + i, COLOR_BLUE);
        }
        start = start->next;
    }
}

// 将秒数转化为分和秒 lrc_time_t类型
int lrc_add_time(Lrc_time_t *t)
{
    t->total++;
    t->sec++;
    if (60 == t->sec)
    {
        t->min++;
        t->sec = 0;
    }
}
