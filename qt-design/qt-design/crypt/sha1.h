#ifndef SHA1_H
#define SHA1_H

#include "my_qt_common.h"

#define SHA1_HASH_SIZE           ( 160 / 8 )
#define SHA1_BLOCK_SIZE          ( 64 )

typedef struct
{
    u32 total[2];
    u32 state[5];     // 4*5=20  0x14  sha1
    u8 buffer[SHA1_BLOCK_SIZE];
} sha1_ctx_t;

typedef struct
{
    u8 bytes[SHA1_HASH_SIZE];
} sha1_hash_t;


sha1_ctx_t * sha1_create();
sha1_hash_t * sha1_hash_create();

void sha1_start(sha1_ctx_t * ctx);

void sha1_update(sha1_ctx_t * ctx, u8 * buffer, size_t buffer_size);

void sha1_final(sha1_ctx_t * ctx, sha1_hash_t * digest);

void sha1_delete(sha1_ctx_t ** ctx);
void sha1_hash_delete(sha1_hash_t ** hash);

void sha1(u8 * buffer, size_t buffer_size, sha1_hash_t * digest);

#endif // SHA1_H
