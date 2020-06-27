#include "sha512.h"

#define  UL64(x)   x##ULL

/*
 * 64-bit integer manipulation macros (big endian)
 */
#ifndef GET_UINT64_BE
#define GET_UINT64_BE(n,b,i)                            \
{                                                       \
    (n) = ( (uint64_t) (b)[(i)    ] << 56 )       \
        | ( (uint64_t) (b)[(i) + 1] << 48 )       \
        | ( (uint64_t) (b)[(i) + 2] << 40 )       \
        | ( (uint64_t) (b)[(i) + 3] << 32 )       \
        | ( (uint64_t) (b)[(i) + 4] << 24 )       \
        | ( (uint64_t) (b)[(i) + 5] << 16 )       \
        | ( (uint64_t) (b)[(i) + 6] <<  8 )       \
        | ( (uint64_t) (b)[(i) + 7]       );      \
}
#endif /* GET_UINT64_BE */

#ifndef PUT_UINT64_BE
#define PUT_UINT64_BE(n,b,i)                            \
{                                                       \
    (b)[(i)    ] = (unsigned char) ( (n) >> 56 );       \
    (b)[(i) + 1] = (unsigned char) ( (n) >> 48 );       \
    (b)[(i) + 2] = (unsigned char) ( (n) >> 40 );       \
    (b)[(i) + 3] = (unsigned char) ( (n) >> 32 );       \
    (b)[(i) + 4] = (unsigned char) ( (n) >> 24 );       \
    (b)[(i) + 5] = (unsigned char) ( (n) >> 16 );       \
    (b)[(i) + 6] = (unsigned char) ( (n) >>  8 );       \
    (b)[(i) + 7] = (unsigned char) ( (n)       );       \
}
#endif /* PUT_UINT64_BE */


void sha512_start(sha512_ctx_t * ctx)
{
    ctx->total[0] = 0;
    ctx->total[1] = 0;
    /* SHA-512 */
    ctx->state[0] = UL64(0x6A09E667F3BCC908);
    ctx->state[1] = UL64(0xBB67AE8584CAA73B);
    ctx->state[2] = UL64(0x3C6EF372FE94F82B);
    ctx->state[3] = UL64(0xA54FF53A5F1D36F1);
    ctx->state[4] = UL64(0x510E527FADE682D1);
    ctx->state[5] = UL64(0x9B05688C2B3E6C1F);
    ctx->state[6] = UL64(0x1F83D9ABFB41BD6B);
    ctx->state[7] = UL64(0x5BE0CD19137E2179);
}

/*
 * Round constants
 */
