#include "my_qt_common.h"
#include <QFileInfo>
#include <QDir>
#include <QDateTime>
#include <QDebug>

//////////// ................................... op about log ............................................
QString log(QString message)
{
    QString str;
    str += QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    str += " ";
    str += message;
//    str += "\r\n";
    return str;
}

QString loginfo(QString message)
{
    QString str;
    str += QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    str += "[info] ";
    str += message;
//    str += "\r\n";
    return str;
}

QString logwarn(QString message)
{
    QString str;
    str += QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    str += "[warn] ";
    str += message;
//    str += "\r\n";
    return str;
}

QString logerror(QString message)
{
    QString str;
    str += QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    str += "[error] ";
    str += message;
//    str += "\r\n";
    return str;
}

//static void log_msg (FILE * fp, const char * fmt, va_list ap)
//{
//    vfprintf (fp, fmt, ap);
//    fprintf (fp, "\n");
//}

//void log_info (const char * fmt, ...)
//{
//    va_list ap;
//    va_start (ap, fmt);
//    log_msg (stdout, fmt, ap);
//    va_end (ap);
//}

//void log_warn (const char * fmt, ...)
//{
//    va_list ap;
//    va_start (ap, fmt);
//    log_msg (stderr, fmt, ap);
//    va_end (ap);
//}

//void log_error (const char * fmt, ...)
//{
//    va_list ap;
//    va_start (ap, fmt);
//    fprintf (stderr, "\n\n");
//    log_msg(stderr, fmt, ap);
//    va_end(ap);
//}

void print_buffer(u8 * buffer, u32 bufflen)
{
    for (u32 i = 0; i < bufflen; i++)
    {
        printf ("%02x", buffer[i]);
    }
    printf ("\n");
}

////////////// ........................................... op about crypt common ................................

/*
 *   big endian store
 * high bits locate in low addr; low bits locate in high addr
*/
void store32_be(u8 dst[4], u32 w)
{
    u32 tmp = w;
    dst[3] = (u8)tmp; tmp >>= 8;
    dst[2] = (u8)tmp; tmp >>= 8;
    dst[1] = (u8)tmp; tmp >>= 8;
    dst[0] = (u8)tmp;
}

/*
 *   little endian store
 * high bits locate in high addr; low bits locate in low addr
*/
void store32_le(u8 dst[4], u32 w)
{
    u32 tmp = w;
    dst[0] = (u8)tmp; tmp >>= 8;
    dst[1] = (u8)tmp; tmp >>= 8;
    dst[2] = (u8)tmp; tmp >>= 8;
    dst[3] = (u8)tmp;
}

u8 tolower(u8 onebyte)
{
    if (onebyte >= 'A' && onebyte <= 'Z')
    {
        return (onebyte += ('a' - 'A'));
    }
}

u8 toupper(u8 onebyte)
{
    if (onebyte >= 'a' && onebyte <= 'z')
    {
        return (onebyte -= ('a' - 'A'));
    }
}

void lowercase(u8 * buf, int len)
{
    for (int i = 0; i < len; i++)
    {
        buf[i] = tolower(buf[i]);
    }
}

void uppercase(u8 * buf, int len)
{
    for (int i = 0; i < len; i++)
    {
        buf[i] = toupper(buf[i]);
    }
}

bool is_valid_base64_char(u8 c)
{
    if ((c >= '0') && (c <= '9'))
    {
        return true;
    }
    if ((c >= 'A') && (c <= 'Z'))
    {
        return true;
    }
    if ((c >= 'a') && (c <= 'z'))
    {
        return true;
    }
    if (c == '+')
    {
        return true;
    }
    if (c == '/')
    {
        return true;
    }
//    if (c == '.')
//    {
//        return true;
//    }
//    if (c == '-')
//    {
//        return true;
//    }
//    if (c == '_')
//    {
//        return true;
//    }
    if (c == '=')
    {
        return true;
    }
    return false;
}

bool is_valid_base64b_char(u8 c)
{
    if ((c >= '0') && (c <= '9'))
    {
        return true;
    }
    if ((c >= 'A') && (c <= 'Z'))
    {
        return true;
    }
    if ((c >= 'a') && (c <= 'z'))
    {
        return true;
    }
    if (c == '.')
    {
        return true;
    }
    if (c == '/')
    {
        return true;
    }
    if (c == '=')
    {
        return true;
    }
    return false;
}

bool is_valid_base64_string(u8 * s, int len)
{
    u8 c = 0;
    for (int i = 0; i < len; i++)
    {
        c = s[i];
        if (is_valid_base64_char(c) == false)
        {
            return false;
        }
    }
    return true;
}

bool is_valid_base64b_string(u8 * s, int len)
{
    u8 c = 0;
    for (int i = 0; i < len; i++)
    {
        c = s[i];
        if (is_valid_base64b_char(c) == false)
        {
            return false;
        }
    }
    return true;
}

u8 hex_convert(u8 c)
{
    return (c&0x0f)+(c>>6)*9;
}

