#include "hmac_md5.h"

hmac_md5_ctx_t * hmac_md5_create()
{
    hmac_md5_ctx_t * ctx = (hmac_md5_ctx_t*)malloc( sizeof(hmac_md5_ctx_t) );
    memset((void*)ctx, 0x00, sizeof(hmac_md5_ctx_t));
    return ctx;
}

hmac_md5_hash_t * hmac_md5_hash_create()
{
    hmac_md5_hash_t * hash = (hmac_md5_hash_t*)malloc( sizeof(hmac_md5_hash_t) );
    memset((void*)hash, 0x00, sizeof(hmac_md5_hash_t));
    return hash;
}

void hmac_md5_start(hmac_md5_ctx_t * ctx, const u8 * key, size_t key_len)
{
    md5_hash_t md5hash;
    if ( key_len > sizeof(ctx->ctx.buffer) )
    {
        md5_start(&ctx->ctx);
        md5_update(&ctx->ctx, key, key_len);
        md5_final(&ctx->ctx, &md5hash);
        key_len = sizeof(md5hash);
        key = md5hash.bytes;
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
    md5_start(&ctx->ctx);
    md5_update(&ctx->ctx, ipad, sizeof(ctx->ctx.buffer));
}

void hmac_md5_update(hmac_md5_ctx_t * ctx, const u8 * input, size_t inlen)
{
    md5_update(&ctx->ctx, input, inlen);
}

void hmac_md5_final(hmac_md5_ctx_t * ctx, hmac_md5_hash_t * hash)
{
    md5_hash_t md5hash;
    u8 * opad = ctx->buffer + sizeof(ctx->ctx.buffer);
    md5_final(&ctx->ctx, &md5hash);
    md5_start(&ctx->ctx);
    md5_update(&ctx->ctx, opad, sizeof(ctx->ctx.buffer));
    md5_update(&ctx->ctx, md5hash.bytes, sizeof(md5hash));
    md5_final(&ctx->ctx, &md5hash);
    memcpy(hash->hash.bytes, md5hash.bytes, sizeof(md5hash));
}

void hmac_md5_delete(hmac_md5_ctx_t ** ctx)
{
    if (*ctx != NULL)
    {
        memset((void*)*ctx, 0x00, sizeof(hmac_md5_ctx_t));
        free((void*)*ctx);
        *ctx = NULL;
    }
}

void hmac_md5_hash_delete(hmac_md5_hash_t ** hash)
{
    if (*hash != NULL)
    {
        memset((void*)*hash, 0x00, sizeof(hmac_md5_hash_t));
        free((void*)*hash);
        *hash = NULL;
    }
}

void hmac_md5(const u8 * key, size_t key_len, const u8 * input, size_t inlen, hmac_md5_hash_t * hash)
{
    hmac_md5_ctx_t ctx;
    hmac_md5_start(&ctx, key, key_len);
    hmac_md5_update(&ctx, input, inlen);
    hmac_md5_final(&ctx, hash);
}

