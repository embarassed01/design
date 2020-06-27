#include "sha256.h"

/*
 * 32-bit integer manipulation macros (big endian)
 */
#ifndef GET_UINT32_BE
#define GET_UINT32_BE(n,b,i)                            \
do {                                                    \
    (n) = ( (uint32_t) (b)[(i)    ] << 24 )             \
        | ( (uint32_t) (b)[(i) + 1] << 16 )             \
        | ( (uint32_t) (b)[(i) + 2] <<  8 )             \
        | ( (uint32_t) (b)[(i) + 3]       );            \
} while( 0 )
#endif

#ifndef PUT_UINT32_BE
#define PUT_UINT32_BE(n,b,i)                            \
do {                                                    \
    (b)[(i)    ] = (unsigned char) ( (n) >> 24 );       \
    (b)[(i) + 1] = (unsigned char) ( (n) >> 16 );       \
    (b)[(i) + 2] = (unsigned char) ( (n) >>  8 );       \
    (b)[(i) + 3] = (unsigned char) ( (n)       );       \
} while( 0 )
#endif

static const u32 K[] =
{
    0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5,
    0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
    0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3,
    0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
    0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC,
    0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
    0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7,
    0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
    0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13,
    0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
    0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3,
    0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
    0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5,
    0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
    0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208,
    0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2,
};

#define  SHR(x,n) (((x) & 0xFFFFFFFF) >> (n))
#define ROTR(x,n) (SHR(x,n) | ((x) << (32 - (n))))

#define S0(x) (ROTR(x, 7) ^ ROTR(x,18) ^  SHR(x, 3))
#define S1(x) (ROTR(x,17) ^ ROTR(x,19) ^  SHR(x,10))

#define S2(x) (ROTR(x, 2) ^ ROTR(x,13) ^ ROTR(x,22))
#define S3(x) (ROTR(x, 6) ^ ROTR(x,11) ^ ROTR(x,25))

#define F0(x,y,z) (((x) & (y)) | ((z) & ((x) | (y))))
#define F1(x,y,z) ((z) ^ ((x) & ((y) ^ (z))))

#define R(t)                                    \
    (                                           \
        W[t] = S1(W[(t) -  2]) + W[(t) -  7] +  \
               S0(W[(t) - 15]) + W[(t) - 16]    \
    )

#define P(a,b,c,d,e,f,g,h,x,K)                          \
    do                                                  \
    {                                                   \
        temp1 = (h) + S3(e) + F1((e),(f),(g)) + (K) + (x);      \
        temp2 = S2(a) + F0((a),(b),(c));                        \
        (d) += temp1; (h) = temp1 + temp2;              \
    } while( 0 )


void internal_sha256_process(sha256_ctx_t * ctx, const u8 data[64])
{
    uint32_t temp1, temp2, W[64];
    uint32_t A[8];
    unsigned int i;
    for ( i = 0; i < 8; i++ )
    {
        A[i] = ctx->state[i];
    }
    for ( i = 0; i < 16; i++ )
    {
       GET_UINT32_BE( W[i], data, 4 * i );
    }
    for ( i = 0; i < 16; i += 8 )
    {
        P( A[0], A[1], A[2], A[3], A[4], A[5], A[6], A[7], W[i+0], K[i+0] );
        P( A[7], A[0], A[1], A[2], A[3], A[4], A[5], A[6], W[i+1], K[i+1] );
        P( A[6], A[7], A[0], A[1], A[2], A[3], A[4], A[5], W[i+2], K[i+2] );
        P( A[5], A[6], A[7], A[0], A[1], A[2], A[3], A[4], W[i+3], K[i+3] );
        P( A[4], A[5], A[6], A[7], A[0], A[1], A[2], A[3], W[i+4], K[i+4] );
        P( A[3], A[4], A[5], A[6], A[7], A[0], A[1], A[2], W[i+5], K[i+5] );
        P( A[2], A[3], A[4], A[5], A[6], A[7], A[0], A[1], W[i+6], K[i+6] );
        P( A[1], A[2], A[3], A[4], A[5], A[6], A[7], A[0], W[i+7], K[i+7] );
    }
    for ( i = 16; i < 64; i += 8 )
    {
        P( A[0], A[1], A[2], A[3], A[4], A[5], A[6], A[7], R(i+0), K[i+0] );
        P( A[7], A[0], A[1], A[2], A[3], A[4], A[5], A[6], R(i+1), K[i+1] );
        P( A[6], A[7], A[0], A[1], A[2], A[3], A[4], A[5], R(i+2), K[i+2] );
        P( A[5], A[6], A[7], A[0], A[1], A[2], A[3], A[4], R(i+3), K[i+3] );
        P( A[4], A[5], A[6], A[7], A[0], A[1], A[2], A[3], R(i+4), K[i+4] );
        P( A[3], A[4], A[5], A[6], A[7], A[0], A[1], A[2], R(i+5), K[i+5] );
        P( A[2], A[3], A[4], A[5], A[6], A[7], A[0], A[1], R(i+6), K[i+6] );
        P( A[1], A[2], A[3], A[4], A[5], A[6], A[7], A[0], R(i+7), K[i+7] );
    }
    for ( i = 0; i < 8; i++ )
    {
        ctx->state[i] += A[i];
    }
}

