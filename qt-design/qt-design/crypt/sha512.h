#ifndef SHA512_H
#define SHA512_H

#include "my_qt_common.h"

#define SHA512_HASH_SIZE           ( 512 / 8 )
#define SHA512_BLOCK_SIZE          ( 128 )

typedef struct
{
    u64 total[2];                        // The number of Bytes processed.
    u64 state[8];                        // The intermediate digest state.
    u8 buffer[SHA512_BLOCK_SIZE];        // The data block being processed.
} sha512_ctx_t;

typedef struct
{
    u8 bytes[SHA512_HASH_SIZE];
} sha512_hash_t;


sha512_ctx_t * sha512_create();
sha512_hash_t * sha512_hash_create();

void sha512_start(sha512_ctx_t * ctx);

void sha512_update(sha512_ctx_t * ctx, u8 * buffer, size_t buffer_size);

void sha512_final(sha512_ctx_t * ctx, sha512_hash_t * digest);

void sha512_delete(sha512_ctx_t ** ctx);
void sha512_hash_delete(sha512_hash_t ** hash);

void sha512(u8 * buffer, size_t buffer_size, sha512_hash_t * digest);

#endif // SHA512_H
