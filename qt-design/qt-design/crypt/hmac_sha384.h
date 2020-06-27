#ifndef HMAC_SHA384_H
#define HMAC_SHA384_H

#include "my_qt_common.h"
#include "sha384.h"

typedef struct
{
    sha384_ctx_t ctx;
    u8 buffer[SHA384_BLOCK_SIZE * 2];
} hmac_sha384_ctx_t;

typedef struct
{
    sha384_hash_t hash;
} hmac_sha384_hash_t;


hmac_sha384_ctx_t * hmac_sha384_create();
hmac_sha384_hash_t * hmac_sha384_hash_create();

void hmac_sha384_start(hmac_sha384_ctx_t * ctx, const u8 * key, size_t key_len);

void hmac_sha384_update(hmac_sha384_ctx_t * ctx, const u8 * input, size_t inlen);

void hmac_sha384_final(hmac_sha384_ctx_t * ctx, hmac_sha384_hash_t * digest);

void hmac_sha384_delete(hmac_sha384_ctx_t ** ctx);
void hmac_sha384_hash_delete(hmac_sha384_hash_t ** hash);

void hmac_sha384(const u8 * key, size_t key_len, const u8 * input, size_t inlen, hmac_sha384_hash_t * digest);

#endif // HMAC_SHA384_H
