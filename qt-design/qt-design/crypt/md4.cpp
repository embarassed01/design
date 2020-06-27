#include "md4.h"


/*
 * 32-bit integer manipulation macros (little endian)
 */
#ifndef GET_UINT32_LE
#define GET_UINT32_LE(n,b,i)                            \
{                                                       \
    (n) = ( (uint32_t) (b)[(i)    ]       )             \
        | ( (uint32_t) (b)[(i) + 1] <<  8 )             \
        | ( (uint32_t) (b)[(i) + 2] << 16 )             \
        | ( (uint32_t) (b)[(i) + 3] << 24 );            \
}
#endif

#ifndef PUT_UINT32_LE
#define PUT_UINT32_LE(n,b,i)                                    \
{                                                               \
    (b)[(i)    ] = (unsigned char) ( ( (n)       ) & 0xFF );    \
    (b)[(i) + 1] = (unsigned char) ( ( (n) >>  8 ) & 0xFF );    \
    (b)[(i) + 2] = (unsigned char) ( ( (n) >> 16 ) & 0xFF );    \
    (b)[(i) + 3] = (unsigned char) ( ( (n) >> 24 ) & 0xFF );    \
}
#endif

int internal_md4_process(md4_ctx_t * ctx, const u8 data[64])
{
    uint32_t X[16], A, B, C, D;

    GET_UINT32_LE( X[ 0], data,  0 );
    GET_UINT32_LE( X[ 1], data,  4 );
    GET_UINT32_LE( X[ 2], data,  8 );
    GET_UINT32_LE( X[ 3], data, 12 );
    GET_UINT32_LE( X[ 4], data, 16 );
    GET_UINT32_LE( X[ 5], data, 20 );
    GET_UINT32_LE( X[ 6], data, 24 );
    GET_UINT32_LE( X[ 7], data, 28 );
    GET_UINT32_LE( X[ 8], data, 32 );
    GET_UINT32_LE( X[ 9], data, 36 );
    GET_UINT32_LE( X[10], data, 40 );
    GET_UINT32_LE( X[11], data, 44 );
    GET_UINT32_LE( X[12], data, 48 );
    GET_UINT32_LE( X[13], data, 52 );
    GET_UINT32_LE( X[14], data, 56 );
    GET_UINT32_LE( X[15], data, 60 );

#define S(x,n) (((x) << (n)) | (((x) & 0xFFFFFFFF) >> (32 - (n))))

    A = ctx->state[0];
    B = ctx->state[1];
    C = ctx->state[2];
    D = ctx->state[3];

#define F(x, y, z) (((x) & (y)) | ((~(x)) & (z)))
#define P(a,b,c,d,x,s)                           \
    do                                           \
    {                                            \
        (a) += F((b),(c),(d)) + (x);             \
        (a) = S((a),(s));                        \
    } while( 0 )


    P( A, B, C, D, X[ 0],  3 );
    P( D, A, B, C, X[ 1],  7 );
    P( C, D, A, B, X[ 2], 11 );
    P( B, C, D, A, X[ 3], 19 );
    P( A, B, C, D, X[ 4],  3 );
    P( D, A, B, C, X[ 5],  7 );
    P( C, D, A, B, X[ 6], 11 );
    P( B, C, D, A, X[ 7], 19 );
    P( A, B, C, D, X[ 8],  3 );
    P( D, A, B, C, X[ 9],  7 );
    P( C, D, A, B, X[10], 11 );
    P( B, C, D, A, X[11], 19 );
    P( A, B, C, D, X[12],  3 );
    P( D, A, B, C, X[13],  7 );
    P( C, D, A, B, X[14], 11 );
    P( B, C, D, A, X[15], 19 );

#undef P
#undef F

#define F(x,y,z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define P(a,b,c,d,x,s)                          \
    do                                          \
    {                                           \
        (a) += F((b),(c),(d)) + (x) + 0x5A827999;       \
        (a) = S((a),(s));                               \
    } while( 0 )

    P( A, B, C, D, X[ 0],  3 );
    P( D, A, B, C, X[ 4],  5 );
    P( C, D, A, B, X[ 8],  9 );
    P( B, C, D, A, X[12], 13 );
    P( A, B, C, D, X[ 1],  3 );
    P( D, A, B, C, X[ 5],  5 );
    P( C, D, A, B, X[ 9],  9 );
    P( B, C, D, A, X[13], 13 );
    P( A, B, C, D, X[ 2],  3 );
    P( D, A, B, C, X[ 6],  5 );
    P( C, D, A, B, X[10],  9 );
    P( B, C, D, A, X[14], 13 );
    P( A, B, C, D, X[ 3],  3 );
    P( D, A, B, C, X[ 7],  5 );
    P( C, D, A, B, X[11],  9 );
    P( B, C, D, A, X[15], 13 );

#undef P
#undef F

#define F(x,y,z) ((x) ^ (y) ^ (z))
#define P(a,b,c,d,x,s)                                  \
    do                                                  \
    {                                                   \
        (a) += F((b),(c),(d)) + (x) + 0x6ED9EBA1;       \
        (a) = S((a),(s));                               \
    } while( 0 )

    P( A, B, C, D, X[ 0],  3 );
    P( D, A, B, C, X[ 8],  9 );
    P( C, D, A, B, X[ 4], 11 );
    P( B, C, D, A, X[12], 15 );
    P( A, B, C, D, X[ 2],  3 );
    P( D, A, B, C, X[10],  9 );
    P( C, D, A, B, X[ 6], 11 );
    P( B, C, D, A, X[14], 15 );
    P( A, B, C, D, X[ 1],  3 );
    P( D, A, B, C, X[ 9],  9 );
    P( C, D, A, B, X[ 5], 11 );
    P( B, C, D, A, X[13], 15 );
    P( A, B, C, D, X[ 3],  3 );
    P( D, A, B, C, X[11],  9 );
    P( C, D, A, B, X[ 7], 11 );
    P( B, C, D, A, X[15], 15 );

#undef F
#undef P

    ctx->state[0] += A;
    ctx->state[1] += B;
    ctx->state[2] += C;
    ctx->state[3] += D;

    return( 0 );
}

