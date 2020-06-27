#include "pbkdf2_sha512.h"
#include "hmac_sha512.h"

void pbkdf2_sha512(const u8 * key, size_t key_len, const u8 * input, size_t inlen, u64 iters, u8 * output, size_t outlen)
{
    if (outlen > 0x1fffffffe0ULL)
    {
        return; /* LCOV_EXCL_LINE */
    }
    // 1.
    hmac_sha512_ctx_t init_ctx, ctx;
    hmac_sha512_hash_t hash, tmphash;
    hmac_sha512_start(&init_ctx, key, key_len);
    hmac_sha512_update(&init_ctx, input, inlen);
    u8 ivec[4] = {0x00};
    int blocklen = 0;
    // 2.
    for (int i = 0; i * sizeof(hmac_sha512_hash_t) < outlen; i++)
    {
        // e.g. 1 == 00 00 00 01 -->big endian--> 01 00 00 00 == 0x1000000
        store32_be(ivec, (u32)(i+1));
        // ensure: same initstate{hmac_sha512(key, input)} / every time
        memcpy((void*)&ctx, (void*)&init_ctx, sizeof(hmac_sha512_ctx_t));
        // tmphash = hmac_sha512(ivec)
        hmac_sha512_update(&ctx, ivec, 4);
        hmac_sha512_final(&ctx, &tmphash);
        // tmphash --> hash
        memcpy((void*)&hash, (void*)&tmphash, sizeof(hmac_sha512_hash_t));
        // other iters: (iters-1)
        for (u64 j = 2; j <= iters; j++)
        {
            // tmphash = hmac_sha512(key, tmphash)
            hmac_sha512_start(&ctx, key, key_len);
            hmac_sha512_update(&ctx, tmphash.hash.bytes, sizeof(hmac_sha512_hash_t));
            hmac_sha512_final(&ctx, &tmphash);
            // hash ^= tmphash
            for (int k = 0; k < sizeof(hmac_sha512_hash_t); k++)
            {
                hash.hash.bytes[k] ^= tmphash.hash.bytes[k];
            }
        }
        // hash --blocklen--> output
        blocklen = outlen - i * sizeof(hmac_sha512_hash_t);
        blocklen = blocklen > sizeof(hmac_sha512_hash_t) ? sizeof(hmac_sha512_hash_t) : blocklen;
        memcpy(output+i*sizeof(hmac_sha512_hash_t), hash.hash.bytes, blocklen);
    }
}

