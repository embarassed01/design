#ifndef MASKPROCESSOR_H
#define MASKPROCESSOR_H

#include "my_qt_common.h"

typedef struct
{
    char cs_buf[256];
    char cs_uniq[256];
    int  cs_len;
    int  cs_pos;
    int  buf_pos;
} cs_t;

typedef struct
{
    char data[256];
} sys_t;

typedef struct
{
    cs_t  cs[256];
    sys_t sys;
    int   min;
    int   max;
} mp_data_t;

mp_data_t * maskprocessor_init(int min, int max);

int maskprocessor_next(mp_data_t * data, char * word, int len);

void maskprocessor_delete(mp_data_t ** data);

void maskprocessor_use_test(int min, int max);


#endif // MASKPROCESSOR_H
