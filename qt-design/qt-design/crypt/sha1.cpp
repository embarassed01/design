#include "sha1.h"

// Decide whether to use the Little-Endian shortcut. If the shortcut is not used then the code will work correctly
// on either big or little endian, however if we do know it is a little endian architecture we can speed it up a bit.
// Note, there are TWO places where USE_LITTLE_ENDIAN_SHORTCUT is used. They MUST be paired together.
#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && ( __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ )
    // gcc defines __BYTE_ORDER__ so if it says its little endian we can use that.
    #define USE_LITTLE_ENDIAN_SHORTCUT
#elif defined( _WIN32 )
    // Windows is always little endian so we can use that.
    #define USE_LITTLE_ENDIAN_SHORTCUT
#endif

typedef union
{
    u8     c[64];
    u32    l[16];
} CHAR64LONG16;

// Endian neutral macro for loading 32 bit value from 4 byte array (in big endian form).
#define LOAD32H(x, y)                           \
     { x = ((uint32_t)((y)[0] & 255)<<24) |     \
           ((uint32_t)((y)[1] & 255)<<16) |     \
           ((uint32_t)((y)[2] & 255)<<8)  |     \
           ((uint32_t)((y)[3] & 255)); }

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

// blk0() and blk() perform the initial expand.
#ifdef USE_LITTLE_ENDIAN_SHORTCUT
    #define blk0(i) (block->l[i] = (rol(block->l[i],24)&0xFF00FF00) | (rol(block->l[i],8)&0x00FF00FF))
#else
    #define blk0(i) block->l[i]
#endif

#define blk(i) (block->l[i&15] = rol(block->l[(i+13)&15] ^ block->l[(i+8)&15] ^ block->l[(i+2)&15] ^ block->l[i&15],1))

// (R0+R1), R2, R3, R4 are the different operations used in SHA1
#define R0(v,w,x,y,z,i)  z += ((w&(x^y))^y)     + blk0(i)+ 0x5A827999 + rol(v,5); w=rol(w,30);
#define R1(v,w,x,y,z,i)  z += ((w&(x^y))^y)     + blk(i) + 0x5A827999 + rol(v,5); w=rol(w,30);
#define R2(v,w,x,y,z,i)  z += (w^x^y)           + blk(i) + 0x6ED9EBA1 + rol(v,5); w=rol(w,30);
#define R3(v,w,x,y,z,i)  z += (((w|x)&y)|(w&x)) + blk(i) + 0x8F1BBCDC + rol(v,5); w=rol(w,30);
#define R4(v,w,x,y,z,i)  z += (w^x^y)           + blk(i) + 0xCA62C1D6 + rol(v,5); w=rol(w,30);

// Loads the 128 bits from ByteArray into WordArray, treating ByteArray as big endian data
#ifdef USE_LITTLE_ENDIAN_SHORTCUT
    #define Load128BitsAsWords(WordArray, ByteArray)  \
        memcpy(WordArray, ByteArray, 64)
#else
    #define Load128BitsAsWords( WordArray, ByteArray )      \
    {                                                       \
        uint32_t i;                                         \
        for( i=0; i<16; i++ )                               \
        {                                                   \
            LOAD32H( (WordArray)[i], (ByteArray)+(i*4) );   \
        }                                                   \
    }
#endif

