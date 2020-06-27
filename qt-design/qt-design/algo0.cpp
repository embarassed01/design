#include "algo0.h"
#include "crypt/md5.h"

//"8743b52063cd84097a65d1633f5c74f5" -- hashcat
// digest
STATE_FLAG algo0_parse_hash    (crack_t * crack, const char * hash,     int len_hash)
{
    _alloc_t * allocator = &crack->pt_allocator;
    //
    char * realhash = (char *)hash;
    // 1. len is match?
    if (len_hash != 16 * 2)
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
    crack->data.digest_len = 16;
    crack->data.digest = (u8*)_alloc_allocate(allocator, crack->data.digest_len, 1);
    memset(crack->data.digest, 0x00, crack->data.digest_len);
    for (int i = 0; i < crack->data.digest_len; i++)
    {
        crack->data.digest[i] = hex_to_u8((u8*)realhash+2*i);
    }
    return SUCCESS_RET;
}

STATE_FLAG algo0_crack_for_test(crack_t * crack, const char * password, int len_pwd)
{
    _alloc_t * allocator = &crack->pt_allocator;
    crack->algo.len_hash = sizeof(md5_hash_t);
    crack->algo.algo_hash = (void*)_alloc_allocate(allocator, crack->algo.len_hash, 1);
    memset((void*)crack->algo.algo_hash, 0x00, crack->algo.len_hash);
    md5_hash_t * hash = (md5_hash_t*)crack->algo.algo_hash;
    crack->algo.len_internal_data = sizeof(md5_ctx_t);
    crack->algo.algo_internal_data = (void*)_alloc_allocate(allocator, crack->algo.len_internal_data, 1);
    memset((void*)crack->algo.algo_internal_data, 0x00, crack->algo.len_internal_data);
    md5_ctx_t * interanal = (md5_ctx_t*)crack->algo.algo_internal_data;

    // 1. hash = md5(pwd):
    md5_start(interanal);
    md5_update(interanal, (u8*)password, len_pwd);
    md5_final(interanal, hash);
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

void       algo0_free_parse_hash(crack_t * crack)
{
    _alloc_t * allocator = &crack->pt_allocator;
    _alloc_deallocate(allocator, crack->data.digest, crack->data.digest_len, 1);
}
