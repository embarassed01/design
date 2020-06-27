#include "algo500.h"
#include "crypt/md5.h"

// "$1$28772684$iEwNOgGugqO9.bIz5sk8k/" -- hashcat
//  $1$   28772684   $    iEwNOgGugqO9.bIz5sk8k/
//         salt             md5crypt_decode()-->digest

STATE_FLAG algo500_parse_hash(crack_t * crack, const char * hash, int len_hash)
{
    _alloc_t * allocator = &crack->pt_allocator;

    // 1. check signature:
    char signature[] = "$1$";
    int signature_len = strlen(signature);
    if ( (memcmp(hash, signature, signature_len) != 0)
       )
    {
        crack->data.salt = NULL;
        crack->data.digest = NULL;
        return PARSE_HASH_SIGNATURE_ERR_RET;
    }
    // 2. hash --> signature | realhash
    char * realhash = (char *)hash + signature_len;
    // 3. len is match?
    if (len_hash < signature_len + 0 + 1 + 22 || len_hash > signature_len + 8 + 1 + 22)
    {
        crack->data.salt = NULL;
        crack->data.digest = NULL;
        return PARSE_HASH_LEN_ERR_RET;
    }
    // 4. split realhash -->          salt $ md5crypt_encode(digest)
    //         or        --> rounds=X $ md5crypt_encode(digest)
    char * str_rounds_salt = strtok(realhash, "$");
    if ( (str_rounds_salt == NULL) ||
         (strlen(str_rounds_salt) > 8)
         )
    {
        crack->data.salt = NULL;
        crack->data.digest = NULL;
        return PARSE_HASH_SALT_LEN_ERR_RET;
    }
    // 5. is rounds=X$ ? or salt?
    u8 tmp_vs_rounds_signature[7] = {0x00};
    if ((memcmp(str_rounds_salt, "rounds=", 7) == 0) ||
            ({
             memcpy(tmp_vs_rounds_signature, realhash, 7);
             memcmp(tmp_vs_rounds_signature, "ROUNDS=", 7) == 0;})
            )
    {   // is rounds=X:
        if (strlen(str_rounds_salt) != 8)
        {
            crack->data.salt = NULL;
            crack->data.digest = NULL;
            return PARSE_HASH_ROUNDS_LEN_ERR_RET;
        }
        crack->data.iters = atoi(str_rounds_salt+7);
        crack->data.salt = NULL;
        crack->data.salt_len = 0;
    }
    else
    {  // is salt:
        crack->data.iters = 1000;
        crack->data.salt_len = strlen(str_rounds_salt);
        crack->data.salt = (u8*)_alloc_allocate(allocator, crack->data.salt_len, 1);
        memset((void*)crack->data.salt, 0x00, crack->data.salt_len);
        memcpy(crack->data.salt, str_rounds_salt, crack->data.salt_len);
    }
    // 6. get encode_digest:
    char * encode_digest = strtok(NULL, "$");
    if ( (encode_digest == NULL) ||
         (strlen(encode_digest) != 22)
         )
    {
        _alloc_deallocate(allocator, crack->data.salt, crack->data.salt_len, 1);
        crack->data.salt = NULL;
        crack->data.digest = NULL;
        return PARSE_HASH_DIGEST_LEN_ERR_RET;
    }
    // 7. encode_digest is valid base64b ?
    if (is_valid_base64b_string((u8*)encode_digest, 22) == false)
    {
        _alloc_deallocate(allocator, crack->data.salt, crack->data.salt_len, 1);
        crack->data.salt = NULL;
        crack->data.digest = NULL;
        return PARSE_HASH_BASE64_ERR_RET;
    }
    // 8. encode_digest --md5crypt_decode()--> digest
    crack->data.digest_len = 16;
    crack->data.digest = (u8*)_alloc_allocate(allocator, crack->data.digest_len, 1);
    memset(crack->data.digest, 0x00, crack->data.digest_len);
    md5crypt_decode(crack->data.digest, (u8*)encode_digest);
    return SUCCESS_RET;
}

