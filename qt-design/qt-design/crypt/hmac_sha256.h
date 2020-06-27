#ifndef HMAC_SHA256_H
#define HMAC_SHA256_H

#include "my_qt_common.h"
#include "sha256.h"

typedef struct
{
    sha256_ctx_t ctx;
    u8 buffer[SHA256_BLOCK_SIZE * 2];
} hmac_sha256_ctx_t;

typedef struct
{
    sha256_hash_t hash;
} hmac_sha256_hash_t;


hmac_sha256_ctx_t * hmac_sha256_create();
hmac_sha256_hash_t * hmac_sha256_hash_create();

void hmac_sha256_start(hmac_sha256_ctx_t * ctx, const u8 * key, size_t key_len);

void hmac_sha256_update(hmac_sha256_ctx_t * ctx, const u8 * input, size_t inlen);

void hmac_sha256_final(hmac_sha256_ctx_t * ctx, hmac_sha256_hash_t * digest);

void hmac_sha256_delete(hmac_sha256_ctx_t ** ctx);
void hmac_sha256_hash_delete(hmac_sha256_hash_t ** hash);

void hmac_sha256(const u8 * key, size_t key_len, const u8 * input, size_t inlen, hmac_sha256_hash_t * digest);

#endif // HMAC_SHA256_H