static const uint64_t K[80] =
{
    UL64(0x428A2F98D728AE22),  UL64(0x7137449123EF65CD),
    UL64(0xB5C0FBCFEC4D3B2F),  UL64(0xE9B5DBA58189DBBC),
    UL64(0x3956C25BF348B538),  UL64(0x59F111F1B605D019),
    UL64(0x923F82A4AF194F9B),  UL64(0xAB1C5ED5DA6D8118),
    UL64(0xD807AA98A3030242),  UL64(0x12835B0145706FBE),
    UL64(0x243185BE4EE4B28C),  UL64(0x550C7DC3D5FFB4E2),
    UL64(0x72BE5D74F27B896F),  UL64(0x80DEB1FE3B1696B1),
    UL64(0x9BDC06A725C71235),  UL64(0xC19BF174CF692694),
    UL64(0xE49B69C19EF14AD2),  UL64(0xEFBE4786384F25E3),
    UL64(0x0FC19DC68B8CD5B5),  UL64(0x240CA1CC77AC9C65),
    UL64(0x2DE92C6F592B0275),  UL64(0x4A7484AA6EA6E483),
    UL64(0x5CB0A9DCBD41FBD4),  UL64(0x76F988DA831153B5),
    UL64(0x983E5152EE66DFAB),  UL64(0xA831C66D2DB43210),
    UL64(0xB00327C898FB213F),  UL64(0xBF597FC7BEEF0EE4),
    UL64(0xC6E00BF33DA88FC2),  UL64(0xD5A79147930AA725),
    UL64(0x06CA6351E003826F),  UL64(0x142929670A0E6E70),
    UL64(0x27B70A8546D22FFC),  UL64(0x2E1B21385C26C926),
    UL64(0x4D2C6DFC5AC42AED),  UL64(0x53380D139D95B3DF),
    UL64(0x650A73548BAF63DE),  UL64(0x766A0ABB3C77B2A8),
    UL64(0x81C2C92E47EDAEE6),  UL64(0x92722C851482353B),
    UL64(0xA2BFE8A14CF10364),  UL64(0xA81A664BBC423001),
    UL64(0xC24B8B70D0F89791),  UL64(0xC76C51A30654BE30),
    UL64(0xD192E819D6EF5218),  UL64(0xD69906245565A910),
    UL64(0xF40E35855771202A),  UL64(0x106AA07032BBD1B8),
    UL64(0x19A4C116B8D2D0C8),  UL64(0x1E376C085141AB53),
    UL64(0x2748774CDF8EEB99),  UL64(0x34B0BCB5E19B48A8),
    UL64(0x391C0CB3C5C95A63),  UL64(0x4ED8AA4AE3418ACB),
    UL64(0x5B9CCA4F7763E373),  UL64(0x682E6FF3D6B2B8A3),
    UL64(0x748F82EE5DEFB2FC),  UL64(0x78A5636F43172F60),
    UL64(0x84C87814A1F0AB72),  UL64(0x8CC702081A6439EC),
    UL64(0x90BEFFFA23631E28),  UL64(0xA4506CEBDE82BDE9),
    UL64(0xBEF9A3F7B2C67915),  UL64(0xC67178F2E372532B),
    UL64(0xCA273ECEEA26619C),  UL64(0xD186B8C721C0C207),
    UL64(0xEADA7DD6CDE0EB1E),  UL64(0xF57D4F7FEE6ED178),
    UL64(0x06F067AA72176FBA),  UL64(0x0A637DC5A2C898A6),
    UL64(0x113F9804BEF90DAE),  UL64(0x1B710B35131C471B),
    UL64(0x28DB77F523047D84),  UL64(0x32CAAB7B40C72493),
    UL64(0x3C9EBE0A15C9BEBC),  UL64(0x431D67C49C100D4C),
    UL64(0x4CC5D4BECB3E42B6),  UL64(0x597F299CFC657E2A),
    UL64(0x5FCB6FAB3AD6FAEC),  UL64(0x6C44198C4A475817)
};

int internal_sha512_process(sha512_ctx_t * ctx, const u8 data[128])
{
    int i;
    uint64_t temp1, temp2, W[80];
    uint64_t A, B, C, D, E, F, G, H;

#define  SHR(x,n) ((x) >> (n))
#define ROTR(x,n) (SHR((x),(n)) | ((x) << (64 - (n))))

#define S0(x) (ROTR(x, 1) ^ ROTR(x, 8) ^  SHR(x, 7))
#define S1(x) (ROTR(x,19) ^ ROTR(x,61) ^  SHR(x, 6))

#define S2(x) (ROTR(x,28) ^ ROTR(x,34) ^ ROTR(x,39))
#define S3(x) (ROTR(x,14) ^ ROTR(x,18) ^ ROTR(x,41))

#define F0(x,y,z) (((x) & (y)) | ((z) & ((x) | (y))))
#define F1(x,y,z) ((z) ^ ((x) & ((y) ^ (z))))

#define P(a,b,c,d,e,f,g,h,x,K)                                  \
    do                                                          \
    {                                                           \
        temp1 = (h) + S3(e) + F1((e),(f),(g)) + (K) + (x);      \
        temp2 = S2(a) + F0((a),(b),(c));                        \
        (d) += temp1; (h) = temp1 + temp2;                      \
    } while (0)

    for (i = 0; i < 16; i++)
    {
        GET_UINT64_BE( W[i], data, i << 3 );
    }
    for (; i < 80; i++)
    {
        W[i] = S1(W[i -  2]) + W[i -  7] +
               S0(W[i - 15]) + W[i - 16];
    }
    A = ctx->state[0];
    B = ctx->state[1];
    C = ctx->state[2];
    D = ctx->state[3];
    E = ctx->state[4];
    F = ctx->state[5];
    G = ctx->state[6];
    H = ctx->state[7];
    i = 0;
    do
    {
        P( A, B, C, D, E, F, G, H, W[i], K[i] ); i++;
        P( H, A, B, C, D, E, F, G, W[i], K[i] ); i++;
        P( G, H, A, B, C, D, E, F, W[i], K[i] ); i++;
        P( F, G, H, A, B, C, D, E, W[i], K[i] ); i++;
        P( E, F, G, H, A, B, C, D, W[i], K[i] ); i++;
        P( D, E, F, G, H, A, B, C, W[i], K[i] ); i++;
        P( C, D, E, F, G, H, A, B, W[i], K[i] ); i++;
        P( B, C, D, E, F, G, H, A, W[i], K[i] ); i++;
    } while (i < 80);
    ctx->state[0] += A;
    ctx->state[1] += B;
    ctx->state[2] += C;
    ctx->state[3] += D;
    ctx->state[4] += E;
    ctx->state[5] += F;
    ctx->state[6] += G;
    ctx->state[7] += H;
    return 0;
}

