#include "maskprocessor.h"
#include <QDebug>

void add_cs_buf(const char * inbuf, cs_t * css, const int css_cnt)
{
    size_t inbuf_len = strlen(inbuf);   // but for fullchar: inbuf_len=256;
    int pos = 0;
    for (int i = 0; i < inbuf_len; i++)
    {
        int u = inbuf[i];
        if (css[css_cnt].cs_uniq[u] == 1)
        {
            continue;
        }
        css[css_cnt].cs_uniq[u] = 1;
        css[css_cnt].cs_buf[pos] = inbuf[i];
        pos++;
    }
    css[css_cnt].cs_len = pos;
    css[css_cnt].cs_pos = 0;
    css[css_cnt].buf_pos = css_cnt;
}

mp_data_t * maskprocessor_init(int min, int max)
{
    mp_data_t * data = (mp_data_t*)malloc( sizeof(mp_data_t) );
    memset((void*)data, 0x00, sizeof(mp_data_t));
    data->max = max;
    data->min = min;
    //
    int pos;
    int chr;
    // for digit:     '0'  -- '9'
    // for big alpha: 'A'  -- 'Z'
    // for small alpha: 'a'-- 'z'                                 for (pos = 0, chr = 'a'; chr <= 'z'; chr++)
    // for symbol:    seechar - digit - big_alpha - small_alpha
    // for fullchar:  0x00 -- 0xff
    // for seechar:   0x20 -- 0x7e                                for (pos = 0, chr = 0x20; chr <= 0x7e; chr++)
    for (pos = 0, chr = 'a'; chr <= 'z'; chr++)
    {
        data->sys.data[pos++] = chr;
    }
    //
    for (int i = 0; i <= data->max; i++)
    {
        add_cs_buf(data->sys.data, data->cs, i);
    }
    //
    return data;
}

int maskprocessor_next(mp_data_t * data, char * word, int len)
{
    for (int i = len - 1; i >= 0; i--)
    {
        cs_t * cs = data->cs + i;
        if (cs->cs_pos < cs->cs_len)
        {
            const int c_new = cs->cs_buf[cs->cs_pos];
            word[i] = (char)c_new;
            cs->cs_pos++;
            return i;
        }
        const int c_new = cs->cs_buf[0];
        word[i] = (char)c_new;
        cs->cs_pos = 1;
    }
    return -1;
}

void maskprocessor_delete(mp_data_t ** data)
{
    if (*data != NULL)
    {
        free((void*)*data);
        *data = NULL;
    }
}

void maskprocessor_use_test(int min, int max)
{
    mp_data_t * mp_data = maskprocessor_init(min, max);
    char word[256] = {0x00};
    int word_len = 0;
    for (word_len = mp_data->min; word_len <= mp_data->max; word_len++)
    {
        for (int i = 0; i < word_len; i++)
        {
            mp_data->cs[i].cs_pos = mp_data->cs[i].cs_len;
        }
        mp_data->cs[0].cs_pos = 0;
        int first;
        while ((first = maskprocessor_next(mp_data, word, word_len)) != -1)
        {   // every time one word:
            qDebug() << word;
            // ToDo ... ...
        }
    }
    maskprocessor_delete(&mp_data);
}

