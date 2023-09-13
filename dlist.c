#include "dlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 头信息结构体的创建函数
Dlist_t *dlist_create(int size, op_t my_destory, op_t my_print)
{
    // 参数检查
    if (NULL == my_destory || NULL == my_destory || size <= 0)
    {
        printf("dlist_create failed\n"
               "parameter is wrong\n");
        goto err0;
    }

    // 分配内存
    Dlist_t *head = (Dlist_t *)malloc(sizeof(Dlist_t));
    if (NULL == head)
    {
        printf("dlist_create malloc failed\n");
        goto err0;
    }
    memset(head, 0, sizeof(Dlist_t));

    // 赋值
    head->count = 0;
    head->my_destroy = my_destory;
    head->my_print = my_print;
    head->size = size;
    head->note = NULL;

    return head;
err0:
    return NULL;
}

// 分配节点内存函数
Note_t *note_malloc(Dlist_t *head, void *data)
{
    // 参数检查
    if (NULL == head || NULL == data)
    {
        printf("note_malloc failed\n"
               "parameter is NULL\n");
        goto err0;
    }

    // 分配内存
    Note_t *note = (Note_t *)malloc(sizeof(Note_t));
    if (NULL == note)
    {
        printf("note_malloc malloc failed\n");
        goto err0;
    }
    memset(note, 0, sizeof(Note_t));

    note->data = malloc(head->size);
    if (NULL == note->data)
    {
        printf("note_malloc malloc failed\n");
        goto err0;
    }
    memset(note->data, 0, head->size);

    // 赋值
    // note->data = data;
    memcpy(note->data, data, head->size);
    note->next = note;
    note->prev = note;

    return note;
err0:
    return NULL;
}

// 头插
int dlist_prepend(Dlist_t *head, void *data)
{
    // 参数检查
    if (NULL == head || NULL == data)
    {
        printf("dlist_prepent failed\n"
               "parameter is NULL\n");
        goto err0;
    }

    // 创建新节点
    Note_t *new = note_malloc(head, data);
    if (NULL == new)
    {
        printf("dlist_prepend failed\n");
        goto err0;
    }

    // 插入操作
    if (0 == head->count)
    {
        head->note = new;
        new->next = new;
        new->prev = new;
    }
    else
    {
        new->prev = head->note->prev;
        new->next = head->note;
        head->note = new;
        new->prev->next = new;
        new->next->prev = new;
    }

    head->count++;
    return 0;
err0:
    return -1;
}

// 尾插
int dlist_append(Dlist_t *head, void *data)
{
    if (NULL == head || NULL == data)
    {
        printf("dlist_append failed\n"
               "parameter is NULL\n");
        goto err0;
    }

    Note_t *new = note_malloc(head, data);
    if (NULL == new)
    {
        printf("dlist_append failed\n");
        goto err0;
    }

    if (0 == head->count)
    {
        head->note = new;
        new->next = new;
        new->prev = new;
    }
    else
    {
        new->next = head->note;
        new->prev = head->note->prev;
        new->prev->next = new;
        new->next->prev = new;
    }

    head->count++;
    // printf("create head->count = %d\n", head->count); //
    return 0;
err0:
    return -1;
}

// 通过索引插入
int dlist_insert(Dlist_t *head, void *data, int index)
{
    // 参数检查
    if (NULL == head || NULL == data)
    {
        printf("dlist_insert failed\n"
               "parameter is NULL\n");
        goto err0;
    }

    // 索引检查
    if (index < 0 || index > head->count)
    {
        printf("dlist_insert failed\n"
               "index is illegal\n");
        goto err0;
    }

    if (0 == index)
    {
        dlist_prepend(head, data);
        goto ret;
    }
    else if (index == head->count)
    {
        dlist_append(head, data);
        goto ret;
    }

    // 创建新的节点结构体
    Note_t *new = note_malloc(head, data);
    if (NULL == new)
    {
        printf("dlist_insert failed\n");
        goto err0;
    }

    // 创建临时节点指针
    Note_t *tmp = head->note;

    // 插入操作
    if (index < head->count / 2)
    {
        for (int i = 1; i < index; i++)
            tmp = tmp->next;
    }
    else
    {
        for (int i = head->count; i >= index; i--)
            tmp = tmp->prev;
    }
    new->prev = tmp;
    new->next = tmp->next;
    tmp->next->prev = new;
    tmp->next = new;

    head->count++;
ret:
    return 0;
err0:
    return -1;
}

