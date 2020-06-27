#include "hmac_md4.h"

hmac_md4_ctx_t * hmac_md4_create()
{
    hmac_md4_ctx_t * ctx = (hmac_md4_ctx_t*)malloc( sizeof(hmac_md4_ctx_t) );
    memset((void*)ctx, 0x00, sizeof(hmac_md4_ctx_t));
    return ctx;
}

hmac_md4_hash_t * hmac_md4_hash_create()
{
    hmac_md4_hash_t * hash = (hmac_md4_hash_t*)malloc( sizeof(hmac_md4_hash_t) );
    memset((void*)hash, 0x00, sizeof(hmac_md4_hash_t));
    return hash;
}

void hmac_md4_start(hmac_md4_ctx_t * ctx, const u8 * key, size_t key_len)
{
    md4_hash_t md4hash;
    if ( key_len > sizeof(ctx->ctx.buffer) )
    {
        md4_start(&ctx->ctx);
        md4_update(&ctx->ctx, key, key_len);
        md4_final(&ctx->ctx, &md4hash);
        key_len = sizeof(md4hash);
        key = md4hash.bytes;
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
    md4_start(&ctx->ctx);
    md4_update(&ctx->ctx, ipad, sizeof(ctx->ctx.buffer));
}

void hmac_md4_update(hmac_md4_ctx_t * ctx, const u8 * input, size_t inlen)
{
    md4_update(&ctx->ctx, input, inlen);
}

void hmac_md4_final(hmac_md4_ctx_t * ctx, hmac_md4_hash_t * hash)
{
    md4_hash_t md4hash;
    u8 * opad = ctx->buffer + sizeof(ctx->ctx.buffer);
    md4_final(&ctx->ctx, &md4hash);
    md4_start(&ctx->ctx);
    md4_update(&ctx->ctx, opad, sizeof(ctx->ctx.buffer));
    md4_update(&ctx->ctx, md4hash.bytes, sizeof(md4hash));
    md4_final(&ctx->ctx, &md4hash);
    memcpy(hash->hash.bytes, md4hash.bytes, sizeof(md4hash));
}

void hmac_md4_delete(hmac_md4_ctx_t ** ctx)
{
    if (*ctx != NULL)
    {
        memset((void*)*ctx, 0x00, sizeof(hmac_md4_ctx_t));
        free((void*)*ctx);
        *ctx = NULL;
    }
}

void hmac_md4_hash_delete(hmac_md4_hash_t ** hash)
{
    if (*hash != NULL)
    {
        memset((void*)*hash, 0x00, sizeof(hmac_md4_hash_t));
        free((void*)*hash);
        *hash = NULL;
    }
}

void hmac_md4(const u8 * key, size_t key_len, const u8 * input, size_t inlen, hmac_md4_hash_t * hash)
{
    hmac_md4_ctx_t ctx;
    hmac_md4_start(&ctx, key, key_len);
    hmac_md4_update(&ctx, input, inlen);
    hmac_md4_final(&ctx, hash);
}

