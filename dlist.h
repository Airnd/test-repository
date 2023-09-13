#ifndef __DLIST_H__
#define __DLIST_H__

typedef int (*op_t)(void *data);
typedef int (*cmp_t)(void *a, void *b);

// 节点类型
typedef struct _Note_t
{
    // 数据域
    void *data;
    // 指向上一个节点
    struct _Note_t *prev;
    // 指向下一个节点
    struct _Note_t *next;
} Note_t;

// 头信息结构体的创建
typedef struct _Dlist_t
{
    // 指向链表的第一个节点
    Note_t *note;
    // 链表中的节点个数
    int count;
    // 链表数据域指向类型的大小
    int size;
    // 自定义的数据域销毁函数
    op_t my_destroy;
    // 自定义的数据域输出函数
    op_t my_print;
} Dlist_t;

// 头信息结构体的创建函数
Dlist_t *dlist_create(int size, op_t my_destory, op_t my_print);

// 分配节点内存函数
Note_t *note_malloc(Dlist_t *head, void *data);

// 头插
int dlist_prepend(Dlist_t *head, void *data);

// 尾插
int dlist_append(Dlist_t *head, void *data);

// 通过索引插入
int dlist_insert(Dlist_t *head, void *data, int index);

// 遍历输出函数
int dlist_traverse(Dlist_t *head);

// 销毁链表
int dlist_destroy(Dlist_t *head);

// 通过key检索链表节点数据
Note_t *dlist_find_by_key(Dlist_t *d, void *key, cmp_t my_cmp);

// 排序链表 选择排序
int dlist_select_sort(Dlist_t *head, cmp_t my_cmp);

#endif /*__DLIST_H__*/