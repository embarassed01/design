#include "algo1100.h"
#include "crypt/md4.h"


// "4dd8965d1d476fa0d026722989a6b772:3060147285011" -- hashcat
//  hex-digest                      : salt

STATE_FLAG algo1100_parse_hash    (crack_t * crack, const char * hash,     int len_hash)
{
    _alloc_t * allocator = &crack->pt_allocator;

    // 1. separator to hex-digest:
    char * digest = strtok((char*)hash, ":");
    if (digest == NULL || strlen(digest) != 32)
    {
        crack->data.digest = NULL;
        crack->data.salt = NULL;
        return PARSE_HASH_LEN_ERR_RET;
    }
    // 2. separator to salt:
    char * salt = strtok(NULL, ":");
    if (strlen(salt) > 256)
    {
        crack->data.salt = NULL;
        crack->data.digest = NULL;
        return PARSE_HASH_SALT_LEN_ERR_RET;
    }
    // 3. -->digest:
    crack->data.digest_len = 32 / 2;
    crack->data.digest = (u8*)_alloc_allocate(allocator, crack->data.digest_len, 1);
    memset(crack->data.digest, 0x00, crack->data.digest_len);
    u32 * digest32 = (u32*)crack->data.digest;
    digest32[0] = hex_to_u32((u8*)(digest + 0));
    digest32[1] = hex_to_u32((u8*)(digest + 8));
    digest32[2] = hex_to_u32((u8*)(digest + 16));
    digest32[3] = hex_to_u32((u8*)(digest + 24));
    // 4. -->salt:
    crack->data.salt_len = strlen(salt);
    crack->data.salt = (u8*)_alloc_allocate(allocator, crack->data.salt_len, 1);
    memset(crack->data.salt, 0x00, crack->data.salt_len);
    memcpy(crack->data.salt, salt, crack->data.salt_len);
    return SUCCESS_RET;
}

STATE_FLAG algo1100_crack_for_test(crack_t * crack, const char * password, int len_pwd)
{
    _alloc_t * allocator = &crack->pt_allocator;
    crack->algo.len_hash = sizeof(md4_hash_t);
    crack->algo.algo_hash = (void*)_alloc_allocate(allocator, crack->algo.len_hash, 1);
    memset(crack->algo.algo_hash, 0x00, crack->algo.len_hash);
    md4_hash_t * hash = (md4_hash_t*)crack->algo.algo_hash;
    crack->algo.len_internal_data = sizeof(md4_ctx_t);
    crack->algo.algo_internal_data = (void*)_alloc_allocate(allocator, crack->algo.len_internal_data, 1);
    memset(crack->algo.algo_internal_data, 0x00, crack->algo.len_internal_data);
    md4_ctx_t * internal = (md4_ctx_t*)crack->algo.algo_internal_data;

    // 1. hash = md4(unicode(pwd))
    char * unicode_pwd = (char*)_alloc_allocate(allocator, len_pwd*2, 1);
    memset(unicode_pwd, 0x00, len_pwd*2);
    int len_unicodepwd = string_to_unicode((char*)password, len_pwd, unicode_pwd);
    md4_start(internal);
    md4_update(internal, (u8*)unicode_pwd, len_unicodepwd);
    md4_final(internal, hash);
    _alloc_deallocate(allocator, unicode_pwd, len_pwd*2, 1);
    // 2. hash = md4(hash|||unicode(salt))
    char * unicode_salt = (char*)_alloc_allocate(allocator, crack->data.salt_len*2, 1);
    memset(unicode_salt, 0x00, crack->data.salt_len*2);
    int len_unicodesalt = string_to_unicode((char*)crack->data.salt, crack->data.salt_len, unicode_salt);
    md4_start(internal);
    md4_update(internal, hash->bytes, crack->algo.len_hash);
    md4_update(internal, (u8*)unicode_salt, len_unicodesalt);
    md4_final(internal, hash);
    _alloc_deallocate(allocator, unicode_salt, crack->data.salt_len*2, 1);
    _alloc_deallocate(allocator, internal, crack->algo.len_internal_data, 1);
    // 3. cmp hash vs digest:
    if (memcmp(hash->bytes, crack->data.digest, crack->data.digest_len) != 0)
    {
        _alloc_deallocate(allocator, hash, crack->algo.len_hash, 1);
        return CRACK_TEST_ERR_RET;
    }
    _alloc_deallocate(allocator, hash, crack->algo.len_hash, 1);
    return SUCCESS_RET;
}

void       algo1100_free_parse_hash(crack_t * crack)
{
    _alloc_t * allocator = &crack->pt_allocator;
    _alloc_deallocate(allocator, crack->data.digest, crack->data.digest_len, 1);
    _alloc_deallocate(allocator, crack->data.salt, crack->data.salt_len, 1);
}
