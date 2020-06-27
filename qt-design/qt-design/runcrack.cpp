#include "runcrack.h"
#include "interface_crack.h"
#include "maskprocessor.h"
#include <QDebug>
#include <QTime>

RunCrack::RunCrack(QObject *parent) : QObject(parent)
{
    //
    m_crack = NULL;
    m_algoid = 0;
    //
    //
    moveToThread(&t);
    t.start();
}

RunCrack::~RunCrack()
{
    //
    crack_delete();
    //
    t.quit();
    t.wait();
}

crack_t * RunCrack::crack_init_internal(int algo_id)
{
    crack_t * crack = (crack_t *)malloc( sizeof(crack_t) );
    memset((void*)crack, 0x00, sizeof(crack_t));
    // initial memory allocator:
    _alloc_init(&crack->pt_allocator);
    // other initial:
    m_algoid = algo_id;
    crack->algo.algo_id = algo_id;
    switch (crack->algo.algo_id) {
    case 0:   // "8743b52063cd84097a65d1633f5c74f5"
                //"MD5"
        strcpy(crack->algo.algo_description, ALGO0_DESCRIPTION);
        crack->parse_hash = algo0_parse_hash;
        crack->crack_for_test = algo0_crack_for_test;
        crack->free_parse_hash = algo0_free_parse_hash;
        strcpy(crack->example_pwd, ALGO0_EXAMPLE_PWD);
        crack->example_hash = (char*)malloc( strlen(ALGO0_EXAMPLE_HASH)+128 );
        strcpy(crack->example_hash, ALGO0_EXAMPLE_HASH);
        break;
    case 10:   // "3d83c8e717ff0e7ecfe187f088d69954:343141"
                //"md5($pass.$salt)"
        strcpy(crack->algo.algo_description, ALGO10_DESCRIPTION);
        crack->parse_hash = algo10_parse_hash;
        crack->crack_for_test = algo10_crack_for_test;
        crack->free_parse_hash = algo10_free_parse_hash;
        strcpy(crack->example_pwd, ALGO10_EXAMPLE_PWD);
        crack->example_hash = (char*)malloc( strlen(ALGO10_EXAMPLE_HASH)+128 );
        strcpy(crack->example_hash, ALGO10_EXAMPLE_HASH);
        break;
    case 20:   // "57ab8499d08c59a7211c77f557bf9425:4247"
                //"md5($salt.$pass)"
        strcpy(crack->algo.algo_description, ALGO20_DESCRIPTION);
        crack->parse_hash = algo20_parse_hash;
        crack->crack_for_test = algo20_crack_for_test;
        crack->free_parse_hash = algo20_free_parse_hash;
        strcpy(crack->example_pwd, ALGO20_EXAMPLE_PWD);
        crack->example_hash = (char*)malloc( strlen(ALGO20_EXAMPLE_HASH)+128 );
        strcpy(crack->example_hash, ALGO20_EXAMPLE_HASH);
        break;
    case 50:   // "e28e4e37e972a945e464b5226053bac0:40"
                //"HMAC-MD5 (key = $pass)"
        strcpy(crack->algo.algo_description, ALGO50_DESCRIPTION);
        crack->parse_hash = algo50_parse_hash;
        crack->crack_for_test = algo50_crack_for_test;
        crack->free_parse_hash = algo50_free_parse_hash;
        strcpy(crack->example_pwd, ALGO50_EXAMPLE_PWD);
        crack->example_hash = (char*)malloc( strlen(ALGO50_EXAMPLE_HASH)+128 );
        strcpy(crack->example_hash, ALGO50_EXAMPLE_HASH);
        break;
    case 111:   // "{SSHA}AZKja92fbuuB9SpRlHqaoXxbTc43Mzc2MDM1Ng=="
                //"nsldaps,SSHA-1(Base64),Netscape LDAP SSHA"
        strcpy(crack->algo.algo_description, ALGO111_DESCRIPTION);
        crack->parse_hash = algo111_parse_hash;
        crack->crack_for_test = algo111_crack_for_test;
        crack->free_parse_hash = algo111_free_parse_hash;
        strcpy(crack->example_pwd, ALGO111_EXAMPLE_PWD);
        crack->example_hash = (char*)malloc( strlen(ALGO111_EXAMPLE_HASH)+128 );
        strcpy(crack->example_hash, ALGO111_EXAMPLE_HASH);
        break;
    case 150:   // "02b256705348a28b1d6c0f063907979f7e0c82f8:10323"
                //"HMAC-SHA1 (key = $pass)"
        strcpy(crack->algo.algo_description, ALGO150_DESCRIPTION);
        crack->parse_hash = algo150_parse_hash;
        crack->crack_for_test = algo150_crack_for_test;
        crack->free_parse_hash = algo150_free_parse_hash;
        strcpy(crack->example_pwd, ALGO150_EXAMPLE_PWD);
        crack->example_hash = (char*)malloc( strlen(ALGO150_EXAMPLE_HASH)+128 );
        strcpy(crack->example_hash, ALGO150_EXAMPLE_HASH);
        break;
    case 200:   // "7196759210defdc0"
                //"mysql323"
        strcpy(crack->algo.algo_description, ALGO200_DESCRIPTION);
        crack->parse_hash = algo200_parse_hash;
        crack->crack_for_test = algo200_crack_for_test;
        crack->free_parse_hash = algo200_free_parse_hash;
        strcpy(crack->example_pwd, ALGO200_EXAMPLE_PWD);
        crack->example_hash = (char*)malloc( strlen(ALGO200_EXAMPLE_HASH)+128 );
        strcpy(crack->example_hash, ALGO200_EXAMPLE_HASH);
        break;
    case 300:   // "fcf7c1b8749cf99d88e5f34271d636178fb5d130"
                //"mysql4.1/mysql5"
        strcpy(crack->algo.algo_description, ALGO300_DESCRIPTION);
        crack->parse_hash = algo300_parse_hash;
        crack->crack_for_test = algo300_crack_for_test;
        crack->free_parse_hash = algo300_free_parse_hash;
        strcpy(crack->example_pwd, ALGO300_EXAMPLE_PWD);
        crack->example_hash = (char*)malloc( strlen(ALGO300_EXAMPLE_HASH)+128 );
        strcpy(crack->example_hash, ALGO300_EXAMPLE_HASH);
        break;
    case 400:   // "$P$984478476IagS59wHZvyQMArzfx58u."
                //"$H$984478476IagS59wHZvyQMArzfx58u."
                //"phpass,WordPress(MD5),Joomla(MD5)/phpass,phpBB3(MD5)"
        strcpy(crack->algo.algo_description, ALGO400_DESCRIPTION);
        crack->parse_hash = algo400_parse_hash;
        crack->crack_for_test = algo400_crack_for_test;
        crack->free_parse_hash = algo400_free_parse_hash;
        strcpy(crack->example_pwd, ALGO400_EXAMPLE_PWD);
        crack->example_hash = (char*)malloc( strlen(ALGO400_EXAMPLE_HASH)+128 );
        strcpy(crack->example_hash, ALGO400_EXAMPLE_HASH);
        break;
    case 500:   // "$1$28772684$iEwNOgGugqO9.bIz5sk8k/"
                //"md5crypt, MD5 (Unix), Cisco-IOS $1$ (MD5)"
        strcpy(crack->algo.algo_description, ALGO500_DESCRIPTION);
        crack->parse_hash = algo500_parse_hash;
        crack->crack_for_test = algo500_crack_for_test;
        crack->free_parse_hash = algo500_free_parse_hash;
        strcpy(crack->example_pwd, ALGO500_EXAMPLE_PWD);
        crack->example_hash = (char*)malloc( strlen(ALGO500_EXAMPLE_HASH)+128 );
        strcpy(crack->example_hash, ALGO500_EXAMPLE_HASH);
        break;
    case 1100:   // "4dd8965d1d476fa0d026722989a6b772:3060147285011"
                //"Domain Cached Credentials (DCC), MS Cache"
        strcpy(crack->algo.algo_description, ALGO1100_DESCRIPTION);
        crack->parse_hash = algo1100_parse_hash;
        crack->crack_for_test = algo1100_crack_for_test;
        crack->free_parse_hash = algo1100_free_parse_hash;
        strcpy(crack->example_pwd, ALGO1100_EXAMPLE_PWD);
        crack->example_hash = (char*)malloc( strlen(ALGO1100_EXAMPLE_HASH)+128 );
        strcpy(crack->example_hash, ALGO1100_EXAMPLE_HASH);
        break;
    case 1400:   // "127e6fbfe24a750e72930c220a8e138275656b8e5d8f48a98c3c92df2caba935"
                //"SHA2-256"
        strcpy(crack->algo.algo_description, ALGO1400_DESCRIPTION);
        crack->parse_hash = algo1400_parse_hash;
        crack->crack_for_test = algo1400_crack_for_test;
        crack->free_parse_hash = algo1400_free_parse_hash;
        strcpy(crack->example_pwd, ALGO1400_EXAMPLE_PWD);
        crack->example_hash = (char*)malloc( strlen(ALGO1400_EXAMPLE_HASH)+128 );
        strcpy(crack->example_hash, ALGO1400_EXAMPLE_HASH);
        break;
    case 1450:   // ""
                //""
        strcpy(crack->algo.algo_description, ALGO1450_DESCRIPTION);
        crack->parse_hash = algo1450_parse_hash;
        crack->crack_for_test = algo1450_crack_for_test;
        crack->free_parse_hash = algo1450_free_parse_hash;
        strcpy(crack->example_pwd, ALGO1450_EXAMPLE_PWD);
        crack->example_hash = (char*)malloc( strlen(ALGO1450_EXAMPLE_HASH)+128 );
        strcpy(crack->example_hash, ALGO1450_EXAMPLE_HASH);
        break;
    case 1711:   // "4dd8965d1d476fa0d026722989a6b772:3060147285011"
                //"Domain Cached Credentials (DCC), MS Cache"
        strcpy(crack->algo.algo_description, ALGO1711_DESCRIPTION);
        crack->parse_hash = algo1711_parse_hash;
        crack->crack_for_test = algo1711_crack_for_test;
        crack->free_parse_hash = algo1711_free_parse_hash;
        strcpy(crack->example_pwd, ALGO1711_EXAMPLE_PWD);
        crack->example_hash = (char*)malloc( strlen(ALGO1711_EXAMPLE_HASH)+128 );
        strcpy(crack->example_hash, ALGO1711_EXAMPLE_HASH);
        break;
    case 12900:   // "15738301074686823451275227041071157383010746868234512752270410712bc4be900bf96ccf43c9852fff49b5f5874a9f6e7bf301686fa6d98286de151f15738301074686823451275227041071"
                //"Android FDE (Samsung DEK)"
        strcpy(crack->algo.algo_description, ALGO12900_DESCRIPTION);
        crack->parse_hash = algo12900_parse_hash;
        crack->crack_for_test = algo12900_crack_for_test;
        crack->free_parse_hash = algo12900_free_parse_hash;
        strcpy(crack->example_pwd, ALGO12900_EXAMPLE_PWD);
        crack->example_hash = (char*)malloc( strlen(ALGO12900_EXAMPLE_HASH)+128 );
        strcpy(crack->example_hash, ALGO12900_EXAMPLE_HASH);
        break;
    default:
        strcpy(crack->algo.algo_description, "");
        crack->parse_hash = NULL;
        crack->crack_for_test = NULL;
        crack->free_parse_hash = NULL;
        strcpy(crack->example_pwd, "");
        crack->example_hash = NULL;
        break;
    }
    return crack;
}