void sha512_process(sha512_ctx_t * ctx, const u8 data[128])
{
    internal_sha512_process(ctx, data);
}

sha512_ctx_t * sha512_create()
{
    sha512_ctx_t * ctx = (sha512_ctx_t*)malloc( sizeof(sha512_ctx_t) );
    memset((void*)ctx, 0x00, sizeof(sha512_ctx_t));
    return ctx;
}

sha512_hash_t * sha512_hash_create()
{
    sha512_hash_t * hash = (sha512_hash_t*)malloc( sizeof(sha512_hash_t) );
    memset((void*)hash, 0x00, sizeof(sha512_hash_t));
    return hash;
}

void sha512_update(sha512_ctx_t * ctx, u8 * input, size_t ilen)
{
    int ret;
    size_t fill;
    unsigned int left;
    if (ilen == 0)
        return;
    left = (unsigned int) (ctx->total[0] & 0x7F);
    fill = 128 - left;
    ctx->total[0] += (uint64_t)ilen;
    if (ctx->total[0] < (uint64_t)ilen)
        ctx->total[1]++;
    if (left && ilen >= fill)
    {
        memcpy((void *)(ctx->buffer + left), input, fill);

        if ((ret = internal_sha512_process(ctx, ctx->buffer)) != 0)
            return;

        input += fill;
        ilen  -= fill;
        left = 0;
    }
    while (ilen >= 128)
    {
        if ((ret = internal_sha512_process(ctx, input)) != 0)
            return;

        input += 128;
        ilen  -= 128;
    }
    if (ilen > 0)
        memcpy((void *)(ctx->buffer + left), input, ilen);
}

void sha512_final(sha512_ctx_t * ctx, sha512_hash_t * digest)
{
    int ret;
    unsigned used;
    uint64_t high, low;
    /*
     * Add padding: 0x80 then 0x00 until 16 bytes remain for the length
     */
    used = ctx->total[0] & 0x7F;
    ctx->buffer[used++] = 0x80;
    if (used <= 112)
    {
        /* Enough room for padding + length in current block */
        memset(ctx->buffer + used, 0, 112 - used);
    }
    else
    {
        /* We'll need an extra block */
        memset(ctx->buffer + used, 0, 128 - used);
        if ((ret = internal_sha512_process(ctx, ctx->buffer)) != 0)
            return;
        memset(ctx->buffer, 0, 112);
    }
    /*
     * Add message length
     */
    high = ( ctx->total[0] >> 61 )
         | ( ctx->total[1] <<  3 );
    low  = ( ctx->total[0] <<  3 );
    PUT_UINT64_BE(high, ctx->buffer, 112);
    PUT_UINT64_BE(low,  ctx->buffer, 120);
    if ((ret = internal_sha512_process(ctx, ctx->buffer)) != 0)
        return;
    /*
     * Output final state
     */
    PUT_UINT64_BE(ctx->state[0], digest->bytes,  0);
    PUT_UINT64_BE(ctx->state[1], digest->bytes,  8);
    PUT_UINT64_BE(ctx->state[2], digest->bytes, 16);
    PUT_UINT64_BE(ctx->state[3], digest->bytes, 24);
    PUT_UINT64_BE(ctx->state[4], digest->bytes, 32);
    PUT_UINT64_BE(ctx->state[5], digest->bytes, 40);
    // sha512 add :
    PUT_UINT64_BE(ctx->state[6], digest->bytes, 48 );
    PUT_UINT64_BE(ctx->state[7], digest->bytes, 56 );
}

void sha512_delete(sha512_ctx_t ** ctx)
{
    if (*ctx != NULL)
    {
        memset((void*)*ctx, 0x00, sizeof(sha512_ctx_t));
        free((void*)*ctx);
        *ctx = NULL;
    }
}

void sha512_hash_delete(sha512_hash_t ** hash)
{
    if (*hash != NULL)
    {
        memset((void*)*hash, 0x00, sizeof(sha512_hash_t));
        free((void*)*hash);
        *hash = NULL;
    }
}

