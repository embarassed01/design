#include "algo400.h"
#include "crypt/md5.h"

// "$P$984478476IagS59wHZvyQMArzfx58u." -- hashcat
// "$H$984478476IagS59wHZvyQMArzfx58u." -- hashcat
// $H$        9      84478476    IagS59wHZvyQMArzfx58u.
// signature  iter    salt         phpass_decode --> digest
//  3         1         8            22                16       (len)
STATE_FLAG algo400_parse_hash(crack_t * crack, const char * hash, int len_hash)
{
    _alloc_t * allocator = &crack->pt_allocator;

    // 1. check signature:
    char signature_phpass_1[] = "$P$";
    char signature_phpass_2[] = "$H$";
    int signature_len = 3;   // strlen(signature)
    if ( (memcmp(hash, signature_phpass_1, strlen(signature_phpass_1)) != 0) &&
            (memcmp(hash, signature_phpass_2, strlen(signature_phpass_2)) != 0)
       )
    {
        crack->data.salt = NULL;
        crack->data.digest = NULL;
        return PARSE_HASH_SIGNATURE_ERR_RET;
    }
    // 2. hash --> signature | realhash
    char * realhash = (char *)hash + signature_len;
    // 3. len is match?
    if (len_hash != signature_len + 1 + 8 + 22)
    {
        crack->data.salt = NULL;
        crack->data.digest = NULL;
        return PARSE_HASH_LEN_ERR_RET;
    }
    // 4. is valid base64b ?
    if (is_valid_base64b_string((u8*)realhash+1+8, 22) == false)
    {
        crack->data.salt = NULL;
        crack->data.digest = NULL;
        return PARSE_HASH_BASE64_ERR_RET;
    }
    // 5. get iter and check iter: iter_len==1
    u32 iter = 1u << itoa64_to_int(realhash[0]);
    if (iter > 0x80000000)
    {
        crack->data.salt = NULL;
        crack->data.digest = NULL;
        return PARSE_HASH_ITER_ERR_RET;
    }
    crack->data.iters = iter;
    // 6. get salt: salt_len==8
    crack->data.salt_len = 8;
    crack->data.salt = (u8*)_alloc_allocate(allocator, crack->data.salt_len, 1);
    memset(crack->data.salt, 0x00, crack->data.salt_len);
    memcpy(crack->data.salt, realhash+1, crack->data.salt_len);
    // 7. realhash+1+8 --phpass_decode()--> digest
    crack->data.digest_len = 16;
    crack->data.digest = (u8*)_alloc_allocate(allocator, crack->data.digest_len, 1);
    memset(crack->data.digest, 0x00, crack->data.digest_len);
    phpass_decode(crack->data.digest, (u8*)(realhash+1+8));
    return SUCCESS_RET;
}

STATE_FLAG algo400_crack_for_test(crack_t * crack, const char * password, int len_pwd)
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
    // 2. loop iter hash = md5(hash|||pwd):
    for (int i = 0; i < crack->data.iters; i++)
    {
        md5_start(interanal);
        md5_update(interanal, hash->bytes, crack->algo.len_hash);
        md5_update(interanal, (u8*)password, len_pwd);
        md5_final(interanal, hash);
    }
    _alloc_deallocate(allocator, interanal, crack->algo.len_internal_data, 1);
    // 3. cmp out vs digest:
    if (memcmp(hash->bytes, crack->data.digest, crack->data.digest_len) != 0)
    {
        _alloc_deallocate(allocator, hash, crack->algo.len_hash, 1);
        return CRACK_TEST_ERR_RET;
    }
    _alloc_deallocate(allocator, hash, crack->algo.len_hash, 1);
    return SUCCESS_RET;
}

void       algo400_free_parse_hash(crack_t * crack)
{
    _alloc_t * allocator = &crack->pt_allocator;
    _alloc_deallocate(allocator, crack->data.salt, crack->data.salt_len, 1);
    _alloc_deallocate(allocator, crack->data.digest, crack->data.digest_len, 1);
}