u8 hex_to_u8(u8 hex[2])
{
    u8 v = 0;
    v |= (hex_convert(hex[1]) << 0);
    v |= (hex_convert(hex[0]) << 4);
    return v;
}

u32 hex_to_u32(u8 hex[8])
{
    u32 v = 0;
    v |= ((u32)hex_convert(hex[1]) <<  0);
    v |= ((u32)hex_convert(hex[0]) <<  4);
    v |= ((u32)hex_convert(hex[3]) <<  8);
    v |= ((u32)hex_convert(hex[2]) << 12);
    v |= ((u32)hex_convert(hex[5]) << 16);
    v |= ((u32)hex_convert(hex[4]) << 20);
    v |= ((u32)hex_convert(hex[7]) << 24);
    v |= ((u32)hex_convert(hex[6]) << 28);
    return v;
}

bool is_valid_hex_char(u8 c)
{
    if ((c >= '0') && (c <= '9'))
    {
        return true;
    }
    if ((c >= 'A') && (c <= 'F'))
    {
        return true;
    }
    if ((c >= 'a') && (c <= 'f'))
    {
        return true;
    }
    return false;
}

bool is_valid_hex_string(u8 * s, int len)
{
    u8 c = 0;
    for (int i = 0; i < len; i++)
    {
        c = s[i];
        if (is_valid_hex_char(c) == false)
        {
            return false;
        }
    }
    return true;
}

u32 my_add(u32 a, u32 b)
{
    u32 c = (a & b) << 1;
    u32 r = a ^ b;   // or ((~a)&b)|(a&(~b))
    if (c)
    {
        return my_add(c, r);
    }
    return r;
}

u8 itoa64_to_int(const u8 c)
{
  const u8 tbl[0x100] =
  {
    0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21,
    0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31,
    0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x00, 0x01,
    0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
    0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a,
    0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x20, 0x21, 0x22, 0x23, 0x24,
    0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34,
    0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x00, 0x01, 0x02, 0x03, 0x04,
    0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14,
    0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24,
    0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34,
    0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x00, 0x01, 0x02, 0x03, 0x04,
    0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14,
    0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24,
    0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34,
    0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x00, 0x01, 0x02, 0x03, 0x04,
  };

  return tbl[c];
}

int string_to_unicode(char * in_str, u32 in_len, char * out_str)
{
    for (int i = in_len - 1; i > 0; i--)
    {
        out_str[i*2] = in_str[i];
        out_str[i*2-1] = 0x00;
    }
    out_str[in_len*2-1] = 0x00;
    out_str[0] = in_str[0];
    return in_len*2;
}

void phpass_decode(u8 digest[16], u8 buf[22])
{
  int l;

  l  = itoa64_to_int (buf[ 0]) <<  0;
  l |= itoa64_to_int (buf[ 1]) <<  6;
  l |= itoa64_to_int (buf[ 2]) << 12;
  l |= itoa64_to_int (buf[ 3]) << 18;

  digest[ 0] = (l >>  0) & 0xff;
  digest[ 1] = (l >>  8) & 0xff;
  digest[ 2] = (l >> 16) & 0xff;

  l  = itoa64_to_int (buf[ 4]) <<  0;
  l |= itoa64_to_int (buf[ 5]) <<  6;
  l |= itoa64_to_int (buf[ 6]) << 12;
  l |= itoa64_to_int (buf[ 7]) << 18;

  digest[ 3] = (l >>  0) & 0xff;
  digest[ 4] = (l >>  8) & 0xff;
  digest[ 5] = (l >> 16) & 0xff;

  l  = itoa64_to_int (buf[ 8]) <<  0;
  l |= itoa64_to_int (buf[ 9]) <<  6;
  l |= itoa64_to_int (buf[10]) << 12;
  l |= itoa64_to_int (buf[11]) << 18;

  digest[ 6] = (l >>  0) & 0xff;
  digest[ 7] = (l >>  8) & 0xff;
  digest[ 8] = (l >> 16) & 0xff;

  l  = itoa64_to_int (buf[12]) <<  0;
  l |= itoa64_to_int (buf[13]) <<  6;
  l |= itoa64_to_int (buf[14]) << 12;
  l |= itoa64_to_int (buf[15]) << 18;

  digest[ 9] = (l >>  0) & 0xff;
  digest[10] = (l >>  8) & 0xff;
  digest[11] = (l >> 16) & 0xff;

  l  = itoa64_to_int (buf[16]) <<  0;
  l |= itoa64_to_int (buf[17]) <<  6;
  l |= itoa64_to_int (buf[18]) << 12;
  l |= itoa64_to_int (buf[19]) << 18;

  digest[12] = (l >>  0) & 0xff;
  digest[13] = (l >>  8) & 0xff;
  digest[14] = (l >> 16) & 0xff;

  l  = itoa64_to_int (buf[20]) <<  0;
  l |= itoa64_to_int (buf[21]) <<  6;

  digest[15] = (l >>  0) & 0xff;
}

