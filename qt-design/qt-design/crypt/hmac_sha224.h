#ifndef HMAC_SHA224_H
#define HMAC_SHA224_H

#include "my_qt_common.h"
#include "sha224.h"

typedef struct
{
    sha224_ctx_t ctx;
    u8 buffer[SHA224_BLOCK_SIZE * 2];
} hmac_sha224_ctx_t;

typedef struct
{
    sha224_hash_t hash;
} hmac_sha224_hash_t;


hmac_sha224_ctx_t * hmac_sha224_create();
hmac_sha224_hash_t * hmac_sha224_hash_create();

void hmac_sha224_start(hmac_sha224_ctx_t * ctx, const u8 * key, size_t key_len);

void hmac_sha224_update(hmac_sha224_ctx_t * ctx, const u8 * input, size_t inlen);

void hmac_sha224_final(hmac_sha224_ctx_t * ctx, hmac_sha224_hash_t * digest);

void hmac_sha224_delete(hmac_sha224_ctx_t ** ctx);
void hmac_sha224_hash_delete(hmac_sha224_hash_t ** hash);

void hmac_sha224(const u8 * key, size_t key_len, const u8 * input, size_t inlen, hmac_sha224_hash_t * digest);

#endif // HMAC_SHA224_H
