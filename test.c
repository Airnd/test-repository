#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "console.h"
#include "lyric.h"
#include "dlist.h"

int main(void)
{
    int i;

    // 存储歌词的链表
    Dlist_t *head = dlist_create(sizeof(Lyric_t), (int (*)(void *))my_destroy, (int (*)(void *))my_print);

    // 存储歌词的行数
    int lines = 0;

    // 存储歌词每一行
    char *lrc_line_text[200] = {NULL};

    // 保存歌词
    char *ptext = NULL;

    // 保存头部信息
    char *song_msg[4] = {NULL};

    Lrc_time_t *t = lrc_time_create(0, 0);

    // 加载歌词文件
    ptext = lrc_read_file("./发如雪.lrc");

    // 输出歌词文本
    // printf("%s\n", ptext);

    // 切割歌词 \r\n分隔
    lines = lrc_strtok_buf(lrc_line_text, ptext);

    printf("lines: %d\n", lines);

// 加载及切割测试
#if 0
    for (i = 0; i < lines; i++)
    {
        printf("%d  %s\n", i + 1, lrc_line_text[i]);
    }
    printf("lines = %d\n", lines);
#endif

    // memset(song_msg, 0, sizeof(song_msg));
    int start_line = lrc_parse_head(lrc_line_text, song_msg);
    printf("start_line = %d\n", start_line);

    // 将歌词加载进链表
    lrc_lyric_parse(head, lines, start_line, lrc_line_text);
    // dlist_traverse(head);

    // 将链表按时间顺序排序
    dlist_select_sort(head, (int (*)(void *, void *))my_cmp_by_time);
    // dlist_traverse(head);
    printf("head->cound = %d\n", head->count);

    // 播放测试
#if 1

    // lrc_color_print(lrc_line_text[5], 15, 6, COLOR_WHITE);

    Note_t *cur_note = head->note;
    Lyric_t *cur = (Lyric_t *)cur_note->data;
    Lyric_t *next = (Lyric_t *)cur_note->next->data;

    i = 0;
    lrc_song_head_print(song_msg);
    lrc_time_print(t);
    lrc_lyric_print(head, cur_note);
    while (i < head->count - 1)
    {
        if (t->total == next->time.total)
        {
            cur_note = cur_note->next;
            cur = (Lyric_t *)cur_note->data;
            next = (Lyric_t *)cur_note->next->data;
            clear_screen();
            lrc_song_head_print(song_msg);
            lrc_time_print(t);
            lrc_lyric_print(head, cur_note);
            i++;
        }
        sleep(1);
        lrc_add_time(t);
        lrc_time_print(t);
    }
    clear_screen();
    printf("歌词播放结束\n");

#endif

    dlist_destroy(head);

    // 释放内存 song_msg[i]
    lrc_song_head_free(song_msg);

    // 释放内存 t
    free(t);

    // 释放内存
    if (NULL != ptext)
    {
        free(ptext);
        ptext = NULL;
    }

    // printf("sizeof(Lyric_t) = %lu\n", sizeof(Lyric_t));
    // printf("sizeof(Lrc_time_t) = %lu\n", sizeof(Lrc_time_t));

    return 0;
}