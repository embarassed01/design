#ifndef HMAC_MD5_H
#define HMAC_MD5_H

#include "my_qt_common.h"
#include "md5.h"

typedef struct
{
    md5_ctx_t ctx;
    u8 buffer[MD5_BLOCK_SIZE * 2];
} hmac_md5_ctx_t;

typedef struct
{
    md5_hash_t hash;
} hmac_md5_hash_t;


hmac_md5_ctx_t * hmac_md5_create();
hmac_md5_hash_t * hmac_md5_hash_create();

void hmac_md5_start(hmac_md5_ctx_t * ctx, const u8 * key, size_t key_len);

void hmac_md5_update(hmac_md5_ctx_t * ctx, const u8 * input, size_t inlen);

void hmac_md5_final(hmac_md5_ctx_t * ctx, hmac_md5_hash_t * hash);

void hmac_md5_delete(hmac_md5_ctx_t ** ctx);
void hmac_md5_hash_delete(hmac_md5_hash_t ** hash);

void hmac_md5(const u8 * key, size_t key_len, const u8 * input, size_t inlen, hmac_md5_hash_t * hash);


#endif // HMAC_MD5_H