int rounds_count_length(const char * input_buf, const int input_len)
{
    if (input_len >= 9) // 9 is minimum because of "rounds=X$"
    {
        const char * rounds = "rounds=";
        if (memcmp(input_buf, rounds, 7) == 0)
        {
            char * next_pos = strchr((char*)input_buf + 8, '$');

            if (next_pos == NULL)
            {
                return -1;
            }

            const int rounds_len = next_pos - input_buf;

            return rounds_len;
        }
    }
    return -1;
}

void md5crypt_decode(u8 digest[16], u8 buf[22])
{
  int l;

  l  = itoa64_to_int (buf[ 0]) <<  0;
  l |= itoa64_to_int (buf[ 1]) <<  6;
  l |= itoa64_to_int (buf[ 2]) << 12;
  l |= itoa64_to_int (buf[ 3]) << 18;

  digest[ 0] = (l >> 16) & 0xff;
  digest[ 6] = (l >>  8) & 0xff;
  digest[12] = (l >>  0) & 0xff;

  l  = itoa64_to_int (buf[ 4]) <<  0;
  l |= itoa64_to_int (buf[ 5]) <<  6;
  l |= itoa64_to_int (buf[ 6]) << 12;
  l |= itoa64_to_int (buf[ 7]) << 18;

  digest[ 1] = (l >> 16) & 0xff;
  digest[ 7] = (l >>  8) & 0xff;
  digest[13] = (l >>  0) & 0xff;

  l  = itoa64_to_int (buf[ 8]) <<  0;
  l |= itoa64_to_int (buf[ 9]) <<  6;
  l |= itoa64_to_int (buf[10]) << 12;
  l |= itoa64_to_int (buf[11]) << 18;

  digest[ 2] = (l >> 16) & 0xff;
  digest[ 8] = (l >>  8) & 0xff;
  digest[14] = (l >>  0) & 0xff;

  l  = itoa64_to_int (buf[12]) <<  0;
  l |= itoa64_to_int (buf[13]) <<  6;
  l |= itoa64_to_int (buf[14]) << 12;
  l |= itoa64_to_int (buf[15]) << 18;

  digest[ 3] = (l >> 16) & 0xff;
  digest[ 9] = (l >>  8) & 0xff;
  digest[15] = (l >>  0) & 0xff;

  l  = itoa64_to_int (buf[16]) <<  0;
  l |= itoa64_to_int (buf[17]) <<  6;
  l |= itoa64_to_int (buf[18]) << 12;
  l |= itoa64_to_int (buf[19]) << 18;

  digest[ 4] = (l >> 16) & 0xff;
  digest[10] = (l >>  8) & 0xff;
  digest[ 5] = (l >>  0) & 0xff;

  l  = itoa64_to_int (buf[20]) <<  0;
  l |= itoa64_to_int (buf[21]) <<  6;

  digest[11] = (l >>  0) & 0xff;
}


////////////// ........................................... op about mem .........................................
void free_ss(void ** ss, i64 ss_len)
{  // (**)ss like ss[ss_len][]
    if (ss != NULL)
    {
        if (ss_len > 0)
        {
            for (i64 h = 0; h < ss_len; h++)
            {
                if (ss[h] != NULL)
                {
                    free(ss[h]);
                    ss[h] = NULL;
                }
            }
        }
        free(ss);
        ss = NULL;
    }
}

////// ............................ 1. alloc aux ............................................
/** exported global variable definition section **/
void (*_gpfun_malloc_handler)(void) = NULL;

/** exported function implementation section **/
/**
 * Malloc for out of memory condition.
 */
void * _alloc_malloc_out_of_memory (size_t t_memsize)
{
    void * pv_allocmem = NULL;     /* point to allocated memory */
    for (;;)
    {
        if (_gpfun_malloc_handler == NULL)
        {
            fprintf(stderr, "FATAL ERROR: memory allocation error!\n");
            exit(EXIT_FAILURE);
            return NULL;
        }
        else
        {
            (*_gpfun_malloc_handler)();
            if ((pv_allocmem = (void*)malloc( t_memsize )) != NULL)
            {
                return pv_allocmem;
            }
        }
    }
}

/**
 * Malloc memory.
 */
void * _alloc_malloc (size_t t_memsize)
{
    void * pv_allocmem = NULL;    /* point to the allocated memory */
    if ((pv_allocmem = (void*)malloc( t_memsize )) == NULL)
    {
        pv_allocmem = _alloc_malloc_out_of_memory(t_memsize);
        assert(pv_allocmem != NULL);
    }
    return pv_allocmem;
}

/**
 * Free memory.
 */
void _alloc_free (void * pv_allocmem)
{
    assert (pv_allocmem != NULL);
    free(pv_allocmem);
}

/**
 * Apply a formated memory list.
 */
