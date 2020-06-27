#include "algo12900.h"
#include "crypt/pbkdf2_sha256.h"
#include "crypt/hmac_sha256.h"

// "15738301074686823451275227041071157383010746868234512752270410712bc4be900bf96ccf43c9852fff49b5f5874a9f6e7bf301686fa6d98286de151f15738301074686823451275227041071"  -- hashcat
//1573830107468682345127522704107115738301074686823451275227041071: salt2 len==32*2
//2bc4be900bf96ccf43c9852fff49b5f5874a9f6e7bf301686fa6d98286de151f: digest len==32*2
//15738301074686823451275227041071: salt1 len==16*2
// salt_len = 32 + 16 = 48
// iters = 4096
STATE_FLAG algo12900_parse_hash    (crack_t * crack, const char * hash,     int len_hash)
{
    _alloc_t * allocator = &crack->pt_allocator;
    // 1. hash --> realhash
    char * realhash = (char*)hash;
    // 2. len ?
    if (len_hash != 32*2 + 32*2 + 16*2)
    {
        crack->data.digest = NULL;
        crack->data.digest_len = 0;
        crack->data.salt = NULL;
        crack->data.salt_len = 0;
        return PARSE_HASH_LEN_ERR_RET;
    }
    // 3. is hex?
    if (!is_valid_hex_string((u8*)realhash, len_hash))
    {
        crack->data.digest = NULL;
        crack->data.digest_len = 0;
        crack->data.salt = NULL;
        crack->data.salt_len = 0;
        return PARSE_HASH_HEX_ERR_RET;
    }
    // 4. realhash == salt2_32B ||| digest_32B ||| salt1_16B
    //     salt == salt2 ||| salt1
    // salt1
    crack->data.salt_len = 32 + 16;
    crack->data.salt = (u8*)_alloc_allocate(allocator, crack->data.salt_len, 1);
    memset(crack->data.salt, 0x00, crack->data.salt_len);
    for (int i = 0; i < 32; i++)
    {
        crack->data.salt[i] = hex_to_u8((u8*)(realhash+i*2));
    }
    // digest
    crack->data.digest_len = 32;
    crack->data.digest = (u8*)_alloc_allocate(allocator, crack->data.digest_len, 1);
    memset(crack->data.digest, 0x00, crack->data.digest_len);
    for (int i = 0; i < 32; i++)
    {
        crack->data.digest[i] = hex_to_u8((u8*)(realhash+32*2+i*2));
    }
    // salt2
    for (int i = 0; i < 16; i++)
    {
        crack->data.salt[32+i] = hex_to_u8((u8*)(realhash+32*2+32*2+i*2));
    }
    // 5. iters == 4096
    crack->data.iters = 4096;
    return SUCCESS_RET;
}

STATE_FLAG algo12900_crack_for_test(crack_t * crack, const char * password, int len_pwd)
{
    _alloc_t * allocator = &crack->pt_allocator;
    crack->algo.len_hash = sizeof(hmac_sha256_hash_t);
    crack->algo.algo_hash = (void*)_alloc_allocate(allocator, crack->algo.len_hash, 1);
    memset(crack->algo.algo_hash, 0x00, crack->algo.len_hash);
    hmac_sha256_hash_t * hash = (hmac_sha256_hash_t*)crack->algo.algo_hash;

    // 1. master_key = pbkdf2_sha256(password, salt1, iters):
    u8 master_key[32] = {0x00};
    pbkdf2_sha256((u8*)password, len_pwd, crack->data.salt+32, 16, crack->data.iters, master_key, 32);
    // 2. hash = hmac_sha256(master_key, salt2) vs digest:
    hmac_sha256(master_key, 32, crack->data.salt, 32, hash);
    //
    if (memcmp(hash->hash.bytes, crack->data.digest, crack->data.digest_len) != 0)
    {
        _alloc_deallocate(allocator, hash, crack->algo.len_hash, 1);
        return FAIL_RET;
    }
    _alloc_deallocate(allocator, hash, crack->algo.len_hash, 1);
    return SUCCESS_RET;
}

void       algo12900_free_parse_hash(crack_t * crack)
{
    _alloc_t * allocator = &crack->pt_allocator;
    _alloc_deallocate(allocator, crack->data.salt, crack->data.salt_len, 1);
    _alloc_deallocate(allocator, crack->data.digest, crack->data.digest_len, 1);
}
