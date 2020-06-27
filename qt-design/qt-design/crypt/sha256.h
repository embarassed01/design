#ifndef SHA256_H
#define SHA256_H

#include "my_qt_common.h"

#define SHA256_HASH_SIZE           ( 256 / 8 )
#define SHA256_BLOCK_SIZE          ( 64 )

typedef struct
{
    u32 total[2];                        // The number of Bytes processed.
    u32 state[8];                        // The intermediate digest state.
    u8 buffer[SHA256_BLOCK_SIZE];        // The data block being processed.
} sha256_ctx_t;

typedef struct
{
    u8 bytes[SHA256_HASH_SIZE];
} sha256_hash_t;


sha256_ctx_t * sha256_create();
sha256_hash_t * sha256_hash_create();

void sha256_start(sha256_ctx_t * ctx);

void sha256_update(sha256_ctx_t * ctx, u8 * buffer, size_t buffer_size);

void sha256_final(sha256_ctx_t * ctx, sha256_hash_t * digest);

void sha256_delete(sha256_ctx_t ** ctx);
void sha256_hash_delete(sha256_hash_t ** hash);

void sha256(u8 * buffer, size_t buffer_size, sha256_hash_t * digest);

#endif // SHA256_H