void _alloc_apply_formated_memory (_alloc_t * pt_allocator, size_t t_allocsize)
{
    size_t t_alloccount = _MEM_CHUNK_COUNT;
    _memlink_t * pt_link = NULL;
    size_t i = 0;
    assert(pt_allocator != NULL);
    pt_link = (_memlink_t*)_alloc_get_memory_chunk(pt_allocator, t_allocsize, &t_alloccount);
    assert(pt_link != NULL);
    /* format the apply memory block to memory list format */
    pt_allocator->_apt_memlink[_MEM_LINK_INDEX(t_allocsize)] = pt_link;
    for (i = 0; i < t_alloccount; ++i)
    {
        if (i == t_alloccount - 1)
        {
            pt_link->_pui_nextmem = NULL;
        }
        else
        {
            pt_link->_pui_nextmem = (_memlink_t*)((u8*)pt_link + t_allocsize);
        }
        pt_link = pt_link->_pui_nextmem;
    }
}

/**
 * Get memory chunk from memory pool.
 */

u8 * _alloc_get_memory_chunk (_alloc_t* pt_allocator, size_t t_allocsize, size_t* pt_alloccount)
{
    size_t t_totalsize = 0;     /* total size of alloc */
    u8 * pby_allocmem = NULL;   /* the allocated memory block */
    size_t t_getmemsize = 0;    /* bytes to get memory from system heap */
    size_t i = 0;
    assert(pt_allocator != NULL);
    assert(pt_alloccount != NULL);
    t_totalsize = t_allocsize * (*pt_alloccount);
    if (pt_allocator->_t_mempoolsize >= t_totalsize)
    {
        pby_allocmem = pt_allocator->_pby_mempool;
        pt_allocator->_pby_mempool = pby_allocmem + t_totalsize;
        pt_allocator->_t_mempoolsize -= t_totalsize;
        return pby_allocmem;
    }
    else if (pt_allocator->_t_mempoolsize >= t_allocsize)
    {
        *pt_alloccount = pt_allocator->_t_mempoolsize / t_allocsize;
        t_totalsize = t_allocsize * (*pt_alloccount);
        pby_allocmem = pt_allocator->_pby_mempool;
        pt_allocator->_pby_mempool = pby_allocmem + t_totalsize;
        pt_allocator->_t_mempoolsize -= t_totalsize;
        return pby_allocmem;
    }
    else
    {
        /* if the memory pool container is full */
        assert(pt_allocator->_t_mempoolindex <= pt_allocator->_t_mempoolcount);
        /* take the small memory block to the memory list */
        if (pt_allocator->_t_mempoolsize > 0)
        {
            ((_memlink_t*)pt_allocator->_pby_mempool)->_pui_nextmem = pt_allocator->_apt_memlink[_MEM_LINK_INDEX(pt_allocator->_t_mempoolsize)];
            pt_allocator->_apt_memlink[_MEM_LINK_INDEX(pt_allocator->_t_mempoolsize)] = (_memlink_t*)pt_allocator->_pby_mempool;
            pt_allocator->_t_mempoolsize = 0;
        }
        t_getmemsize = 2 * t_totalsize;
        pt_allocator->_pby_mempool = (u8*)malloc( t_getmemsize );
        if (pt_allocator->_pby_mempool == NULL)
        {
            /* search the memory list for unuse memory that meet the size */
            for (i = t_allocsize; i < _MEM_SMALL_MEM_SIZE_MAX; i += _MEM_ALIGNMENT)
            {
                pt_allocator->_pby_mempool = (u8*)pt_allocator->_apt_memlink[_MEM_LINK_INDEX((i))];
                if (pt_allocator->_pby_mempool != NULL)
                {
                    pt_allocator->_apt_memlink[_MEM_LINK_INDEX(i)] = ((_memlink_t*)pt_allocator->_pby_mempool)->_pui_nextmem;
                    pt_allocator->_t_mempoolsize = i;
                    return _alloc_get_memory_chunk(pt_allocator, t_allocsize, pt_alloccount);
                }
            }
            pt_allocator->_pby_mempool = (u8*)_alloc_malloc_out_of_memory(t_getmemsize);
        }
        /* if the memory pool container is full */
        if (pt_allocator->_t_mempoolindex == pt_allocator->_t_mempoolcount)
        {
            u8 ** ppby_oldmempool = pt_allocator->_ppby_mempoolcontainer;
            pt_allocator->_t_mempoolcount += _MEM_POOL_DEFAULT_COUNT;
            pt_allocator->_ppby_mempoolcontainer = (u8**)malloc( pt_allocator->_t_mempoolcount * sizeof(u8*) );
            if (pt_allocator->_ppby_mempoolcontainer != NULL)
            {
                for (i = 0; i < pt_allocator->_t_mempoolcount; ++i)
                {
                    pt_allocator->_ppby_mempoolcontainer[i] = NULL;
                }
                memcpy(pt_allocator->_ppby_mempoolcontainer, ppby_oldmempool, (pt_allocator->_t_mempoolcount - _MEM_POOL_DEFAULT_COUNT) * sizeof(u8*));
                free(ppby_oldmempool);
                assert(pt_allocator->_t_mempoolindex < pt_allocator->_t_mempoolcount);
            }
            else
            {
                fprintf(stderr,  "FATAL ERROR: memory allocation error!\n");
                exit(EXIT_FAILURE);
            }
        }
        pt_allocator->_ppby_mempoolcontainer[pt_allocator->_t_mempoolindex++] = pt_allocator->_pby_mempool;
        /* apply memory from system heap success or call _alloc_malloc_out_of_memory success */
        pt_allocator->_t_mempoolsize = t_getmemsize;
        pby_allocmem = pt_allocator->_pby_mempool;
        pt_allocator->_pby_mempool = pby_allocmem + t_totalsize;
        pt_allocator->_t_mempoolsize -= t_totalsize;
        return pby_allocmem;
    }
}

