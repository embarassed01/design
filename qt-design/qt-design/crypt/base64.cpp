#include "base64.h"

static const u8 base64_enc_map[64] =
{
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
    'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
    'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', '+', '/'
};

static const u8 base64_dec_map[128] =
{
    127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
    127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
    127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
    127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
    127, 127, 127,  62, 127, 127, 127,  63,  52,  53,
     54,  55,  56,  57,  58,  59,  60,  61, 127, 127,
    127,  64, 127, 127, 127,   0,   1,   2,   3,   4,
      5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
     15,  16,  17,  18,  19,  20,  21,  22,  23,  24,
     25, 127, 127, 127, 127, 127, 127,  26,  27,  28,
     29,  30,  31,  32,  33,  34,  35,  36,  37,  38,
     39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
     49,  50,  51, 127, 127, 127, 127, 127
};


int base64_encode(u8 * out, int * outlen, const u8 * in, int inlen)
{
    int i, n;
    int C1, C2, C3;
    unsigned char *p;

    n = inlen/3 + (inlen%3 != 0);

    n *= 4;

    n = (inlen/3)*3;

    for (i = 0, p = out; i < n; i += 3)
    {
        C1 = *in++;
        C2 = *in++;
        C3 = *in++;

        *p++ = base64_enc_map[(C1 >> 2) & 0x3F];
        *p++ = base64_enc_map[(((C1 &  3) << 4) + (C2 >> 4)) & 0x3F];
        *p++ = base64_enc_map[(((C2 & 15) << 2) + (C3 >> 6)) & 0x3F];
        *p++ = base64_enc_map[C3 & 0x3F];
    }

    if (i < inlen)
    {
        C1 = *in++;
        C2 = ((i+1) < inlen) ? *in++ : 0;

        *p++ = base64_enc_map[(C1 >> 2) & 0x3F];
        *p++ = base64_enc_map[(((C1 & 3) << 4) + (C2 >> 4)) & 0x3F];

        if ((i + 1) < inlen)
        {
            *p++ = base64_enc_map[((C2 & 15) << 2) & 0x3F];
        }
        else
        {
            *p++ = '=';
        }
        *p++ = '=';
    }

    *outlen = p - out;
    *p = 0;
    return 0;
}

int base64_decode(u8 * out, int * outlen, const u8 * in, int inlen)
{
    int i, n;
    uint32_t j, x;
    u8 * p;
    for (i = n = j = 0; i < inlen; i++)
    {
        x = 0;
        while (i < inlen && in[i] == ' ')
        {
            ++i;
            ++x;
        }
        if (i == inlen)
        {
            break;
        }
        if ((inlen - i) >= 2 && in[i] == '\r' && in[i + 1] == '\n')
        {
            continue;
        }
        if (in[i] == '\n')
        {
            continue;
        }
        if (x != 0)
        {
            return -1;      //ERROR_HAVE_BLANK
        }
        if (in[i] == '=' && ++j > 2)
        {
            return  -1;     //ERR_BASE64_INVALID_CHARACTER
        }
        if (in[i] > 127 || base64_dec_map[in[i]] == 127)
        {
            return -1;     //ERR_BASE64_INVALID_CHARACTER
        }
        if (base64_dec_map[in[i]] < 64 && j != 0)
        {
            return -1;     //ERR_BASE64_INVALID_CHARACTER
        }
        n++;
    }
    if (n == 0)
    {
        *outlen = 0;
        return 0;
    }
    n = (( n * 6 ) + 7)>>3;
    n -= j;
    for (j = 3, n = x = 0, p = out; i > 0; i--, in++ )
    {
        if (*in == '\r' || *in == '\n' || *in == ' ')
        {
            continue;
        }
        j -= ( base64_dec_map[*in] == 64 );
        x = (x << 6) | ( base64_dec_map[*in] & 0x3F );
        if (++n == 4)
        {
            n = 0;
            if (j > 0)
            {
                *p++ = (u8)(x>>16);
            }
            if (j > 1)
            {
                *p++ = (u8)(x>>8);
            }
            if (j > 2)
            {
                *p++ = (u8)(x);
            }
        }
    }
    *outlen = p - out;
    return 0;
}

static const unsigned char base64_test_dec[64] =
{
    0x24, 0x48, 0x6E, 0x56, 0x87, 0x62, 0x5A, 0xBD,
    0xBF, 0x17, 0xD9, 0xA2, 0xC4, 0x17, 0x1A, 0x01,
    0x94, 0xED, 0x8F, 0x1E, 0x11, 0xB3, 0xD7, 0x09,
    0x0C, 0xB6, 0xE9, 0x10, 0x6F, 0x22, 0xEE, 0x13,
    0xCA, 0xB3, 0x07, 0x05, 0x76, 0xC9, 0xFA, 0x31,
    0x6C, 0x08, 0x34, 0xFF, 0x8D, 0xC2, 0x6C, 0x38,
    0x00, 0x43, 0xE9, 0x54, 0x97, 0xAF, 0x50, 0x4B,
    0xD1, 0x41, 0xBA, 0x95, 0x31, 0x5A, 0x0B, 0x97
};

static const unsigned char base64_test_enc[] =
    "JEhuVodiWr2/F9mixBcaAZTtjx4Rs9cJDLbpEG8i7hPK"
    "swcFdsn6MWwINP+Nwmw4AEPpVJevUEvRQbqVMVoLlw==";

int base64_self_test()
{
    int len;
    const u8 * src;
    u8 buffer[128];
    src = base64_test_dec;
    if (base64_encode(buffer, &len, src, 64) != 0 || memcmp(base64_test_enc, buffer, 88) != 0)
    {
        return 1;
    }
    src = base64_test_enc;
    if (base64_decode(buffer, &len, src, 88) != 0 || memcmp( base64_test_dec, buffer, 64) != 0)
    {
        return 1;
    }
    return 0;
}
