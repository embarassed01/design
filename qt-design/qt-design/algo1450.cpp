#include "algo1450.h"
#include "crypt/hmac_sha256.h"

// "b435ffbacea34d5eb0dbc4d69a92f0152f2cf4cd364d34c2ece322ca22d8b334:21217" -- hashcat
//   digest                                                          salt
STATE_FLAG algo1450_parse_hash    (crack_t * crack, const char * hash,     int len_hash)
{
    _alloc_t * allocator = &crack->pt_allocator;
    //
    const char * sep = ":";
    char * realhash = (char *)hash;
    // 1. len is match?
    if (len_hash < 32 * 2 || len_hash > 32 * 2 + 1 + 256)
    {
        crack->data.salt = NULL;
        crack->data.digest = NULL;
        return PARSE_HASH_LEN_ERR_RET;
    }
    // 2. get digest:
    char * str_digest = strtok(realhash, sep);
    if ( (str_digest == NULL) ||
            (strlen(str_digest) != 32 * 2) )
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
    crack->data.digest_len = 32;
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

STATE_FLAG algo1450_crack_for_test(crack_t * crack, const char * password, int len_pwd)
{
    _alloc_t * allocator = &crack->pt_allocator;
    crack->algo.len_hash = sizeof(hmac_sha256_hash_t);
    crack->algo.algo_hash = (void*)_alloc_allocate(allocator, crack->algo.len_hash, 1);
    memset((void*)crack->algo.algo_hash, 0x00, crack->algo.len_hash);
    hmac_sha256_hash_t * hash = (hmac_sha256_hash_t*)crack->algo.algo_hash;
    // 1. hash = hmac_sha256(key=pwd, in=salt):
    hmac_sha256((u8*)password, len_pwd, crack->data.salt, crack->data.salt_len, hash);
    // 2. cmp out vs digest:
    if (memcmp(hash->hash.bytes, crack->data.digest, crack->data.digest_len) != 0)
    {
        _alloc_deallocate(allocator, hash, crack->algo.len_hash, 1);
        return CRACK_TEST_ERR_RET;
    }
    _alloc_deallocate(allocator, hash, crack->algo.len_hash, 1);
    return SUCCESS_RET;
}

void       algo1450_free_parse_hash(crack_t * crack)
{
    _alloc_t * allocator = &crack->pt_allocator;
    _alloc_deallocate(allocator, crack->data.salt, crack->data.salt_len, 1);
    _alloc_deallocate(allocator, crack->data.digest, crack->data.digest_len, 1);
}