void sha512(u8 * input, size_t ilen, sha512_hash_t * digest)
{
    sha512_ctx_t * ctx = sha512_create();
    sha512_start(ctx);
    sha512_update(ctx, input, ilen);
    sha512_final(ctx, digest);
    sha512_delete(&ctx);
}

/*
 * FIPS-180-2 test vectors
 */
static const unsigned char sha512_test_buf[3][113] =
{
    { "abc" },   // 0
    { "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmn"
      "hijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu" },   // 1
    { "" }       // 3
};

static const size_t sha512_test_buflen[3] =
{
    3, 112, 1000
};

static const unsigned char sha512_test_sum[3][64] =
{
    /*
     * SHA-512 test vectors
     */
    { 0xDD, 0xAF, 0x35, 0xA1, 0x93, 0x61, 0x7A, 0xBA,
      0xCC, 0x41, 0x73, 0x49, 0xAE, 0x20, 0x41, 0x31,
      0x12, 0xE6, 0xFA, 0x4E, 0x89, 0xA9, 0x7E, 0xA2,
      0x0A, 0x9E, 0xEE, 0xE6, 0x4B, 0x55, 0xD3, 0x9A,
      0x21, 0x92, 0x99, 0x2A, 0x27, 0x4F, 0xC1, 0xA8,
      0x36, 0xBA, 0x3C, 0x23, 0xA3, 0xFE, 0xEB, 0xBD,
      0x45, 0x4D, 0x44, 0x23, 0x64, 0x3C, 0xE8, 0x0E,
      0x2A, 0x9A, 0xC9, 0x4F, 0xA5, 0x4C, 0xA4, 0x9F },
    { 0x8E, 0x95, 0x9B, 0x75, 0xDA, 0xE3, 0x13, 0xDA,
      0x8C, 0xF4, 0xF7, 0x28, 0x14, 0xFC, 0x14, 0x3F,
      0x8F, 0x77, 0x79, 0xC6, 0xEB, 0x9F, 0x7F, 0xA1,
      0x72, 0x99, 0xAE, 0xAD, 0xB6, 0x88, 0x90, 0x18,
      0x50, 0x1D, 0x28, 0x9E, 0x49, 0x00, 0xF7, 0xE4,
      0x33, 0x1B, 0x99, 0xDE, 0xC4, 0xB5, 0x43, 0x3A,
      0xC7, 0xD3, 0x29, 0xEE, 0xB6, 0xDD, 0x26, 0x54,
      0x5E, 0x96, 0xE5, 0x5B, 0x87, 0x4B, 0xE9, 0x09 },
    { 0xE7, 0x18, 0x48, 0x3D, 0x0C, 0xE7, 0x69, 0x64,
      0x4E, 0x2E, 0x42, 0xC7, 0xBC, 0x15, 0xB4, 0x63,
      0x8E, 0x1F, 0x98, 0xB1, 0x3B, 0x20, 0x44, 0x28,
      0x56, 0x32, 0xA8, 0x03, 0xAF, 0xA9, 0x73, 0xEB,
      0xDE, 0x0F, 0xF2, 0x44, 0x87, 0x7E, 0xA6, 0x0A,
      0x4C, 0xB0, 0x43, 0x2C, 0xE5, 0x77, 0xC3, 0x1B,
      0xEB, 0x00, 0x9C, 0x5C, 0x2C, 0x49, 0xAA, 0x2E,
      0x4E, 0xAD, 0xB2, 0x17, 0xAD, 0x8C, 0xC0, 0x9B }
};

/*
 * Checkup routine
 */
void sha512_self_test()
{
    u8 buf[1024] = {0x00};
    sha512_ctx_t * ctx = sha512_create();
    sha512_hash_t * hash = sha512_hash_create();
    for(int i = 0; i < 3; i++ )
    {
        int j = i % 3;
        sha512_start(ctx);
        if (j == 2)
        {
            memset(buf, 'a', 1000);

            for( j = 0; j < 1000; j++ )
            {
                sha512_update(ctx, buf, 1000);
            }
        }
        else
        {
            sha512_update(ctx, (u8*)sha512_test_buf[j], sha512_test_buflen[j]);
        }
        sha512_final(ctx, hash);
        if (memcmp(hash->bytes, sha512_test_sum[i], SHA512_HASH_SIZE) != 0)
        {
            printf( "failed\n" );
        }
        else
        {
            printf( "passed\n" );
        }
    }
    sha512_hash_delete(&hash);
    sha512_delete(&ctx);
}
