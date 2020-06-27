#ifndef HMAC_SHA1_H
#define HMAC_SHA1_H

#include "my_qt_common.h"
#include "sha1.h"

typedef struct
{
    sha1_ctx_t ctx;
    u8 buffer[SHA1_BLOCK_SIZE * 2];
} hmac_sha1_ctx_t;

typedef struct
{
    sha1_hash_t hash;
} hmac_sha1_hash_t;


hmac_sha1_ctx_t * hmac_sha1_create();
hmac_sha1_hash_t * hmac_sha1_hash_create();

void hmac_sha1_start(hmac_sha1_ctx_t * ctx, const u8 * key, size_t key_len);

void hmac_sha1_update(hmac_sha1_ctx_t * ctx, const u8 * input, size_t inlen);

void hmac_sha1_final(hmac_sha1_ctx_t * ctx, hmac_sha1_hash_t * digest);

void hmac_sha1_delete(hmac_sha1_ctx_t ** ctx);
void hmac_sha1_hash_delete(hmac_sha1_hash_t ** hash);

void hmac_sha1(const u8 * key, size_t key_len, const u8 * input, size_t inlen, hmac_sha1_hash_t * digest);

#endif // HMAC_SHA1_H
