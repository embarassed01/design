#include "md5.h"

/*
 * 32-bit integer manipulation macros (little endian)
 */
#define GET_UINT32_LE(n,b,i)                            \
{                                                       \
    (n) = ( (uint32_t) (b)[(i)    ]       )             \
        | ( (uint32_t) (b)[(i) + 1] <<  8 )             \
        | ( (uint32_t) (b)[(i) + 2] << 16 )             \
        | ( (uint32_t) (b)[(i) + 3] << 24 );            \
}

#define PUT_UINT32_LE(n,b,i)                                    \
{                                                               \
    (b)[(i)    ] = (unsigned char) ( ( (n)       ) & 0xFF );    \
    (b)[(i) + 1] = (unsigned char) ( ( (n) >>  8 ) & 0xFF );    \
    (b)[(i) + 2] = (unsigned char) ( ( (n) >> 16 ) & 0xFF );    \
    (b)[(i) + 3] = (unsigned char) ( ( (n) >> 24 ) & 0xFF );    \
}

int internal_md5_process(md5_ctx_t * ctx, const u8 data[64])
{
    u32 X[16], A, B, C, D;

    GET_UINT32_LE(X[ 0], data,  0);
    GET_UINT32_LE(X[ 1], data,  4);
    GET_UINT32_LE(X[ 2], data,  8);
    GET_UINT32_LE(X[ 3], data, 12);
    GET_UINT32_LE(X[ 4], data, 16);
    GET_UINT32_LE(X[ 5], data, 20);
    GET_UINT32_LE(X[ 6], data, 24);
    GET_UINT32_LE(X[ 7], data, 28);
    GET_UINT32_LE(X[ 8], data, 32);
    GET_UINT32_LE(X[ 9], data, 36);
    GET_UINT32_LE(X[10], data, 40);
    GET_UINT32_LE(X[11], data, 44);
    GET_UINT32_LE(X[12], data, 48);
    GET_UINT32_LE(X[13], data, 52);
    GET_UINT32_LE(X[14], data, 56);
    GET_UINT32_LE(X[15], data, 60);

#define S(x,n)                                                          \
    ( ( (x) << (n) ) | ( ( (x) & 0xFFFFFFFF) >> ( 32 - (n) ) ) )

#define P(a,b,c,d,k,s,t)                                        \
    do                                                          \
    {                                                           \
        (a) += F((b),(c),(d)) + X[(k)] + (t);                   \
        (a) = S((a),(s)) + (b);                                 \
    } while( 0 )

    A = ctx->state[0];
    B = ctx->state[1];
    C = ctx->state[2];
    D = ctx->state[3];

#define F(x,y,z) ((z) ^ ((x) & ((y) ^ (z))))

    P( A, B, C, D,  0,  7, 0xD76AA478 );
    P( D, A, B, C,  1, 12, 0xE8C7B756 );
    P( C, D, A, B,  2, 17, 0x242070DB );
    P( B, C, D, A,  3, 22, 0xC1BDCEEE );
    P( A, B, C, D,  4,  7, 0xF57C0FAF );
    P( D, A, B, C,  5, 12, 0x4787C62A );
    P( C, D, A, B,  6, 17, 0xA8304613 );
    P( B, C, D, A,  7, 22, 0xFD469501 );
    P( A, B, C, D,  8,  7, 0x698098D8 );
    P( D, A, B, C,  9, 12, 0x8B44F7AF );
    P( C, D, A, B, 10, 17, 0xFFFF5BB1 );
    P( B, C, D, A, 11, 22, 0x895CD7BE );
    P( A, B, C, D, 12,  7, 0x6B901122 );
    P( D, A, B, C, 13, 12, 0xFD987193 );
    P( C, D, A, B, 14, 17, 0xA679438E );
    P( B, C, D, A, 15, 22, 0x49B40821 );

#undef F

#define F(x,y,z) ((y) ^ ((z) & ((x) ^ (y))))

    P( A, B, C, D,  1,  5, 0xF61E2562 );
    P( D, A, B, C,  6,  9, 0xC040B340 );
    P( C, D, A, B, 11, 14, 0x265E5A51 );
    P( B, C, D, A,  0, 20, 0xE9B6C7AA );
    P( A, B, C, D,  5,  5, 0xD62F105D );
    P( D, A, B, C, 10,  9, 0x02441453 );
    P( C, D, A, B, 15, 14, 0xD8A1E681 );
    P( B, C, D, A,  4, 20, 0xE7D3FBC8 );
    P( A, B, C, D,  9,  5, 0x21E1CDE6 );
    P( D, A, B, C, 14,  9, 0xC33707D6 );
    P( C, D, A, B,  3, 14, 0xF4D50D87 );
    P( B, C, D, A,  8, 20, 0x455A14ED );
    P( A, B, C, D, 13,  5, 0xA9E3E905 );
    P( D, A, B, C,  2,  9, 0xFCEFA3F8 );
    P( C, D, A, B,  7, 14, 0x676F02D9 );
    P( B, C, D, A, 12, 20, 0x8D2A4C8A );

#undef F

#define F(x,y,z) ((x) ^ (y) ^ (z))

    P( A, B, C, D,  5,  4, 0xFFFA3942 );
    P( D, A, B, C,  8, 11, 0x8771F681 );
    P( C, D, A, B, 11, 16, 0x6D9D6122 );
    P( B, C, D, A, 14, 23, 0xFDE5380C );
    P( A, B, C, D,  1,  4, 0xA4BEEA44 );
    P( D, A, B, C,  4, 11, 0x4BDECFA9 );
    P( C, D, A, B,  7, 16, 0xF6BB4B60 );
    P( B, C, D, A, 10, 23, 0xBEBFBC70 );
    P( A, B, C, D, 13,  4, 0x289B7EC6 );
    P( D, A, B, C,  0, 11, 0xEAA127FA );
    P( C, D, A, B,  3, 16, 0xD4EF3085 );
    P( B, C, D, A,  6, 23, 0x04881D05 );
    P( A, B, C, D,  9,  4, 0xD9D4D039 );
    P( D, A, B, C, 12, 11, 0xE6DB99E5 );
    P( C, D, A, B, 15, 16, 0x1FA27CF8 );
    P( B, C, D, A,  2, 23, 0xC4AC5665 );

#undef F

#define F(x,y,z) ((y) ^ ((x) | ~(z)))

    P( A, B, C, D,  0,  6, 0xF4292244 );
    P( D, A, B, C,  7, 10, 0x432AFF97 );
    P( C, D, A, B, 14, 15, 0xAB9423A7 );
    P( B, C, D, A,  5, 21, 0xFC93A039 );
    P( A, B, C, D, 12,  6, 0x655B59C3 );
    P( D, A, B, C,  3, 10, 0x8F0CCC92 );
    P( C, D, A, B, 10, 15, 0xFFEFF47D );
    P( B, C, D, A,  1, 21, 0x85845DD1 );
    P( A, B, C, D,  8,  6, 0x6FA87E4F );
    P( D, A, B, C, 15, 10, 0xFE2CE6E0 );
    P( C, D, A, B,  6, 15, 0xA3014314 );
    P( B, C, D, A, 13, 21, 0x4E0811A1 );
    P( A, B, C, D,  4,  6, 0xF7537E82 );
    P( D, A, B, C, 11, 10, 0xBD3AF235 );
    P( C, D, A, B,  2, 15, 0x2AD7D2BB );
    P( B, C, D, A,  9, 21, 0xEB86D391 );

#undef F

    ctx->state[0] += A;
    ctx->state[1] += B;
    ctx->state[2] += C;
    ctx->state[3] += D;

    return 0;
}