void md4_process(md4_ctx_t * ctx, const u8 data[64])
{
    internal_md4_process( ctx, data );
}

md4_ctx_t * md4_create()
{
    md4_ctx_t * ctx = (md4_ctx_t*)malloc( sizeof(md4_ctx_t) );
    memset((void*)ctx, 0x00, sizeof(md4_ctx_t));
    return ctx;
}

md4_hash_t * md4_hash_create()
{
    md4_hash_t * hash = (md4_hash_t*)malloc( sizeof(md4_hash_t) );
    memset((void*)hash, 0x00, sizeof(md4_hash_t));
    return hash;
}

void md4_update(md4_ctx_t * ctx, const u8 * input, size_t ilen)
{
    int ret;
    size_t fill;
    u32 left;
    if (ilen == 0)
        return;
    left = ctx->total[0] & 0x3F;
    fill = 64 - left;
    ctx->total[0] += (uint32_t) ilen;
    ctx->total[0] &= 0xFFFFFFFF;
    if ( ctx->total[0] < (uint32_t) ilen )
        ctx->total[1]++;
    if ( left && ilen >= fill )
    {
        memcpy( (void *) (ctx->buffer + left),
                (void *) input, fill );
        if (( ret = internal_md4_process( ctx, ctx->buffer ) ) != 0)
            return;
        input += fill;
        ilen  -= fill;
        left = 0;
    }
    while ( ilen >= 64 )
    {
        if( ( ret = internal_md4_process( ctx, input ) ) != 0 )
            return;
        input += 64;
        ilen  -= 64;
    }
    if ( ilen > 0 )
    {
        memcpy((void *)(ctx->buffer + left), (void *)input, ilen);
    }
    return;
}

void md4_start(md4_ctx_t * ctx)
{
    ctx->total[0] = 0;
    ctx->total[1] = 0;
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xEFCDAB89;
    ctx->state[2] = 0x98BADCFE;
    ctx->state[3] = 0x10325476;
}