void RunCrack::crack_reinit()
{
    if (m_crack == NULL)
    {
        m_crack = crack_init_internal(m_algoid);
    }
}

void RunCrack::crack_delete()
{
    if (m_crack != NULL)
    {
        // free memory allocator:
        _alloc_destroy(&m_crack->pt_allocator);
        //
        if (m_crack->example_hash != NULL)
        {
            free(m_crack->example_hash);
            m_crack->example_hash = NULL;
        }
        //
        free(m_crack);
        m_crack = NULL;
    }
}

STATE_FLAG RunCrack::parse_hash(const char * hash,     int len_hash)
{
    m_crack->parse_hash(m_crack, hash, len_hash);
}

STATE_FLAG RunCrack::crack_for_test(const char * password, int len_pwd)
{
    m_crack->crack_for_test(m_crack, password, len_pwd);
}

void RunCrack::free_parse_hash()
{
    m_crack->free_parse_hash(m_crack);
}

QString RunCrack::get_example_pwd()
{
    return m_crack->example_pwd;
}

QString RunCrack::get_example_hash()
{
    return m_crack->example_hash;
}

QString RunCrack::get_correct_pwd()
{
    return m_crack->correct_pwd;
}

void RunCrack::crack_init(qint32 algo_id)
{
    // 1. clear old crack:
    crack_delete();
    // 2. init m_crack:
    m_crack = crack_init_internal(algo_id);
}

