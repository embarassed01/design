#ifndef SHA224_H
#define SHA224_H

#include "my_qt_common.h"

#define SHA224_HASH_SIZE           ( 224 / 8 )
#define SHA224_BLOCK_SIZE          ( 64 )

typedef struct
{
    u32 total[2];                        // The number of Bytes processed.
    u32 state[8];                        // The intermediate digest state.
    u8 buffer[SHA224_BLOCK_SIZE];        // The data block being processed.
} sha224_ctx_t;

typedef struct
{
    u8 bytes[SHA224_HASH_SIZE];
} sha224_hash_t;


sha224_ctx_t * sha224_create();
sha224_hash_t * sha224_hash_create();

void sha224_start(sha224_ctx_t * ctx);

void sha224_update(sha224_ctx_t * ctx, u8 * buffer, size_t buffer_size);

void sha224_final(sha224_ctx_t * ctx, sha224_hash_t * digest);

void sha224_delete(sha224_ctx_t ** ctx);
void sha224_hash_delete(sha224_hash_t ** hash);

void sha224(u8 * buffer, size_t buffer_size, sha224_hash_t * digest);

#endif // SHA224_H
