#ifndef MD4_H
#define MD4_H

#include "my_qt_common.h"

#define MD4_HASH_SIZE           ( 128 / 8 )
#define MD4_BLOCK_SIZE          ( 64 )

typedef struct
{
    u32 total[2];                      // number of bytes processed
    u32 state[4];                      // intermediate digest state
    u8  buffer[MD4_BLOCK_SIZE];        // data block being processed
} md4_ctx_t;

typedef struct
{
    u8 bytes[MD4_HASH_SIZE];
} md4_hash_t;


md4_ctx_t * md4_create();
md4_hash_t * md4_hash_create();

void md4_start(md4_ctx_t * ctx);

void md4_update(md4_ctx_t * ctx, const u8 * input, size_t ilen);

void md4_final(md4_ctx_t * ctx, md4_hash_t * hash);

void md4_delete(md4_ctx_t ** ctx);
void md4_hash_delete(md4_hash_t ** hash);

void md4(const u8 * input, size_t ilen, md4_hash_t * digest);


#endif // MD4_H
