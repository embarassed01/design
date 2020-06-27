#ifndef RUNCRACK_H
#define RUNCRACK_H

#include <QObject>
#include <QThread>
#include "my_qt_common.h"

enum STATE_FLAG
{
    SUCCESS_RET,
    PARSE_HASH_SIGNATURE_ERR_RET,
    PARSE_HASH_BASE64_ERR_RET,
    PARSE_HASH_LEN_ERR_RET,
    PARSE_HASH_SALT_LEN_ERR_RET,
    PARSE_HASH_ROUNDS_LEN_ERR_RET,
    PARSE_HASH_DIGEST_LEN_ERR_RET,
    PARSE_HASH_HEX_ERR_RET,
    PARSE_HASH_ITER_ERR_RET,
    CRACK_TEST_ERR_RET,
    FAIL_RET
};

typedef struct _crack_algo_
{
    int    algo_id;
    char   algo_description[64];
    void * algo_internal_data;   // such as: sha1_ctx_t
    int    len_internal_data;
    void * algo_hash;            // result of hash  algorithm (such as: sha1, md5)
    int    len_hash;
    void * algo_block_crypt;     // result of crypt algorithm every block (such as: des, aes)
    int    len_crypt;
} crack_algo_t;

typedef struct crack_data_
{
    u32  iters;
    u8 * salt;
    int  salt_len;
    u8 * digest;
    int  digest_len;
} crack_data_t;

struct crack_
{
    // memory allocator:
    _alloc_t pt_allocator;
    // algo struct:
    crack_algo_t algo;
    // inhash data struct:
    crack_data_t data;
    // func of (crack for test):
    STATE_FLAG (* parse_hash)     (struct crack_ * crack, const char * hash,     int len_hash);
    STATE_FLAG (* crack_for_test) (struct crack_ * crack, const char * password, int len_pwd);
    void       (* free_parse_hash)(struct crack_ * crack);
    // correct pwd:
    char correct_pwd[256];
    // example pair {pwd(always is hashcat), hash}
    char example_pwd[16];
    char * example_hash;
};
typedef struct crack_ crack_t;


class RunCrack : public QObject
{
    Q_OBJECT
public:
    explicit RunCrack(QObject *parent = nullptr);
    ~RunCrack();

private:
    crack_t * crack_init_internal(int algo_id);

public:
    void crack_delete();
    void crack_init(qint32 algo_id);
    void crack_reinit();
    STATE_FLAG parse_hash(const char * hash,     int len_hash);
    STATE_FLAG crack_for_test(const char * password, int len_pwd);
    void free_parse_hash();
    QString get_example_pwd();
    QString get_example_hash();
    QString get_correct_pwd();

signals:
    void sig_done_crack_force(qint8 is_success);
    void sig_realtime_crack_force_info(QString info);

public slots:
    void slot_crack_force(qint16 minlen, qint16 maxlen);   // default all seechars(95)

private:
    QThread t;
    crack_t * m_crack;
    int m_algoid;
};

#endif // RUNCRACK_H
