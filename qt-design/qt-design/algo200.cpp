#include "algo200.h"

// "7196759210defdc0"

STATE_FLAG algo200_parse_hash(crack_t * crack, const char * hash, int len_hash)
{
    _alloc_t * allocator = &crack->pt_allocator;

    // 1. check len:
    if (len_hash != 0x10 || len_hash % 2 != 0)
    {
        crack->data.salt = NULL;
        crack->data.digest = NULL;
        return PARSE_HASH_LEN_ERR_RET;
    }
    crack->data.digest_len = 0x10 / 2;   // 8B digest
    // 2. is valid hex?
    if (is_valid_hex_string((u8*)hash, len_hash) == false)
    {
        crack->data.salt = NULL;
        crack->data.digest = NULL;
        return PARSE_HASH_HEX_ERR_RET;
    }
    // 3. hash --hex_to_u8--> digest
    crack->data.digest = (u8*)_alloc_allocate(allocator, crack->data.digest_len, 1);
    memset(crack->data.digest, 0x00, crack->data.digest_len);
    for (int i = 0; i < crack->data.digest_len; i++)
    {
        crack->data.digest[i] = hex_to_u8((u8*)(hash+2*i));
    }
    return SUCCESS_RET;
}

STATE_FLAG algo200_crack_for_test(crack_t * crack, const char * password, int len_pwd)
{
    _alloc_t * allocator = &crack->pt_allocator;

    // 1. initial out{nr,nr2}, add:
    u8 out[8] = "5W0PqV4"; out[7] = 0x12;  // 35 57 30 50 71 56 34 12
    u32 * out32 = (u32*)out;
    u32 add = 7;
    // 2. out = loop(skip blank or tab, len_pwd, (
    //                                              nr ^=  (nr left-shift 8bit)  + ((nr's low 7bit)+add)*p[0]
    //                                              nr2 += (nr2 left-shift 8bit) ^ nr
    //                                              add += p[0]
    //                                              )
    //               )
    for (u8 * p = (u8*)password; *p; p++)
    {
        if (*p == ' ' || *p == '\t')
        {
            continue;
        }
        out32[0] = out32[0] ^       ((u32)(out32[0] << 8) + (((u32)(out32[0] & 0x3f) + add) * p[0]) );
        out32[1] = my_add(out32[1], ((u32)(out32[1]<<8)) ^ out32[0]);      // direct use '+' in unix have problem!!
        add = add + p[0];
    }
    // 3. nr  = nr & (1<<31-1)
    //    nr2 = nr2 & (1<<31-1)
    //    out = BigEnd(out)
    out32[0] = (out32[0] & (((u32)1<<31)-1));
    out32[1] = (out32[1] & (((u32)1<<31)-1));
    out32[0] = swap2bigend_32(out);
    out32[1] = swap2bigend_32(out+4);
    // 4. cmp out vs digest:
    if (memcmp(out, crack->data.digest, crack->data.digest_len) != 0)
    {
        return CRACK_TEST_ERR_RET;
    }
    return SUCCESS_RET;
}

void       algo200_free_parse_hash(crack_t * crack)
{
    _alloc_t * allocator = &crack->pt_allocator;
    _alloc_deallocate(allocator, crack->data.digest, crack->data.digest_len, 1);
}