//  TransformFunction
//  Hash a single 512-bit block. This is the core of the algorithm
static void TransformFunction(u32 state[5], u8 buffer[64])
{
    u32 a;
    u32 b;
    u32 c;
    u32 d;
    u32 e;
    u8 workspace[64];
    CHAR64LONG16 * block = (CHAR64LONG16*)workspace;

    Load128BitsAsWords(block->l, buffer);

    // Copy context->state[] to working vars
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];

    // 4 rounds of 20 operations each. Loop unrolled.
    R0(a,b,c,d,e, 0); R0(e,a,b,c,d, 1); R0(d,e,a,b,c, 2); R0(c,d,e,a,b, 3);
    R0(b,c,d,e,a, 4); R0(a,b,c,d,e, 5); R0(e,a,b,c,d, 6); R0(d,e,a,b,c, 7);
    R0(c,d,e,a,b, 8); R0(b,c,d,e,a, 9); R0(a,b,c,d,e,10); R0(e,a,b,c,d,11);
    R0(d,e,a,b,c,12); R0(c,d,e,a,b,13); R0(b,c,d,e,a,14); R0(a,b,c,d,e,15);
    R1(e,a,b,c,d,16); R1(d,e,a,b,c,17); R1(c,d,e,a,b,18); R1(b,c,d,e,a,19);
    R2(a,b,c,d,e,20); R2(e,a,b,c,d,21); R2(d,e,a,b,c,22); R2(c,d,e,a,b,23);
    R2(b,c,d,e,a,24); R2(a,b,c,d,e,25); R2(e,a,b,c,d,26); R2(d,e,a,b,c,27);
    R2(c,d,e,a,b,28); R2(b,c,d,e,a,29); R2(a,b,c,d,e,30); R2(e,a,b,c,d,31);
    R2(d,e,a,b,c,32); R2(c,d,e,a,b,33); R2(b,c,d,e,a,34); R2(a,b,c,d,e,35);
    R2(e,a,b,c,d,36); R2(d,e,a,b,c,37); R2(c,d,e,a,b,38); R2(b,c,d,e,a,39);
    R3(a,b,c,d,e,40); R3(e,a,b,c,d,41); R3(d,e,a,b,c,42); R3(c,d,e,a,b,43);
    R3(b,c,d,e,a,44); R3(a,b,c,d,e,45); R3(e,a,b,c,d,46); R3(d,e,a,b,c,47);
    R3(c,d,e,a,b,48); R3(b,c,d,e,a,49); R3(a,b,c,d,e,50); R3(e,a,b,c,d,51);
    R3(d,e,a,b,c,52); R3(c,d,e,a,b,53); R3(b,c,d,e,a,54); R3(a,b,c,d,e,55);
    R3(e,a,b,c,d,56); R3(d,e,a,b,c,57); R3(c,d,e,a,b,58); R3(b,c,d,e,a,59);
    R4(a,b,c,d,e,60); R4(e,a,b,c,d,61); R4(d,e,a,b,c,62); R4(c,d,e,a,b,63);
    R4(b,c,d,e,a,64); R4(a,b,c,d,e,65); R4(e,a,b,c,d,66); R4(d,e,a,b,c,67);
    R4(c,d,e,a,b,68); R4(b,c,d,e,a,69); R4(a,b,c,d,e,70); R4(e,a,b,c,d,71);
    R4(d,e,a,b,c,72); R4(c,d,e,a,b,73); R4(b,c,d,e,a,74); R4(a,b,c,d,e,75);
    R4(e,a,b,c,d,76); R4(d,e,a,b,c,77); R4(c,d,e,a,b,78); R4(b,c,d,e,a,79);

    // Add the working vars back into context.state[]
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
}

sha1_ctx_t * sha1_create()
{
    sha1_ctx_t * ctx = (sha1_ctx_t*)malloc( sizeof(sha1_ctx_t) );
    memset((void*)ctx, 0x00, sizeof(sha1_ctx_t));
    return ctx;
}

sha1_hash_t * sha1_hash_create()
{
    sha1_hash_t * hash = (sha1_hash_t*)malloc( sizeof(sha1_hash_t) );
    memset((void*)hash, 0x00, sizeof(sha1_hash_t));
    return hash;
}

void sha1_start(sha1_ctx_t * ctx)
{
    // SHA1 initialisation constants
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xEFCDAB89;
    ctx->state[2] = 0x98BADCFE;
    ctx->state[3] = 0x10325476;
    ctx->state[4] = 0xC3D2E1F0;
    ctx->total[0] = 0;
    ctx->total[1] = 0;
}


void sha1_update(sha1_ctx_t * ctx, u8 * buffer, size_t buffer_size)
{
    u32 i;
    u32 j;
    j = (ctx->total[0] >> 3) & 0x3f;
    if ( (ctx->total[0] += buffer_size << 3) < (buffer_size << 3) )
    {
        ctx->total[1]++;
    }
    ctx->total[1] += (buffer_size >> 29);
    if ( (j + buffer_size) > 0x3f )
    {
        i = 0x40 - j;
        memcpy(&ctx->buffer[j], buffer, i);
        TransformFunction(ctx->state, ctx->buffer);
        for ( ; i + 0x3f < buffer_size; i += 0x40 )   // loop
        {
            TransformFunction(ctx->state, (u8*)buffer + i);
        }
        j = 0;
    }
    else
    {
        i = 0;
    }
    memcpy(&ctx->buffer[j], &((u8*)buffer)[i], buffer_size - i);
}

