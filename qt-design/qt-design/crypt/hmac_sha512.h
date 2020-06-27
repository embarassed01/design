#ifndef HMAC_SHA512_H
#define HMAC_SHA512_H

#include "my_qt_common.h"
#include "sha512.h"

typedef struct
{
    sha512_ctx_t ctx;
    u8 buffer[SHA512_BLOCK_SIZE * 2];
} hmac_sha512_ctx_t;

typedef struct
{
    sha512_hash_t hash;
} hmac_sha512_hash_t;


hmac_sha512_ctx_t * hmac_sha512_create();
hmac_sha512_hash_t * hmac_sha512_hash_create();

void hmac_sha512_start(hmac_sha512_ctx_t * ctx, const u8 * key, size_t key_len);

void hmac_sha512_update(hmac_sha512_ctx_t * ctx, const u8 * input, size_t inlen);

void hmac_sha512_final(hmac_sha512_ctx_t * ctx, hmac_sha512_hash_t * digest);

void hmac_sha512_delete(hmac_sha512_ctx_t ** ctx);
void hmac_sha512_hash_delete(hmac_sha512_hash_t ** hash);

void hmac_sha512(const u8 * key, size_t key_len, const u8 * input, size_t inlen, hmac_sha512_hash_t * digest);

#endif // HMAC_SHA512_H
