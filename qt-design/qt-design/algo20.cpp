#include "algo20.h"
#include "crypt/md5.h"

// "57ab8499d08c59a7211c77f557bf9425:4247" -- hashcat
//   digest                           salt
//   len==16*2
STATE_FLAG algo20_parse_hash    (crack_t * crack, const char * hash,     int len_hash)
{
    _alloc_t * allocator = &crack->pt_allocator;
    //
    const char * sep = ":";
    char * realhash = (char *)hash;
    // 1. len is match?
    if (len_hash < 16 * 2 || len_hash > 16 * 2 + 1 + 256)
    {
        crack->data.salt = NULL;
        crack->data.digest = NULL;
        return PARSE_HASH_LEN_ERR_RET;
    }
    // 2. get digest:
    char * str_digest = strtok(realhash, sep);
    if ( (str_digest == NULL) ||
            (strlen(str_digest) != 16 * 2) )
    {
        crack->data.salt = NULL;
        crack->data.digest = NULL;
        return PARSE_HASH_DIGEST_LEN_ERR_RET;
    }
    // 3. digest is valid hex ?
    if (is_valid_hex_string((u8*)str_digest, strlen(str_digest)) == false)
    {
        crack->data.salt = NULL;
        crack->data.digest = NULL;
        return PARSE_HASH_HEX_ERR_RET;
    }
    // 4. hash --hex_to_u8--> digest
    crack->data.digest_len = 16;
    crack->data.digest = (u8*)_alloc_allocate(allocator, crack->data.digest_len, 1);
    memset(crack->data.digest, 0x00, crack->data.digest_len);
    for (int i = 0; i < crack->data.digest_len; i++)
    {
        crack->data.digest[i] = hex_to_u8((u8*)str_digest+2*i);
    }
    // 5. get salt:
    char * str_salt = strtok(NULL, sep);
    if ((str_salt == NULL) ||
         (strlen(str_salt) > 256) )
    {
        _alloc_deallocate(allocator, crack->data.digest, crack->data.digest_len, 1);
        crack->data.digest = NULL;
        crack->data.salt = NULL;
        return PARSE_HASH_SALT_LEN_ERR_RET;
    }
    crack->data.salt_len = strlen(str_salt);
    crack->data.salt = (u8*)_alloc_allocate(allocator, crack->data.salt_len, 1);
    memset(crack->data.salt, 0x00, crack->data.salt_len);
    memcpy(crack->data.salt, str_salt, crack->data.salt_len);
    return SUCCESS_RET;
}

STATE_FLAG algo20_crack_for_test(crack_t * crack, const char * password, int len_pwd)
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

    // 1. hash = md5(salt|||pwd):
    md5_start(interanal);
    md5_update(interanal, crack->data.salt, crack->data.salt_len);
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

void       algo20_free_parse_hash(crack_t * crack)
{
    _alloc_t * allocator = &crack->pt_allocator;
    _alloc_deallocate(allocator, crack->data.salt, crack->data.salt_len, 1);
    _alloc_deallocate(allocator, crack->data.digest, crack->data.digest_len, 1);
}