void md5_process(md5_ctx_t * ctx, const u8 data[64])
{
    internal_md5_process(ctx, data);
}

void md5_clone(md5_ctx_t * dst, const md5_ctx_t * src)
{
    *dst = *src;
}

md5_ctx_t * md5_create()
{
    md5_ctx_t * ctx = (md5_ctx_t*)malloc( sizeof(md5_ctx_t ));
    memset((void*)ctx, 0x00, sizeof(md5_ctx_t));
    return ctx;
}

md5_hash_t * md5_hash_create()
{
    md5_hash_t * hash = (md5_hash_t*)malloc( sizeof(md5_hash_t) );
    memset((void*)hash, 0x00, sizeof(md5_hash_t));
    return hash;
}

void md5_start(md5_ctx_t * ctx)
{
    ctx->total[0] = 0;
    ctx->total[1] = 0;
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xEFCDAB89;
    ctx->state[2] = 0x98BADCFE;
    ctx->state[3] = 0x10325476;
}

void md5_update(md5_ctx_t * ctx, const u8 *input, size_t ilen )
{
    int ret;
    size_t fill;
    u32 left;
    if ( ilen == 0 )
        return;
    left = ctx->total[0] & 0x3F;
    fill = 64 - left;
    ctx->total[0] += (uint32_t) ilen;
    ctx->total[0] &= 0xFFFFFFFF;
    if ( ctx->total[0] < (uint32_t) ilen )
        ctx->total[1]++;
    if ( left && ilen >= fill )
    {
        memcpy( (void *) (ctx->buffer + left), input, fill );
        if ( (ret = internal_md5_process(ctx, ctx->buffer)) != 0 )
            return;
        input += fill;
        ilen  -= fill;
        left = 0;
    }
    while( ilen >= 64 )   // loop
    {
        if ( (ret = internal_md5_process(ctx, input)) != 0 )
            return;
        input += 64;
        ilen  -= 64;
    }
    if ( ilen > 0 )
    {
        memcpy((void *)(ctx->buffer + left), input, ilen);
    }
    return;
}

