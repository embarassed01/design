#ifndef HMAC_MD4_H
#define HMAC_MD4_H

#include "my_qt_common.h"
#include "md4.h"

typedef struct
{
    md4_ctx_t ctx;
    u8 buffer[MD4_BLOCK_SIZE * 2];
} hmac_md4_ctx_t;

typedef struct
{
    md4_hash_t hash;
} hmac_md4_hash_t;


hmac_md4_ctx_t * hmac_md4_create();
hmac_md4_hash_t * hmac_md4_hash_create();

void hmac_md4_start(hmac_md4_ctx_t * ctx, const u8 * key, size_t key_len);

void hmac_md4_update(hmac_md4_ctx_t * ctx, const u8 * input, size_t inlen);

void hmac_md4_final(hmac_md4_ctx_t * ctx, hmac_md4_hash_t * hash);

void hmac_md4_delete(hmac_md4_ctx_t ** ctx);
void hmac_md4_hash_delete(hmac_md4_hash_t ** hash);

void hmac_md4(const u8 * key, size_t key_len, const u8 * input, size_t inlen, hmac_md4_hash_t * hash);

#endif // HMAC_MD4_H