void sha1_final(sha1_ctx_t * ctx, sha1_hash_t * digest)
{
    u32 i;
    u8  finalcount[8];
    for (i = 0; i < 8; i++)
    {
        finalcount[i] = (unsigned char)((ctx->total[(i >= 4 ? 0 : 1)] >> ((3-(i & 3)) * 8)) & 0xff);  // Endian independent
    }
    sha1_update(ctx, (u8*)"\x80", 1);
    while ( (ctx->total[0] & 0x1f8) != 0x1c0 )
    {
        sha1_update(ctx, (u8*)"\0", 1);
    }
    sha1_update(ctx, finalcount, 8);  // Should cause a Sha1TransformFunction()
    for(i = 0; i < SHA1_HASH_SIZE; i++)
    {
        digest->bytes[i] = (u8)((ctx->state[i>>2] >> ((3-(i & 3)) * 8) ) & 0xff);
    }
}

void sha1_delete(sha1_ctx_t ** ctx)
{
    if (*ctx != NULL)
    {
        memset((void*)*ctx, 0x00, sizeof(sha1_ctx_t));
        free((void*)*ctx);
        *ctx = NULL;
    }
}

void sha1_hash_delete(sha1_hash_t ** hash)
{
    if (*hash != NULL)
    {
        memset((void*)*hash, 0x00, sizeof(sha1_hash_t));
        free((void*)*hash);
        *hash = NULL;
    }
}


void sha1(u8 * buffer, size_t buffer_size, sha1_hash_t * digest)
{
    sha1_ctx_t * ctx = sha1_create();
    sha1_start(ctx);
    sha1_update(ctx, buffer, buffer_size);
    sha1_final(ctx, digest);
    sha1_delete(&ctx);
}

/*
 * FIPS-180-1 test vectors
 */
static const unsigned char sha1_test_buf[3][57] =
{
    { "abc" },
    { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq" },
    { "" }
};

static const size_t sha1_test_buflen[3] =
{
    3, 56, 1000
};

static const unsigned char sha1_test_sum[3][20] =
{
    { 0xA9, 0x99, 0x3E, 0x36, 0x47, 0x06, 0x81, 0x6A, 0xBA, 0x3E,
      0x25, 0x71, 0x78, 0x50, 0xC2, 0x6C, 0x9C, 0xD0, 0xD8, 0x9D },
    { 0x84, 0x98, 0x3E, 0x44, 0x1C, 0x3B, 0xD2, 0x6E, 0xBA, 0xAE,
      0x4A, 0xA1, 0xF9, 0x51, 0x29, 0xE5, 0xE5, 0x46, 0x70, 0xF1 },
    { 0x34, 0xAA, 0x97, 0x3C, 0xD4, 0xC4, 0xDA, 0xA4, 0xF6, 0x1E,
      0xEB, 0x2B, 0xDB, 0xAD, 0x27, 0x31, 0x65, 0x34, 0x01, 0x6F }
};

/*
 * Checkup routine
 */
void mbedtls_sha1_self_test()
{
    int i, j, buflen;
    u8 buf[1024] = {0x00};
    sha1_ctx_t * ctx = sha1_create();
    sha1_hash_t * hash = sha1_hash_create();
    for( i = 0; i < 3; i++ )
    {
        sha1_start(ctx);
        if( i == 2 )
        {
            memset(buf, 'a', buflen = 1000);

            for( j = 0; j < 1000; j++ )
            {
                sha1_update(ctx, buf, buflen);
            }
        }
        else
        {
            sha1_update(ctx, (u8*)sha1_test_buf[i], sha1_test_buflen[i]);
        }
        sha1_final(ctx, hash);
        if (memcmp(hash->bytes, sha1_test_sum[i], SHA1_HASH_SIZE) != 0)
        {
            printf( "failed\n" );
        }
        else
        {
            printf( "passed\n" );
        }
    }
    sha1_hash_delete(&hash);
    sha1_delete(&ctx);
}

