#include "algo1400.h"
#include "crypt/sha256.h"

// "127e6fbfe24a750e72930c220a8e138275656b8e5d8f48a98c3c92df2caba935" -- hashcat
//    digest  len==32*2
STATE_FLAG algo1400_parse_hash    (crack_t * crack, const char * hash,     int len_hash)
{
    _alloc_t * allocator = &crack->pt_allocator;
    //
    char * realhash = (char *)hash;
    // 1. len is match?
    if (len_hash != 32 * 2)
    {
        crack->data.salt = NULL;
        crack->data.digest = NULL;
        return PARSE_HASH_LEN_ERR_RET;
    }
    // 2. is valid base64b ?
    if (is_valid_hex_string((u8*)realhash, len_hash) == false)
    {
        crack->data.salt = NULL;
        crack->data.digest = NULL;
        return PARSE_HASH_BASE64_ERR_RET;
    }
    // 3. hash --hex_to_u8--> digest
    crack->data.digest_len = 32;
    crack->data.digest = (u8*)_alloc_allocate(allocator, crack->data.digest_len, 1);
    memset(crack->data.digest, 0x00, crack->data.digest_len);
    for (int i = 0; i < crack->data.digest_len; i++)
    {
        crack->data.digest[i] = hex_to_u8((u8*)realhash+2*i);
    }
    return SUCCESS_RET;
}

STATE_FLAG algo1400_crack_for_test(crack_t * crack, const char * password, int len_pwd)
{
    _alloc_t * allocator = &crack->pt_allocator;
    crack->algo.len_hash = sizeof(sha256_hash_t);
    crack->algo.algo_hash = (void*)_alloc_allocate(allocator, crack->algo.len_hash, 1);
    memset((void*)crack->algo.algo_hash, 0x00, crack->algo.len_hash);
    sha256_hash_t * hash = (sha256_hash_t*)crack->algo.algo_hash;
    crack->algo.len_internal_data = sizeof(sha256_ctx_t);
    crack->algo.algo_internal_data = (void*)_alloc_allocate(allocator, crack->algo.len_internal_data, 1);
    memset((void*)crack->algo.algo_internal_data, 0x00, crack->algo.len_internal_data);
    sha256_ctx_t * interanal = (sha256_ctx_t*)crack->algo.algo_internal_data;

    // 1. hash = sha256(pwd):
    sha256_start(interanal);
    sha256_update(interanal, (u8*)password, len_pwd);
    sha256_final(interanal, hash);
    _alloc_deallocate(allocator, interanal, crack->algo.len_internal_data, 1);
    // 2. cmp out vs digest:
    if (memcmp(hash->bytes, crack->data.digest, crack->data.digest_len) != 0)
    {
        _alloc_deallocate(allocator, hash, crack->algo.len_hash, 1);
        return CRACK_TEST_ERR_RET;
    }
    _alloc_deallocate(allocator, hash, crack->algo.len_hash, 1);
    return SUCCESS_RET;
}

void       algo1400_free_parse_hash(crack_t * crack)
{
    _alloc_t * allocator = &crack->pt_allocator;
    _alloc_deallocate(allocator, crack->data.digest, crack->data.digest_len, 1);
}

