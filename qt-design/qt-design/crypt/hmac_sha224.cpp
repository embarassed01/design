#include "hmac_sha224.h"

hmac_sha224_ctx_t * hmac_sha224_create()
{
    hmac_sha224_ctx_t * ctx = (hmac_sha224_ctx_t*)malloc( sizeof(hmac_sha224_ctx_t) );
    memset((void*)ctx, 0x00, sizeof(hmac_sha224_ctx_t));
    return ctx;
}

hmac_sha224_hash_t * hmac_sha224_hash_create()
{
    hmac_sha224_hash_t * hash = (hmac_sha224_hash_t*)malloc( sizeof(hmac_sha224_hash_t) );
    memset((void*)hash, 0x00, sizeof(hmac_sha224_hash_t));
    return hash;
}

void hmac_sha224_start(hmac_sha224_ctx_t * ctx, const u8 * key, size_t key_len)
{
    sha224_hash_t hash;
    if ( key_len > sizeof(ctx->ctx.buffer) )
    {
        sha224_start(&ctx->ctx);
        sha224_update(&ctx->ctx, (u8*)key, key_len);
        sha224_final(&ctx->ctx, &hash);
        key_len = sizeof(hash);
        key = hash.bytes;
    }
    u8 * ipad = ctx->buffer;
    u8 * opad = ctx->buffer + sizeof(ctx->ctx.buffer);  // + MD5_BLOCK_SIZE
    memset(ipad, 0x36, sizeof(ctx->ctx.buffer));
    memset(opad, 0x5C, sizeof(ctx->ctx.buffer));
    for (int i = 0; i < key_len; i++)
    {
        ipad[i] = ipad[i] ^ key[i];
        opad[i] = opad[i] ^ key[i];
    }
    sha224_start(&ctx->ctx);
    sha224_update(&ctx->ctx, ipad, sizeof(ctx->ctx.buffer));
}

void hmac_sha224_update(hmac_sha224_ctx_t * ctx, const u8 * input, size_t inlen)
{
    sha224_update(&ctx->ctx, (u8*)input, inlen);
}

void hmac_sha224_final(hmac_sha224_ctx_t * ctx, hmac_sha224_hash_t * digest)
{
    sha224_hash_t hash;
    u8 * opad = ctx->buffer + sizeof(ctx->ctx.buffer);
    sha224_final(&ctx->ctx, &hash);
    sha224_start(&ctx->ctx);
    sha224_update(&ctx->ctx, opad, sizeof(ctx->ctx.buffer));
    sha224_update(&ctx->ctx, hash.bytes, sizeof(hash));
    sha224_final(&ctx->ctx, &hash);
    memcpy(digest->hash.bytes, hash.bytes, sizeof(hash));
}

void hmac_sha224_delete(hmac_sha224_ctx_t ** ctx)
{
    if (*ctx != NULL)
    {
        memset((void*)*ctx, 0x00, sizeof(hmac_sha224_ctx_t));
        free((void*)*ctx);
        *ctx = NULL;
    }
}

void hmac_sha224_hash_delete(hmac_sha224_hash_t ** hash)
{
    if (*hash != NULL)
    {
        memset((void*)*hash, 0x00, sizeof(hmac_sha224_hash_t));
        free((void*)*hash);
        *hash = NULL;
    }
}

void hmac_sha224(const u8 * key, size_t key_len, const u8 * input, size_t inlen, hmac_sha224_hash_t * digest)
{
    hmac_sha224_ctx_t ctx;
    hmac_sha224_start(&ctx, key, key_len);
    hmac_sha224_update(&ctx, input, inlen);
    hmac_sha224_final(&ctx, digest);
}

