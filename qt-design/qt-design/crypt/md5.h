#ifndef MD5_H
#define MD5_H

#include "my_qt_common.h"

#define MD5_HASH_SIZE           ( 128 / 8 )
#define MD5_BLOCK_SIZE          ( 64 )

typedef struct
{
    u32 total[2];                      // number of bytes processed
    u32 state[4];                      // intermediate digest state
    u8  buffer[MD5_BLOCK_SIZE];        // data block being processed
} md5_ctx_t;

typedef struct
{
    u8 bytes[MD5_HASH_SIZE];
} md5_hash_t;


md5_ctx_t * md5_create();
md5_hash_t * md5_hash_create();

void md5_start(md5_ctx_t * ctx);

void md5_update(md5_ctx_t * ctx, const u8 * input, size_t ilen);

void md5_final(md5_ctx_t * ctx, md5_hash_t * hash);

void md5_delete(md5_ctx_t ** ctx);
void md5_hash_delete(md5_hash_t ** hash);

void md5(const u8 * input, size_t ilen, md5_hash_t * hash);


#endif // MD5_H
