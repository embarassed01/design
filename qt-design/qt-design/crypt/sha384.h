#ifndef SHA384_H
#define SHA384_H

#include "my_qt_common.h"

#define SHA384_HASH_SIZE           ( 384 / 8 )
#define SHA384_BLOCK_SIZE          ( 128 )

typedef struct
{
    u64 total[2];                        // The number of Bytes processed.
    u64 state[8];                        // The intermediate digest state.
    u8 buffer[SHA384_BLOCK_SIZE];        // The data block being processed.
} sha384_ctx_t;

typedef struct
{
    u8 bytes[SHA384_HASH_SIZE];
} sha384_hash_t;


sha384_ctx_t * sha384_create();
sha384_hash_t * sha384_hash_create();

void sha384_start(sha384_ctx_t * ctx);

void sha384_update(sha384_ctx_t * ctx, u8 * buffer, size_t buffer_size);

void sha384_final(sha384_ctx_t * ctx, sha384_hash_t * digest);

void sha384_delete(sha384_ctx_t ** ctx);
void sha384_hash_delete(sha384_hash_t ** hash);

void sha384(u8 * buffer, size_t buffer_size, sha384_hash_t * digest);

#endif // SHA384_H