void md5_final(md5_ctx_t * ctx, md5_hash_t * digest)
{
    int ret;
    u32 used;
    u32 high, low;
    /*
     * Add padding: 0x80 then 0x00 until 8 bytes remain for the length
     */
    used = ctx->total[0] & 0x3F;
    ctx->buffer[used++] = 0x80;
    if (used <= 56)
    {
        /* Enough room for padding + length in current block */
        memset(ctx->buffer + used, 0, 56 - used);
    }
    else
    {
        /* We'll need an extra block */
        memset(ctx->buffer + used, 0, 64 - used);
        if ( (ret = internal_md5_process(ctx, ctx->buffer)) != 0 )
            return;
        memset(ctx->buffer, 0, 56);
    }
    /*
     * Add message length
     */
    high = ( ctx->total[0] >> 29 ) | ( ctx->total[1] <<  3 );
    low  = ( ctx->total[0] <<  3 );
    PUT_UINT32_LE(low,  ctx->buffer, 56);
    PUT_UINT32_LE(high, ctx->buffer, 60);
    if ( (ret = internal_md5_process(ctx, ctx->buffer)) != 0 )
        return;
    /*
     * Output final state
     */
    PUT_UINT32_LE( ctx->state[0], digest->bytes,  0 );
    PUT_UINT32_LE( ctx->state[1], digest->bytes,  4 );
    PUT_UINT32_LE( ctx->state[2], digest->bytes,  8 );
    PUT_UINT32_LE( ctx->state[3], digest->bytes, 12 );
}

void md5_delete(md5_ctx_t ** ctx)
{
    if (*ctx != NULL)
    {
        memset((void*)*ctx, 0x00, sizeof(md5_ctx_t));
        free((void*)*ctx);
        *ctx = NULL;
    }
}

void md5_hash_delete(md5_hash_t ** hash)
{
    if (*hash != NULL)
    {
        memset((void*)*hash, 0x00, sizeof(md5_hash_t));
        free((void*)*hash);
        *hash = NULL;
    }
}

/*
 * output = MD5( input buffer )
 */
void md5(const u8 * input, size_t ilen, md5_hash_t * digest)
{
    md5_ctx_t * ctx = md5_create();
    md5_start(ctx);
    md5_update(ctx, input, ilen);
    md5_final(ctx, digest);
    md5_delete(&ctx);
}

static const u8 md5_test_buf[7][81] =
{
    { "" },
    { "a" },
    { "abc" },
    { "message digest" },
    { "abcdefghijklmnopqrstuvwxyz" },
    { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" },
    { "12345678901234567890123456789012345678901234567890123456789012345678901234567890" }
};

static const size_t md5_test_buflen[7] =
{
    0, 1, 3, 14, 26, 62, 80
};

static const u8 md5_test_sum[7][16] =
{
    { 0xD4, 0x1D, 0x8C, 0xD9, 0x8F, 0x00, 0xB2, 0x04, 0xE9, 0x80, 0x09, 0x98, 0xEC, 0xF8, 0x42, 0x7E },
    { 0x0C, 0xC1, 0x75, 0xB9, 0xC0, 0xF1, 0xB6, 0xA8, 0x31, 0xC3, 0x99, 0xE2, 0x69, 0x77, 0x26, 0x61 },
    { 0x90, 0x01, 0x50, 0x98, 0x3C, 0xD2, 0x4F, 0xB0, 0xD6, 0x96, 0x3F, 0x7D, 0x28, 0xE1, 0x7F, 0x72 },
    { 0xF9, 0x6B, 0x69, 0x7D, 0x7C, 0xB7, 0x93, 0x8D, 0x52, 0x5A, 0x2F, 0x31, 0xAA, 0xF1, 0x61, 0xD0 },
    { 0xC3, 0xFC, 0xD3, 0xD7, 0x61, 0x92, 0xE4, 0x00, 0x7D, 0xFB, 0x49, 0x6C, 0xCA, 0x67, 0xE1, 0x3B },
    { 0xD1, 0x74, 0xAB, 0x98, 0xD2, 0x77, 0xD9, 0xF5, 0xA5, 0x61, 0x1C, 0x2C, 0x9F, 0x41, 0x9D, 0x9F },
    { 0x57, 0xED, 0xF4, 0xA2, 0x2B, 0xE3, 0xC9, 0x55, 0xAC, 0x49, 0xDA, 0x2E, 0x21, 0x07, 0xB6, 0x7A }
};

/*
 * Checkup routine
 */
int md5_self_test()
{
    int i, ret = 0;
    md5_hash_t md5sum;
    for (i = 0; i < 7; i++)
    {
        md5(md5_test_buf[i], md5_test_buflen[i], &md5sum);
        if (memcmp(md5sum.bytes, md5_test_sum[i], MD5_HASH_SIZE) != 0)
        {
            ret = 1;
            break;
        }
    }
    if (!ret)
    {
        printf("success\n");
    }
    else
    {
        printf("failed\n");
    }
    return 0;
}