void RunCrack::slot_crack_force(qint16 minlen, qint16 maxlen)
{
    //
    QTime timedebuge;//声明一个时钟对象
    timedebuge.start();
    //
    bool is_success = false;
    char pwd[256] = {0x00};
    int len_pwd;
    mp_data_t * mp_data = maskprocessor_init(minlen, maxlen);
    u64 cnt = 0;
    double speed = 0.0;
    bool speed_is_K = false;
    double mstime = 0.0;
    for (len_pwd = minlen; len_pwd <= maxlen; len_pwd++)
    {
        for (int i = 0; i < len_pwd; i++)
        {
            mp_data->cs[i].cs_pos = mp_data->cs[i].cs_len;
        }
        mp_data->cs[0].cs_pos = 0;
        int first;
        while ((first = maskprocessor_next(mp_data, pwd, len_pwd)) != -1)
        {
            cnt++;
            //qDebug() << pwd;
            if (cnt % 100000 == 0)
            {
                mstime = timedebuge.elapsed();   // (ms)
                speed = ((100000.0 / mstime)*1000)/(1024*1024);
                if (speed < 1)
                {
                    speed = speed * 1000;
                    speed_is_K = true;
                }
                else
                {
                    speed_is_K = false;
                }
                timedebuge.restart();
            }
            if (cnt % 10000 == 0)
            {
                QString speed_unit = "";
                if (speed_is_K)
                {
                    speed_unit = "K/S";
                }
                else
                {
                    speed_unit = "M/S";
                }
                QString info = QString("current len: %1\ncurrent force-crack speed: %2(%3)\ncurrent force-crack password: %4\nforce-cracking ... ... ... ... ")
                        .arg(len_pwd)
                        .arg((float)speed)
                        .arg(speed_unit)
                        .arg(pwd);
                emit sig_realtime_crack_force_info(info);

            }
            //
            if (crack_for_test(pwd, len_pwd) == SUCCESS_RET)
            {
                is_success = true;
                break;
            }
        }
        if (is_success)
        {
            break;
        }
    }
    maskprocessor_delete(&mp_data);
    if (is_success)
    {
        QString info = QString("success force-cracked!");
        emit sig_realtime_crack_force_info(info);
        strcpy(m_crack->correct_pwd, pwd);
        emit sig_done_crack_force(1);
    }
    else
    {
        QString info = QString("fail force-cracked!");
        emit sig_realtime_crack_force_info(info);
        memset(m_crack->correct_pwd, 0x00, 256);
        emit sig_done_crack_force(0);
    }
}

