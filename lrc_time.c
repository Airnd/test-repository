#include "lrc_time.h"
#include <stdio.h>
#include <stdlib.h>

Lrc_time_t *lrc_time_create(int min, int sec)
{
    Lrc_time_t *new = (Lrc_time_t *)malloc(sizeof(Lrc_time_t));
    if (NULL == new)
    {
        printf("lrc_time_create malloc failed\n");
        goto err0;
    }
    new->min = min;
    new->sec = sec;
    new->total = min * 60 + sec;

    return new;
err0:
    return NULL;
}