void sha256_process(sha256_ctx_t * ctx, const u8 data[64])
{
    internal_sha256_process( ctx, data );
}

sha256_ctx_t * sha256_create()
{
    sha256_ctx_t * ctx = (sha256_ctx_t*)malloc( sizeof(sha256_ctx_t) );
    memset((void*)ctx, 0x00, sizeof(sha256_ctx_t));
    return ctx;
}

sha256_hash_t * sha256_hash_create()
{
    sha256_hash_t * hash = (sha256_hash_t*)malloc( sizeof(sha256_hash_t) );
    memset((void*)hash, 0x00, sizeof(sha256_hash_t));
    return hash;
}

void sha256_start(sha256_ctx_t * ctx)
{
    ctx->total[0] = 0;
    ctx->total[1] = 0;
    /* SHA-256 */
    ctx->state[0] = 0x6A09E667;
    ctx->state[1] = 0xBB67AE85;
    ctx->state[2] = 0x3C6EF372;
    ctx->state[3] = 0xA54FF53A;
    ctx->state[4] = 0x510E527F;
    ctx->state[5] = 0x9B05688C;
    ctx->state[6] = 0x1F83D9AB;
    ctx->state[7] = 0x5BE0CD19;
}

void sha256_update(sha256_ctx_t * ctx, u8 * buffer, size_t buffer_size)
{
    int ret;
    size_t fill;
    uint32_t left;
    if ( buffer_size == 0 )
    {
        return;
    }
    left = ctx->total[0] & 0x3F;
    fill = 64 - left;
    ctx->total[0] += (uint32_t) buffer_size;
    ctx->total[0] &= 0xFFFFFFFF;
    if ( ctx->total[0] < (uint32_t) buffer_size )
    {
        ctx->total[1]++;
    }
    if ( left && buffer_size >= fill )
    {
        memcpy((void *) (ctx->buffer + left), buffer, fill);
        internal_sha256_process(ctx, ctx->buffer);
        buffer += fill;
        buffer_size  -= fill;
        left = 0;
    }
    while ( buffer_size >= 64 )
    {
        internal_sha256_process(ctx, buffer);
        buffer += 64;
        buffer_size  -= 64;
    }
    if ( buffer_size > 0 )
    {
        memcpy((void *)(ctx->buffer + left), buffer, buffer_size);
    }
}

void sha256_final(sha256_ctx_t * ctx, sha256_hash_t * digest)
{
    int ret;
    uint32_t used;
    uint32_t high, low;

    /*
     * Add padding: 0x80 then 0x00 until 8 bytes remain for the length
    */
    used = ctx->total[0] & 0x3F;
    ctx->buffer[used++] = 0x80;

    if ( used <= 56 )
    {
        /* Enough room for padding + length in current block */
        memset(ctx->buffer + used, 0, 56 - used);
    }
    else
    {
        /* We'll need an extra block */
        memset(ctx->buffer + used, 0, 64 - used);
        internal_sha256_process(ctx, ctx->buffer);
        memset(ctx->buffer, 0, 56);
    }
    /*
     * Add message length
     */
    high = ( ctx->total[0] >> 29 ) | ( ctx->total[1] <<  3 );
    low  = ( ctx->total[0] <<  3 );
    PUT_UINT32_BE( high, ctx->buffer, 56 );
    PUT_UINT32_BE( low,  ctx->buffer, 60 );
    internal_sha256_process(ctx, ctx->buffer);
    /*
     * Output final state
     */
    PUT_UINT32_BE( ctx->state[0], digest->bytes,  0 );
    PUT_UINT32_BE( ctx->state[1], digest->bytes,  4 );
    PUT_UINT32_BE( ctx->state[2], digest->bytes,  8 );
    PUT_UINT32_BE( ctx->state[3], digest->bytes, 12 );
    PUT_UINT32_BE( ctx->state[4], digest->bytes, 16 );
    PUT_UINT32_BE( ctx->state[5], digest->bytes, 20 );
    PUT_UINT32_BE( ctx->state[6], digest->bytes, 24 );
    PUT_UINT32_BE( ctx->state[7], digest->bytes, 28 );
}