static const unsigned char md4_padding[64] =
{
 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


void md4_final(md4_ctx_t * ctx, md4_hash_t * hash)
{
    int ret;
    uint32_t last, padn;
    uint32_t high, low;
    unsigned char msglen[8];
    high = ( ctx->total[0] >> 29 )
         | ( ctx->total[1] <<  3 );
    low  = ( ctx->total[0] <<  3 );
    PUT_UINT32_LE( low,  msglen, 0 );
    PUT_UINT32_LE( high, msglen, 4 );
    last = ctx->total[0] & 0x3F;
    padn = ( last < 56 ) ? ( 56 - last ) : ( 120 - last );
    md4_update(ctx, (u8 *)md4_padding, padn);
    md4_update(ctx, msglen, 8);
    PUT_UINT32_LE( ctx->state[0], hash->bytes,  0 );
    PUT_UINT32_LE( ctx->state[1], hash->bytes,  4 );
    PUT_UINT32_LE( ctx->state[2], hash->bytes,  8 );
    PUT_UINT32_LE( ctx->state[3], hash->bytes, 12 );
}

void md4_delete(md4_ctx_t ** ctx)
{
    if (*ctx != NULL)
    {
        memset((void*)*ctx, 0x00, sizeof(md4_ctx_t));
        free((void*)*ctx);
        *ctx = NULL;
    }
}

void md4_hash_delete(md4_hash_t ** hash)
{
    if (*hash != NULL)
    {
        memset((void*)*hash, 0x00, sizeof(md4_hash_t));
        free((void*)*hash);
        *hash = NULL;
    }
}

void md4(const u8 * input, size_t ilen, md4_hash_t * hash)
{
    md4_ctx_t * ctx = md4_create();
    md4_start(ctx);
    md4_update(ctx, input, ilen);
    md4_final(ctx, hash);
    md4_delete(&ctx);
}

/*
 * RFC 1320 test vectors
 */
static const unsigned char md4_test_str[7][81] =
{
    { "" },
    { "a" },
    { "abc" },
    { "message digest" },
    { "abcdefghijklmnopqrstuvwxyz" },
    { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" },
    { "12345678901234567890123456789012345678901234567890123456789012"
      "345678901234567890" }
};

static const size_t md4_test_strlen[7] =
{
    0, 1, 3, 14, 26, 62, 80
};

static const unsigned char md4_test_sum[7][16] =
{
    { 0x31, 0xD6, 0xCF, 0xE0, 0xD1, 0x6A, 0xE9, 0x31,
      0xB7, 0x3C, 0x59, 0xD7, 0xE0, 0xC0, 0x89, 0xC0 },
    { 0xBD, 0xE5, 0x2C, 0xB3, 0x1D, 0xE3, 0x3E, 0x46,
      0x24, 0x5E, 0x05, 0xFB, 0xDB, 0xD6, 0xFB, 0x24 },
    { 0xA4, 0x48, 0x01, 0x7A, 0xAF, 0x21, 0xD8, 0x52,
      0x5F, 0xC1, 0x0A, 0xE8, 0x7A, 0xA6, 0x72, 0x9D },
    { 0xD9, 0x13, 0x0A, 0x81, 0x64, 0x54, 0x9F, 0xE8,
      0x18, 0x87, 0x48, 0x06, 0xE1, 0xC7, 0x01, 0x4B },
    { 0xD7, 0x9E, 0x1C, 0x30, 0x8A, 0xA5, 0xBB, 0xCD,
      0xEE, 0xA8, 0xED, 0x63, 0xDF, 0x41, 0x2D, 0xA9 },
    { 0x04, 0x3F, 0x85, 0x82, 0xF2, 0x41, 0xDB, 0x35,
      0x1C, 0xE6, 0x27, 0xE1, 0x53, 0xE7, 0xF0, 0xE4 },
    { 0xE3, 0x3B, 0x4D, 0xDC, 0x9C, 0x38, 0xF2, 0x19,
      0x9C, 0x3E, 0x7B, 0x16, 0x4F, 0xCC, 0x05, 0x36 }
};

/*
 * Checkup routine
 */
void md4_self_test( int verbose )
{
    int i;
    md4_hash_t * md4sum = md4_hash_create();
    for( i = 0; i < 7; i++ )
    {
        md4(md4_test_str[i], md4_test_strlen[i], md4sum);
        if (memcmp(md4sum->bytes, md4_test_sum[i], MD4_HASH_SIZE) != 0)
        {
            printf( "passed\n" );
        }
        else
        {
            printf( "failed\n" );
        }
    }
    md4_hash_delete(&md4sum);
}