STATE_FLAG algo500_crack_for_test(crack_t * crack, const char * password, int len_pwd)
{
    _alloc_t * allocator = &crack->pt_allocator;
    crack->algo.len_hash = sizeof(md5_hash_t);
    crack->algo.algo_hash = (void*)_alloc_allocate(allocator, crack->algo.len_hash, 1);
    memset((void*)crack->algo.algo_hash, 0x00, crack->algo.len_hash);
    md5_hash_t * hash = (md5_hash_t*)crack->algo.algo_hash;
    crack->algo.len_internal_data = sizeof(md5_ctx_t);
    crack->algo.algo_internal_data = (void*)_alloc_allocate(allocator, crack->algo.len_internal_data, 1);
    memset((void*)crack->algo.algo_internal_data, 0x00, crack->algo.len_internal_data);
    md5_ctx_t * internal = (md5_ctx_t*)crack->algo.algo_internal_data;

    // 1. hash = md5(pwd, salt, pwd)
    md5_start(internal);
    md5_update(internal, (u8*)password, len_pwd);
    md5_update(internal, crack->data.salt, crack->data.salt_len);
    md5_update(internal, (u8*)password, len_pwd);
    md5_final(internal, hash);
    // 2. hash = md5(pwd, $1$, salt, loop?(hash), loop?(pwd[0]))
    char sigature[] = "$1$";
    int pl = 0;
    int ph = 0;
    u8 mh[1] = {0x00};
    md5_start(internal);
    md5_update(internal, (u8*)password, len_pwd);
    md5_update(internal, (u8*)sigature, strlen(sigature));
    md5_update(internal, crack->data.salt, crack->data.salt_len);
    for (pl = len_pwd; pl > 16; pl -= 16)
    {
        md5_update(internal, hash->bytes, crack->algo.len_hash);
    }
    md5_update(internal, hash->bytes, pl);
    for (ph = len_pwd; ph != 0; ph >>= 1)
    {
        if (ph & 0x01)
        {
            mh[0] = 0x00;
        }
        else
        {
            mh[0] = password[0] & 0xff;
        }
        md5_update(internal, mh, 1);
    }
    md5_final(internal, hash);
    // 3. lp = 0--iters; hash = loop(iters, md5( lp even?pwd:hash,
    //                                          lp %3!=0?salt:null,
    //                                          lp %7!=0?pwd:null,
    //                                          lp even?hash:pwd
    //                                        )
    //                              )
    for (int lp = 0; lp < crack->data.iters; lp++)
    {
        md5_start(internal);
        if (lp & 0x01)
        {
            md5_update(internal, (u8*)password, len_pwd);
        }
        else
        {
            md5_update(internal, hash->bytes, crack->algo.len_hash);
        }
        if (lp % 3 != 0)
        {
            md5_update(internal, crack->data.salt, crack->data.salt_len);
        }
        if (lp % 7 != 0)
        {
            md5_update(internal, (u8*)password, len_pwd);
        }
        if (lp & 0x01)
        {
            md5_update(internal, hash->bytes, crack->algo.len_hash);
        }
        else
        {
            md5_update(internal, (u8*)password, len_pwd);
        }
        md5_final(internal, hash);
    }
    _alloc_deallocate(allocator, internal, crack->algo.len_internal_data, 1);
    // 4. cmp hash vs digest:
    if (memcmp(hash->bytes, crack->data.digest, crack->data.digest_len) != 0)
    {
        _alloc_deallocate(allocator, hash, crack->algo.len_hash, 1);
        return CRACK_TEST_ERR_RET;
    }
    _alloc_deallocate(allocator, hash, crack->algo.len_hash, 1);
    return SUCCESS_RET;
}

void algo500_free_parse_hash     (crack_t * crack)
{
    _alloc_t * allocator = &crack->pt_allocator;
    _alloc_deallocate(allocator, crack->data.salt, crack->data.salt_len, 1);
    _alloc_deallocate(allocator, crack->data.digest, crack->data.digest_len, 1);
}

