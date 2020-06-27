#include "algo111.h"
#include "crypt/base64.h"
#include "crypt/sha1.h"

// "{SSHA}AZKja92fbuuB9SpRlHqaoXxbTc43Mzc2MDM1Ng=="
//  signature base64()

STATE_FLAG algo111_parse_hash(crack_t * crack, const char * hash, int len_hash)
{
    _alloc_t * allocator = &crack->pt_allocator;

    // 1. check signature:
    char signature[] = "{SSHA}";
    if ( (memcmp(hash, signature, strlen(signature)) != 0) &&
            ({lowercase((u8*)signature, strlen(signature)); memcmp(hash, signature, strlen(signature)) != 0;})
       )
    {
        crack->data.salt = NULL;
        crack->data.digest = NULL;
        return PARSE_HASH_SIGNATURE_ERR_RET;
    }
    // 2. hash --> signature | realhash
    char * realhash = (char *)hash + strlen(signature);
    // 3. is valid base64?
    if (is_valid_base64_string((u8*)realhash, len_hash-strlen(signature)) == false)
    {
        crack->data.salt = NULL;
        crack->data.digest = NULL;
        return PARSE_HASH_BASE64_ERR_RET;
    }
    // 3. realhash --base64--> decoded_hash
    char * decoded_hash = (char*)_alloc_allocate(allocator, len_hash-strlen(signature), 1);
    memset(decoded_hash, 0x00, len_hash-strlen(signature));
    int decode_len = 0;
    base64_decode((u8*)decoded_hash, &decode_len, (u8*)realhash, len_hash-strlen(signature));
    // 4. check len:
    if (decode_len < 20)
    {
        _alloc_deallocate(allocator, decoded_hash, len_hash-strlen(signature), 1);
        crack->data.salt = NULL;
        crack->data.digest = NULL;
        return PARSE_HASH_LEN_ERR_RET;
    }
    // 5. decoded_hash == digest 20B | salt
    crack->data.digest = (u8*)_alloc_allocate(allocator, 20, 1);
    memset(crack->data.digest, 0x00, 20);
    memcpy(crack->data.digest, decoded_hash, 20);
    crack->data.digest_len = 20;         // 0x14
    // 6. salt --> p_crackdata->salt
    int salt_len = decode_len - 20;
    crack->data.salt = (u8*)_alloc_allocate(allocator, salt_len, 1);
    memset(crack->data.salt, 0x00, salt_len);
    memcpy(crack->data.salt, decoded_hash+20, salt_len);
    crack->data.salt_len = salt_len;

    _alloc_deallocate(allocator, decoded_hash, len_hash-strlen(signature), 1);
    return SUCCESS_RET;
}


STATE_FLAG algo111_crack_for_test(crack_t * crack, const char * password, int len_pwd)
{
    _alloc_t * allocator = &crack->pt_allocator;
    crack->algo.len_hash = sizeof(sha1_hash_t);
    crack->algo.algo_hash = (void*)_alloc_allocate(allocator, crack->algo.len_hash, 1);
    memset(crack->algo.algo_hash, 0x00, crack->algo.len_hash);
    sha1_hash_t * hash = (sha1_hash_t*)crack->algo.algo_hash;
    crack->algo.len_internal_data = sizeof(sha1_ctx_t);
    crack->algo.algo_internal_data = (void*)_alloc_allocate(allocator, crack->algo.len_internal_data, 1);
    memset(crack->algo.algo_internal_data, 0x00, crack->algo.len_internal_data);
    sha1_ctx_t * internal = (sha1_ctx_t*)crack->algo.algo_internal_data;

    // 1. hash = sha1(pwd|||salt)
    sha1_start(internal);
    sha1_update(internal, (u8*)password, len_pwd);
    sha1_update(internal, (u8*)crack->data.salt, crack->data.salt_len);
    sha1_final(internal, hash);
    _alloc_deallocate(allocator, internal, crack->algo.len_internal_data, 1);
    // 2. cmp hash vs digest:
    if (memcmp(hash->bytes, crack->data.digest, crack->data.digest_len) != 0)
    {
        _alloc_deallocate(allocator, hash, crack->algo.len_hash, 1);
        return CRACK_TEST_ERR_RET;
    }
    _alloc_deallocate(allocator, hash, crack->algo.len_hash, 1);
    return SUCCESS_RET;
}

void       algo111_free_parse_hash(crack_t * crack)
{
    _alloc_t * allocator = &crack->pt_allocator;
    _alloc_deallocate(allocator, crack->data.salt, crack->data.salt_len, 1);
    _alloc_deallocate(allocator, crack->data.digest, crack->data.digest_len, 1);
}

