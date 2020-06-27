#include "algo300.h"
#include "crypt/sha1.h"

// "fcf7c1b8749cf99d88e5f34271d636178fb5d130" -- hashcat
STATE_FLAG algo300_parse_hash(crack_t * crack, const char * hash, int len_hash)
{
    _alloc_t * allocator = &crack->pt_allocator;

    // 1. check len:
    if (len_hash != 40 || len_hash % 2 != 0)
    {
        crack->data.salt = NULL;
        crack->data.digest = NULL;
        return PARSE_HASH_LEN_ERR_RET;
    }
    crack->data.digest_len = 40 / 2;   // 0x14
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
        crack->data.digest[i] = hex_to_u8((u8*)hash+2*i);
    }
    return SUCCESS_RET;
}

STATE_FLAG algo300_crack_for_test(crack_t * crack, const char * password, int len_pwd)
{
    _alloc_t * allocator = &crack->pt_allocator;
    crack->algo.len_hash = sizeof(sha1_hash_t);
    crack->algo.algo_hash = (void*)_alloc_allocate(allocator, crack->algo.len_hash, 1);
    memset((void*)crack->algo.algo_hash, 0x00, crack->algo.len_hash);
    sha1_hash_t * hash = (sha1_hash_t*)crack->algo.algo_hash;
    crack->algo.len_internal_data = sizeof(sha1_ctx_t);
    crack->algo.algo_internal_data = (void*)_alloc_allocate(allocator, crack->algo.len_internal_data, 1);
    memset((void*)crack->algo.algo_internal_data, 0x00, crack->algo.len_internal_data);
    sha1_ctx_t * internal = (sha1_ctx_t*)crack->algo.algo_internal_data;

    // 1. hash = sha1(pwd)
    sha1_start(internal);
    sha1_update(internal, (u8*)password, len_pwd);
    sha1_final(internal, hash);
    // 2. hash = sha1(hash)
    sha1_start(internal);
    sha1_update(internal, (u8*)hash->bytes, crack->algo.len_hash);
    sha1_final(internal, hash);
    _alloc_deallocate(allocator, internal, crack->algo.len_internal_data, 1);
    // 3. cmp hash.bytes vs digest:
    if (memcmp(hash->bytes, crack->data.digest, crack->data.digest_len) != 0)
    {
        _alloc_deallocate(allocator, hash, crack->algo.len_hash, 1);
        return CRACK_TEST_ERR_RET;
    }
    _alloc_deallocate(allocator, hash, crack->algo.len_hash, 1);
    return SUCCESS_RET;
}

void       algo300_free_parse_hash(crack_t * crack)
{
    _alloc_t * allocator = &crack->pt_allocator;
    _alloc_deallocate(allocator, crack->data.digest, crack->data.digest_len, 1);
}

