#ifndef __LRC_TIME_H__
#define __LRC_TIME_H__

typedef struct _Lrc_time_t
{
    int min;
    int sec;
    int total;
} Lrc_time_t;

Lrc_time_t *lrc_time_create(int min, int sec);

#endif /*__LRC_TIME_H__*/