void sha256_delete(sha256_ctx_t ** ctx)
{
    if (*ctx != NULL)
    {
        memset((void*)*ctx, 0x00, sizeof(sha256_ctx_t));
        free((void*)*ctx);
        *ctx = NULL;
    }
}

void sha256_hash_delete(sha256_hash_t ** hash)
{
    if (*hash != NULL)
    {
        memset((void*)*hash, 0x00, sizeof(sha256_hash_t));
        free((void*)*hash);
        *hash = NULL;
    }
}

void sha256(u8 * buffer, size_t buffer_size, sha256_hash_t * digest)
{
    sha256_ctx_t * ctx = sha256_create();
    sha256_start(ctx);
    sha256_update(ctx, buffer, buffer_size);
    sha256_final(ctx, digest);
    sha256_delete(&ctx);
}

////////////////////////////////////////////////////////////////////////////////
static const unsigned char sha256_test_buf[3][57] =
{
    { "abc" },
    { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq" },
    { "" }
};

static const size_t sha256_test_buflen[3] =
{
    3, 56, 1000
};

static const unsigned char sha256_test_sum[3][32] =
{
    /*
     * SHA-256 test vectors
     */
    { 0xBA, 0x78, 0x16, 0xBF, 0x8F, 0x01, 0xCF, 0xEA,
      0x41, 0x41, 0x40, 0xDE, 0x5D, 0xAE, 0x22, 0x23,
      0xB0, 0x03, 0x61, 0xA3, 0x96, 0x17, 0x7A, 0x9C,
      0xB4, 0x10, 0xFF, 0x61, 0xF2, 0x00, 0x15, 0xAD },
    { 0x24, 0x8D, 0x6A, 0x61, 0xD2, 0x06, 0x38, 0xB8,
      0xE5, 0xC0, 0x26, 0x93, 0x0C, 0x3E, 0x60, 0x39,
      0xA3, 0x3C, 0xE4, 0x59, 0x64, 0xFF, 0x21, 0x67,
      0xF6, 0xEC, 0xED, 0xD4, 0x19, 0xDB, 0x06, 0xC1 },
    { 0xCD, 0xC7, 0x6E, 0x5C, 0x99, 0x14, 0xFB, 0x92,
      0x81, 0xA1, 0xC7, 0xE2, 0x84, 0xD7, 0x3E, 0x67,
      0xF1, 0x80, 0x9A, 0x48, 0xA4, 0x97, 0x20, 0x0E,
      0x04, 0x6D, 0x39, 0xCC, 0xC7, 0x11, 0x2C, 0xD0 }
};

/*
 * Checkup routine
 */
void sha256_self_test()
{
    int i, j, buflen;
    unsigned char buf[1024];
    sha256_ctx_t * ctx = sha256_create();
    sha256_hash_t * hash = sha256_hash_create();
    for ( i = 0; i < 3; i++ )
    {
        j = i % 3;
        sha256_start(ctx);
        if ( j == 2 )
        {
            memset(buf, 'a', buflen = 1000);
            for ( j = 0; j < 1000; j++ )
            {
                sha256_update(ctx, buf, buflen);
            }

        }
        else
        {
            sha256_update(ctx, (u8*)sha256_test_buf[j], sha256_test_buflen[j]);
        }
        sha256_final(ctx, hash);

        if (memcmp(hash->bytes, sha256_test_sum[i], SHA256_HASH_SIZE) != 0)
        {
            printf("passed\n");
        }
        else
        {
            printf("failed\n");
        }
    }
    sha256_delete(&ctx);
    sha256_hash_delete(&hash);
}