////// ............................ 2. alloc     ............................................
/**
 * Initialize the _alloc_t.
 **/
void _alloc_init (_alloc_t * pt_allocator)
{
    size_t i = 0;
    assert(pt_allocator != NULL);
    pt_allocator->_t_mempoolsize = 0;
    pt_allocator->_t_mempoolindex = 0;
    pt_allocator->_pby_mempool = NULL;
    for (i = 0; i < _MEM_LINK_COUNT; i++)
    {
        pt_allocator->_apt_memlink[i] = NULL;
    }
    /* initialize memory pool */
    pt_allocator->_t_mempoolcount = _MEM_POOL_DEFAULT_COUNT;
    pt_allocator->_ppby_mempoolcontainer = (u8**)malloc( pt_allocator->_t_mempoolcount * sizeof(u8*) );
    if (pt_allocator->_ppby_mempoolcontainer != NULL)
    {
        for (i = 0; i < pt_allocator->_t_mempoolcount; i++)
        {
            pt_allocator->_ppby_mempoolcontainer[i] = NULL;
        }
    }
    else
    {
        fprintf(stderr, "FATAL ERROR: memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Destroy the _alloc_t.
 */
void _alloc_destroy(_alloc_t * pt_allocator)
{
    size_t i = 0;
    assert(pt_allocator != NULL);
    /* destroy memory pool */
    for (i = 0; i < pt_allocator->_t_mempoolcount; ++i)
    {
        free(pt_allocator->_ppby_mempoolcontainer[i]);
        pt_allocator->_ppby_mempoolcontainer[i] = NULL;
    }
    free(pt_allocator->_ppby_mempoolcontainer);
    pt_allocator->_ppby_mempoolcontainer = NULL;
    for (i = 0; i < _MEM_LINK_COUNT; ++i)
    {
        pt_allocator->_apt_memlink[i] = NULL;
    }
    pt_allocator->_pby_mempool = NULL;
    pt_allocator->_t_mempoolindex = 0;
    pt_allocator->_t_mempoolsize = 0;
    pt_allocator->_t_mempoolcount = 0;
}

/**
 * Allocate to user specified amount of memory.
 */
void * _alloc_allocate (_alloc_t * pt_allocator, size_t t_size, size_t t_count)
{
    void * pv_allocmem = NULL;        /* allocated memory pointer */
    _memlink_t * pt_link = NULL;      /* memory link */
    size_t t_allocsize = t_size * t_count;    /* allocated memory size */
    if (t_allocsize == 0)
    {
        return NULL;
    }
    assert(pt_allocator != NULL);
    if (t_allocsize > _MEM_SMALL_MEM_SIZE_MAX)   // mem>128
    {
        pv_allocmem = _alloc_malloc(t_allocsize);
        assert(pv_allocmem != NULL);
    }
    else
    {
        pt_link = pt_allocator->_apt_memlink[_MEM_LINK_INDEX(t_allocsize)];
        if (pt_link == NULL)
        {
            _alloc_apply_formated_memory(pt_allocator, _MEM_ROUND_UP(t_allocsize));
            pt_link = pt_allocator->_apt_memlink[_MEM_LINK_INDEX(t_allocsize)];
            assert(pt_link != NULL);
        }
        pt_allocator->_apt_memlink[_MEM_LINK_INDEX(t_allocsize)] = pt_link->_pui_nextmem;
        pv_allocmem = (void*)pt_link;
    }

    return pv_allocmem;
}

/**
 * Release allocated memory
 */
void _alloc_deallocate (_alloc_t * pt_allocator, void * pv_allocmem, size_t t_size, size_t t_count)
{
    size_t t_allocsize = t_size * t_count;    /* allocated memory size */
    if (t_allocsize == 0)
    {
        return;
    }
    assert(pt_allocator != NULL);
    assert(pv_allocmem != NULL);
    if (t_allocsize > _MEM_SMALL_MEM_SIZE_MAX)
    {
        _alloc_free(pv_allocmem);
    }
    else
    {
        ((_memlink_t*)pv_allocmem)->_pui_nextmem = pt_allocator->_apt_memlink[_MEM_LINK_INDEX((t_allocsize))];
        pt_allocator->_apt_memlink[_MEM_LINK_INDEX((t_allocsize))] = ((_memlink_t*)pv_allocmem);
    }
}

/**
 * Set the out of memory handler and return the old handler.
 */
void (*_alloc_set_malloc_handler(void (*pfun_newhandler)(void)))(void)
{
    void (*pfun_oldhandler)(void) = _gpfun_malloc_handler;
    _gpfun_malloc_handler = pfun_newhandler;
    return pfun_oldhandler;
}

////////////////////................................ op about file ....................................................
static bool is_special_dir (const char * path);
void delete_files(const char * path)
{
    if (strlen(path) >= 2000)
    {
        return;
    }
    char file_path[2048];
	struct stat s_buf;
	if (stat(path, &s_buf) == -1)
    {
        return;
    }
    if (S_ISDIR(s_buf.st_mode))
	{
		struct dirent * drt;
		DIR * dir;
		dir = opendir (path);
		// readdir must be called repeatly!
		while ((drt = readdir(dir)) != NULL)
		{
			// skiss '.' '..'
			if (is_special_dir(drt->d_name))
			{
				continue;
			}
			memset(file_path, 0x00, 2048);            
			strcpy(file_path, path);
			strcat (file_path, "/");
			strcat (file_path, drt->d_name);
			stat(file_path, &s_buf);
			if (S_ISDIR(s_buf.st_mode))
			{  // this is a folder, skiss!
				continue;
			}
			else if (S_ISREG(s_buf.st_mode))
			{  // this is a regular file, delete!!
				remove (file_path);
			}
		}
        closedir(dir);
	}
	else if (S_ISREG (s_buf.st_mode))
	{ // if is a regular file, nothing to do.		
    }
}

void delete_file (QString filename)
{
    QFile * file = new QFile(filename);
    // 判断文件是否存在：
    if (file->exists())
    {
        file->remove(filename);
    }
    file->close();
    delete file;
}

// N.B. : cannot support Multi-Set: Chinese, Japan, Korea Language ... ...
char ** traversing_one_folder (const char * path, int * files_cnt)
{
    files_cnt[0] = 0;
    char ** retss = NULL;
    if (strlen(path) >= 2000)
    {
        return NULL;
    }
    char file_path[2048];
    struct stat s_buf;
    if (stat(path, &s_buf) == -1)
    {
        return NULL;
    }
    if (S_ISREG (s_buf.st_mode))
    {
        return NULL;
    }
    else if (S_ISDIR(s_buf.st_mode))
	{
        retss = (char **)malloc( sizeof(char*)*0x100000 );    // 1M
        memset((void*)retss, 0x00, sizeof(char*)*0x100000 );
		struct dirent * drt;
		DIR * dir;
		dir = opendir (path);
		// readdir must be called repeatly!
        int cnt = 0;
		while ((drt = readdir(dir)) != NULL)
		{
			// skiss '.' '..'
			if (is_special_dir(drt->d_name))
			{
				continue;
			}
			memset(file_path, 0x00, 2048);            
			strcpy(file_path, path);
			strcat (file_path, "/");
			strcat (file_path, drt->d_name);
			stat(file_path, &s_buf);
			if (S_ISDIR(s_buf.st_mode))
			{  // this is a folder, skiss!
				continue;
			}
			else if (S_ISREG(s_buf.st_mode))
			{  // add to retss
                cnt++;
                if (cnt > 0x10000)
                {
                    retss = (char **)realloc( retss, sizeof(char *)*cnt );
                }
                int len = strlen(file_path);
                len = len%0x10 ? ((len/0x10)+1)*0x10: len;
                retss[cnt-1] = (char *)malloc( sizeof(u8)*(len+0x10) );
                strcpy(retss[cnt-1], file_path);
                files_cnt[0] = cnt;
			}
		}
        closedir(dir);
	}
    return retss;
}

void qt_traversing_one_folder (QString inpath, QStringList * p_files_list)
{
    QFileInfo info(inpath);
    if (!info.isDir())
    {
        return;
    }
    QDir d(inpath);
    d.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::AllDirs);
    QFileInfoList list = d.entryInfoList();
    QFileInfoList::const_iterator iterator = list.begin();
    while ( iterator != list.end() )
    {
        // skip special folder: "."  ".."
        if ( (*iterator).fileName() == "." || (*iterator).fileName() == ".." )
        {
            iterator++;
            continue;
        }
        if ((*iterator).isDir())
        {
            // digui:
            qt_traversing_one_folder((*iterator).filePath(), p_files_list);
        }
        else if ((*iterator).isFile())
        {
            (*p_files_list).append((*iterator).filePath());
        }
        iterator++;
    }
    return;
}

QStringList find_somefiles_one_folder(QString path, QString reg)
{
    QStringList ret;
    QFileInfoList infos = QDir(path).entryInfoList();
    foreach (QFileInfo info, infos)
    {
        if (!info.isFile())
        {
            continue;
        }
        if (info.fileName().contains(reg))
        {
            ret.append(info.filePath());
        }
    }
    infos.clear();
    return ret;
}

bool is_special_dir(const char * path)
{
    return strcmp (path, "..") == 0 || strcmp (path, ".") == 0;
}

bool direct_empty(char * dn)
{
    struct dirent * dp;
    DIR * dfd;
    bool is_empty = true;
    dfd = opendir(dn);
    while ((dp = readdir(dfd)) != NULL)
    {
        if (is_special_dir(dp->d_name))
            continue;
        is_empty = false;
        break;
    }
    closedir(dfd);
    return is_empty;
}

bool is_exist_folder(QString path)
{
    QDir * file_dir = new QDir(path);
    bool isexist = false;
    if (!file_dir->exists(path))
    {
        isexist = false;
    }
    else
    {
        isexist = true;
    }
    delete file_dir;
    return isexist;
}

QString get_up_folder(QString infile)
{
    QDir file_updir(infile);
    file_updir.cdUp();
    return file_updir.path();
    /*     or
      QFileInfo fileinfo(infile);
      return fileinfo.path();
    */
}

u8 ** read_block_txt_to_mem(txtfile_gdata_t * global_data, QString intxtfile, int  block_size, int * real_block_size, int * lines_cnt)
{
    real_block_size[0] = 0;
    //// 1. fp open file
    if (intxtfile != "")
    {
        global_data->fp = new QFile(intxtfile);
        global_data->fp->open(QIODevice::ReadOnly);
        if (global_data->fp == NULL)
        {
            if (global_data->len_remain_buf > 0)
            {
                free(global_data->remain_buf);
                global_data->remain_buf = NULL;
            }
            global_data->remain_buf = NULL;
            global_data->len_remain_buf = 0;
            real_block_size[0] = -1;
            return NULL;
        }
        global_data->is_eof = false;
        global_data->is_first_read = true;
        if (global_data->len_remain_buf > 0)
        {
            free(global_data->remain_buf);
            global_data->remain_buf = NULL;
        }
        global_data->remain_buf = NULL;
        global_data->len_remain_buf = 0;
    }
    else
    {  // IF intxtfile == NULL(""): continue reading same intxtfile.
        global_data->is_first_read = false;
    }

    //// 2. remain+block->buff
    u8 * buff = NULL;
    u8 * tmpbuff = NULL;
    int len_tmpbuff = block_size+1;
    len_tmpbuff = len_tmpbuff%0x10 ? ((len_tmpbuff/0x10)+1)*0x10 : len_tmpbuff;
    tmpbuff = (u8*)malloc( sizeof(u8)*(len_tmpbuff+0x10) );
    memset((void*)tmpbuff, 0x00, sizeof(u8)*(len_tmpbuff+0x10));
    real_block_size[0] = global_data->fp->read((char*)tmpbuff, block_size);
    if (real_block_size[0] <= 0)  // read at the eof of file:
    {
        if (global_data->len_remain_buf > 0)
        {
            buff = (u8 *)malloc( sizeof(u8)*(global_data->len_remain_buf+0x10) );
            memset(buff, 0x00, sizeof(u8)*(global_data->len_remain_buf+0x10));
            memset(buff + global_data->len_remain_buf, '\n', 1);
            memcpy(buff, global_data->remain_buf, global_data->len_remain_buf);
            real_block_size[0] = global_data->len_remain_buf + 1;  // add the '\n' 便于后面利用该标志字节确定。
            free(global_data->remain_buf);
            global_data->remain_buf = NULL;
            global_data->len_remain_buf = 0;
        }
        global_data->is_eof = true;
        free(tmpbuff);
        tmpbuff = NULL;
        global_data->fp->close();
        delete global_data->fp;
        global_data->fp = NULL;
    }
    else if (global_data->len_remain_buf > 0)  // concat remain + inbuf, when during reading the file:
    {
        buff = (u8 *)malloc( sizeof(u8)*(global_data->len_remain_buf + real_block_size[0]+0x10) );
        memset(buff, 0x00, sizeof(u8)*(global_data->len_remain_buf + real_block_size[0]+0x10));
        memcpy(buff, global_data->remain_buf, global_data->len_remain_buf);
        memcpy(buff + global_data->len_remain_buf, tmpbuff, real_block_size[0]);
        real_block_size[0] = global_data->len_remain_buf + real_block_size[0];
        free(global_data->remain_buf);
        global_data->remain_buf = NULL;
        global_data->len_remain_buf = 0;
        free(tmpbuff);
        tmpbuff = NULL;
    }
    else    // when no remaindata, during reading the file:
    {
        buff = tmpbuff;
        real_block_size[0] = real_block_size[0];
    }

    //// 3.  buff->buffss buffss[n_lines][line_str]
    lines_cnt[0] = 0;
    u8 * p = buff;
    u8 * cur = buff;
    int n_line = 0;
    u8 ** buffss = (u8**)malloc( sizeof(u8*)*RD_UNIT_BLOCK );  // 申请10MB大小的二维数组。
    memset((void*)buffss, 0x00, sizeof(u8*)*RD_UNIT_BLOCK);
    for (int i = 0; i < real_block_size[0]; i++)
    {
        if (*p == '\n')
        {
            *p = 0x00;                //  目前不考虑 多字节 的问题！ 默认认为0x00是字符串结束标志.
//            if ((global_data->is_first_read && p - cur >= 1 && *(p - 1) == '\r') ||
//                    (!global_data->is_first_read && *(p - 1) == '\r'))
            if ( p - cur >= 1 && *(p - 1) == '\r' )
            {
                *(p - 1) = 0x00;    // 考虑到windows和linux的回车的区别："\r\n"和'\n'
            }

            if (strlen((char*)cur) == 0)
            {
                cur = p + 1;
                p++;
                continue;
            }

            n_line++;
            lines_cnt[0] = n_line;
            int len = strlen((char*)cur)+1;
            len = len%0x10 ? ((len/0x10)+1)*0x10 : len;
            buffss[n_line - 1] = (u8*)malloc( sizeof(u8)*(len+0x10) );
            memset(buffss[n_line-1], 0x00, sizeof(u8)*(len+0x10));
            strcpy((char*)buffss[n_line-1], (char*)cur);

            cur = p + 1;
        }
        p++;
    }

    //// 4. process remain
    if (cur != NULL && strlen((char*)cur) != 0)
    {
        global_data->len_remain_buf = strlen((char*)cur);
        global_data->remain_buf = (u8 *)malloc( sizeof(u8)*(global_data->len_remain_buf+0x10) );
        memset(global_data->remain_buf, 0x00, sizeof(u8)*(global_data->len_remain_buf+0x10));
        memcpy(global_data->remain_buf, cur, global_data->len_remain_buf);
        real_block_size[0] = real_block_size[0] - global_data->len_remain_buf;
    }

    free(buff);
    buff = NULL;
    if (global_data->is_eof)
    {
        real_block_size[0] = -1;
        free(global_data->remain_buf);
        global_data->remain_buf = NULL;
        global_data->len_remain_buf = 0;
    }

    return buffss;
}

u8 * read_block_bin_to_mem (binfile_gdata_t * global_data, QString inbinfile, int block_size, int * real_block_size)
{
    real_block_size[0] = 0;
    //// 1. fr open file
     if (inbinfile != "")
     {
         global_data->fp = new QFile(inbinfile);
         global_data->fp->open(QIODevice::ReadOnly);
         if (global_data->fp == NULL)
         {
             real_block_size[0] = -1;
             return NULL;
         }
         global_data->is_eof = false;
     }

     //// 2. block->buffs
     int len_tmpbuff = block_size+1;
     len_tmpbuff = len_tmpbuff%0x10 ? ((len_tmpbuff/0x10)+1)*0x10 : len_tmpbuff;
     u8 * buffs = (u8*)malloc ( sizeof(u8)*(len_tmpbuff+0x10) );
     memset(buffs, 0x00, sizeof(u8)*(len_tmpbuff+0x10));
     real_block_size[0] = global_data->fp->read((char*)buffs, block_size);
     if (real_block_size[0] <= 0)
     {  // read at the eof of file.
         global_data->is_eof = true;
         free(buffs);
         buffs = NULL;
         global_data->fp->close();
         delete global_data->fp;
         global_data->fp = NULL;
     }
    if (global_data->is_eof)
    {
        real_block_size[0] = -1;
    }
    return buffs;
}

u64 get_file_size(QString infile)
{
    QFileInfo * pinfo = new QFileInfo(infile);
    u64 fsize = pinfo->size();
    delete pinfo;
    return fsize;
}

QString get_file_name(QString file_fullpath)
{
    QFileInfo * pinfo = new QFileInfo(file_fullpath);
    QString filename = pinfo->fileName();
    delete pinfo;
    return filename;
}
//////////////// ...............................................................................................................

u64 swap2bigend_64(u8 * in)
{
    u64 n = *(u64*)in;
    return (n & 0xFF00000000000000ULL) >> 56 | (n & 0x00FF000000000000ULL) >> 40 |
              (n & 0x0000FF0000000000ULL) >> 24 | (n & 0x000000FF00000000ULL) >> 8 |
              (n & 0x00000000FF000000ULL) << 8  | (n & 0x0000000000FF0000ULL) << 24 |
              (n & 0x000000000000FF00ULL) << 40 | (n & 0x00000000000000FFULL) << 56;
    // or
//    u8 tmp[8] = {0x00};
//    for (int i = 0; i < 8; i++)
//    {
//        tmp[i] = in[8-1-i];
//    }
//    return *(u64*)tmp;
}

u32 swap2bigend_32(u8 * in)
{
    u32 n = *(u32*)in;
    return (n & 0xFF000000) >> 24 | (n & 0x00FF0000) >> 8 |
                (n & 0x0000FF00) << 8 | (n & 0x000000FF) << 24;
    // or
//    u8 tmp[4] = {0x00};
//    for (int i = 0; i < 4; i++)
//    {
//        tmp[i] = in[4-1-i];
//    }
//    return *(u32*)tmp;
}

u16 swap2bigend_16(u8 * in)
{
    u16 n = *(u16*)in;
    return (u16)((n >> 8) | (n << 8));
    // or
//    u8 tmp[2] = {0x00};
//    for (int i = 0; i < 2; i++)
//    {
//        tmp[i] = in[2-1-i];
//    }
//    return *(u16*)tmp;
}