// 遍历输出函数
int dlist_traverse(Dlist_t *head)
{
    // 参数检查
    if (NULL == head)
    {
        printf("dlist_traverse failed\n"
               "parameter is NULL\n");
        goto err0;
    }

    // 确认输出函数可用
    if (NULL == head->my_print)
    {
        printf("dlist_traverse failed\n"
               "my_print is NULL\n");
        goto err0;
    }

    // 创建临时指针变量
    Note_t *tmp = head->note;
    do
    {
        head->my_print(tmp->data);
        tmp = tmp->next;
    } while (tmp != head->note);

    return 0;
err0:
    return -1;
}

// 销毁链表
int dlist_destroy(Dlist_t *head)
{
    if (NULL == head)
    {
        printf("dlist_destroy failed\n"
               "head is NULL\n");
        goto err0;
    }

    Note_t *tmp = head->note;
    Note_t *save = NULL;
    // printf("head->count = %d\n", head->count);

    while (head->count > 0)
    {
        // printf("head->count = %d\n", head->count); //
        save = tmp->next;
        head->my_destroy(tmp->data);
        free(tmp);
        // printf("111\n");
        tmp = save;
        head->count--;
    }
    free(head);
    head = NULL;

    return 0;
err0:
    return -1;
}

// 通过key检索链表节点数据
Note_t *dlist_find_by_key(Dlist_t *head, void *key, cmp_t my_cmp)
{
    if (NULL == head || NULL == key || NULL == my_cmp)
    {
        printf("dlist_find_by_key failed\n");
        goto err0;
    }

    Note_t *tmp = head->note;
    if (NULL == tmp)
    {
        printf("dlist_find_by_key failed\n"
               "dlist is empty\n");
        goto err0;
    }

    int i;
    for (i = 0; i < head->count; i++)
    {
        if (my_cmp(tmp->data, key))
            return tmp;
        tmp = tmp->next;
    }

err0:
    return NULL;
}

// 排序链表 选择排序
int dlist_select_sort(Dlist_t *head, cmp_t my_cmp)
{
    if (NULL == head || NULL == my_cmp)
    {
        printf("dlist_select_sort failed\n"
               "parameter is NULL\n");
        goto err0;
    }

    void *tmp = malloc(head->size);
    if (NULL == tmp)
    {
        printf("dlist_select_sort malloc failed\n");
        goto err0;
    }
    memset(tmp, 0, head->size);

    // int i, j;
    Note_t *min = NULL;
    Note_t *start = head->note;
    Note_t *save = NULL;
    Note_t *cur = NULL;
    /*
        for (i = 0; i < head->count; i++)
        {
            start = start->next;
            for (j = i; j < head->count; j++)
            {
                if (0 == my_cmp(min->data, cur->data))
            }
        }
    */
    for (start = head->note; start != head->note->prev; start = save)
    {
        save = start->next;
        min = start;
        for (cur = save; cur != head->note; cur = cur->next)
        {
            if (my_cmp(cur->data, min->data) < 0)
                min = cur;
        }
        memcpy(tmp, start->data, head->size);
        memcpy(start->data, min->data, head->size);
        memcpy(min->data, tmp, head->size);

        /*
            if (start == head->note)
                head->note = min;
            if (start == min)
                continue;
            else if (start->next == min)
            {
                start->next = min->next;
                min->next->prev = start;
                min->next = start;
                min->prev = start->prev;
                min->prev->next = min;
                start->prev = min;
            }
            else if (start->prev = min)
            {
                min->next = start->next;
                start->next->prev = min;
                start->next = min;
                start->prev = min->prev;
                start->prev->next = start;
                min->prev = start;
            }
            else
            {
                Note_t *sta_pre = start->prev;
                start->next = min->next;
                start->next->prev = start;
                min->next = save;
                min->next->prev = min;
                start->prev = min->prev;
                start->prev->next = start;
                min->prev = sta_pre;
                min->prev->next = min;
            }
        */
    }

    free(tmp);
    return 0;
err0:
    return -1;
}
