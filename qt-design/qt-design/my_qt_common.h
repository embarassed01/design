#ifndef _LX_MY_COMMON_H_
#define _LX_MY_COMMON_H_

#define _WIN         // 影响回车符 "\r\n" 还是 "\n"

#define _FILE_OFFSET_BITS   64

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>
#include <stdarg.h>
#include <assert.h>
#include <float.h>
#include <time.h>
#include <stddef.h>

////// Qt5 header
#include <QString>
#include <QFile>

typedef uint8_t       u8;
typedef uint16_t      u16;
typedef uint32_t      u32;
typedef uint64_t      u64;

typedef int8_t        i8;
typedef int16_t       i16;
typedef int32_t       i32;
typedef int64_t       i64;

//////////////////////////// macro function define
/*
 *  type define of unary func, binary func
 * N.B.:  the last parameter is for output
*/
typedef void (*unary_function_t)(const void*, void*);          // the 1st "void*" for output
typedef void (*binary_function_t)(const void*, const void*, void*);  // the 2nd "void*" for output
typedef unary_function_t  ufun_t;
typedef binary_function_t bfun_t;

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

#define ERR_PLAIN                       -1
#define ERR_INFILE_OPEN                 -2
#define ERR_OUTFILE_CREATE              -3
#define ERR_REG_KEY_OPEN                -4
#define ERR_REG_VAL_QUERY               -5

#define MSG_ERR_ENOMEM      "Insufficient memory available"

/////////////// ............................................... op about log ..............................................
//// 日志相关函数.
QString log(QString message);
QString loginfo(QString message);
QString logwarn(QString message);
QString logerror(QString message);

//void log_info (const char * fmt, ...);
//void log_warn (const char * fmt, ...);
//void log_error (const char * fmt, ...);

void print_buffer (u8 * buffer, u32 bufflen);

/////////////// ............................................. op about crypt common ........................................
void store32_be(u8 dst[4], u32 w);
void store32_le(u8 dst[4], u32 w);

u8 tolower(u8 onebyte);
u8 toupper(u8 onebyte);
void lowercase(u8 * buf, int len);
void uppercase(u8 * buf, int len);

bool is_valid_base64_char(u8 c);
bool is_valid_base64_string(u8 * s, int len);
bool is_valid_base64b_string(u8 * s, int len);

// 'd' --> 0x0d
u8 hex_convert(u8 c);
u8 hex_to_u8(u8 hex[2]);
u32 hex_to_u32(u8 hex[8]);

bool is_valid_hex_char(u8 c);
bool is_valid_hex_string(u8 * s, int len);

u32 my_add(u32 a, u32 b);

// unkown meaning!
u8 itoa64_to_int(const u8 c);

void phpass_decode(u8 digest[16], u8 buf[22]);

int rounds_count_length(const char * input_buf, const int input_len);

void md5crypt_decode(u8 digest[16], u8 buf[22]);

int string_to_unicode(char * in_str, u32 in_len, char * out_str);

/////////////// ............................................. op about mem .................................................
//  多维组的内存管理函数  实际使用不会超过2d mem free
void free_ss (void ** ss, i64 ss_len);

/////// .......................... 0. alloc's struct data ....................................
#define _MEM_ALIGNMENT              8                                          /* boundary for small memory block */
#define _MEM_SMALL_MEM_SIZE_MAX     128                                        /* the maxinum size of small memory   128  */
#define _MEM_LINK_COUNT             _MEM_SMALL_MEM_SIZE_MAX/_MEM_ALIGNMENT     // 16
#define _MEM_CHUNK_COUNT            16                                         /* default chunk count getted from pool   16 */
#define _MEM_POOL_DEFAULT_COUNT     16                                         /* memory pool count 16  */

/* round up the size of memory to the multiple of 8 */
#define _MEM_ROUND_UP(memsize)      (((memsize) + _MEM_ALIGNMENT - 1) & ~(_MEM_ALIGNMENT - 1))
/* get the memory link index with memsize */
#define _MEM_LINK_INDEX(memsize)    (((memsize) + _MEM_ALIGNMENT - 1) / _MEM_ALIGNMENT - 1)

/** data type declaration and struct, union, enum section **/
typedef union _tagmemlink
{
    union _tagmemlink* _pui_nextmem;  /* point to next memory block */
    u8                 _pby_mem[1];    /* represent memory block */
} _memlink_t;

typedef struct _tagalloc
{
    _memlink_t* _apt_memlink[_MEM_LINK_COUNT];   /* memory list */
    u8**        _ppby_mempoolcontainer;          /* memory pool container */
    u8*         _pby_mempool;                    /* memory pool start */
    size_t      _t_mempoolsize;                  /* memory pool size */
    size_t      _t_mempoolindex;                 /* memory pool index */
    size_t      _t_mempoolcount;                 /* memory pool count */
} _alloc_t;

////// ............................ 1. alloc aux ............................................
/** constant declaration and macro section **/
/** data type declaration and struct, union, enum section **/
/** exported global variable declaration section **/
extern void (*_gpfun_malloc_handler)(void);

/** exported function prototype section **/
/**
 * Malloc for out of memory condition.
 * @param t_memset  memory size.
 * @return point to the allocated memory.
 */
void* _alloc_malloc_out_of_memory(size_t t_memsize);

/**
 * Malloc memory.
 * @param t_memsize  memory size.
 * @return point to the allocated memory.
 */
void* _alloc_malloc(size_t t_memsize);

/**
 * Free memory.
 * @param pv_allocmem  memory.
 * @return void.
 * @remarks if pv_allocmem == NULL, then the function of behavior is undefined.
 */
void _alloc_free(void* pv_allocmem);

/**
 * Apply a formated memory list.
 * @param pt_allocator  pointer that point to allocator.
 * @param t_allocsize   the size of allocated memory.
 * @return void.
 * @remarks if pt_allocator == NULL then the function of behavior is undefined.
 */
void _alloc_apply_formated_memory(_alloc_t* pt_allocator, size_t t_allocsize);

/**
 * Get memory chunk from memory pool.
 * @param pt_allocator   pointer pointes to allocator.
 * @param t_allocsize    the size of allocated memory chunk.
 * @param pn_alloccount  the number of allocated memory chunk.
 * @return pointer pointes to allocated chunk.
 * @remarks if pt_allocator == NULL or pn_alloccount == NULL, then the function of behavior is undefined.
 *          if the total size of allocated memory chunk is less than the size of memory pool, then allocated
 *          memory chunk from memory pool directly. if the size of memory pool is less than total size of
 *          allocated memory chunk, but greater than or equal to the size of only one memory chunk, than
 *          allocated appropriate number of memory chunk, then update the allocate count. otherwise allocated
 *          larger memory pool from allocated memory chunk.
 */
u8* _alloc_get_memory_chunk(_alloc_t* pt_allocator, size_t t_allocsize, size_t* pt_alloccount);

////// ............................ 2. alloc     ............................................
/** exported global variable declaration section **/

/** exported function prototype section **/
/**
 * Initialize the _alloc_t.
 * @param pt_allocator  pointer that points to allocator.
 * @return void.
 * @remarks if pt_allocator == NULL, then function of the behavior is undefined.
 */
void _alloc_init(_alloc_t* pt_allocator);

/**
 * Destroy the _alloc_t.
 * @param pt_allocator  pointer that points to allocator.
 * @return void.
 * @remarks if pt_allocator == NULL, then function of the behavior is undefined. if pt_allocator is not initialized by
 *          _alloc_init, then function of the behavior is undefined.
 */
void _alloc_destroy(_alloc_t* pt_allocator);

/**
 * Allocate to user specified amount of memory.
 * @param pt_allocator  pointer that points to allocator.
 * @param t_size        memory size.
 * @param t_count       memory count.
 * @return point to the allocated memory.
 * @remarks if pt_allocator == NULL, then function of the behavior is undefined. if allocator is not initialized by
 *          _alloc_init, then function of the behavior is undefined. the size of allocated memory is t_size * t_count.
 */
void* _alloc_allocate(_alloc_t* pt_allocator, size_t t_size, size_t t_count);

/**
 * Release allocated memory
 * @param pt_allocator  pointer that point to allocator.
 * @param pv_allocmem   pointer that point to allocated memory.
 * @param t_size        allocated memory size.
 * @param t_count       allocated memory count.
 * @return void.
 * @remarks if pt_allocator == NULL or pv_allocmem == NULL, then function of the behavior is undefined. if allocator is
 *          not initialized by _alloc_init, or pv_allocmem is not allocated by _alloc_allocate, then function of the
 *          behavior is undefined.
 */
void _alloc_deallocate(_alloc_t* pt_allocator, void* pv_allocmem, size_t t_size, size_t t_count);

//#ifndef NDEBUG
///**
// * Test alloc_t is initialized.
// * @param cpt_allocator  allocator.
// * @return if allocator is initialized then return true, else return false.
// * @remarks if cpt_allocator == NULL then the behavior is undefined.
// */
//extern bool _alloc_is_inited(const _alloc_t* cpt_allocator);
//#endif /* NDEBUG */

/**
 * Set the out of memory handler and return the old handler.
 * @param pfun_newhandler  new out of memory handler.
 * @return old hanlder.
 * @remarks if pfun_newhandler == NULL, than delete the out of memory handler.
 */
extern void (*_alloc_set_malloc_handler(void (*pfun_newhandler)(void)))(void);

//////////////////// .......................................... op about file ................................................
/*
 *          文件分隔符
 *   windows: "\r\n"
 *   linux: "\n"
 * */
 #if defined (_WIN)
 #define EOL "\r\n"
 #else 
 #define EOL "\n"
 #endif

//// csv分隔符
#define  CSVSEP   ","

void delete_file (QString filename);

/*
 *      delete_files
 * \brief     删除指定目录下的所有文件, 不包括子目录
 * \param path 指定目录. like "lib\\tmp\\*"  必须有*结尾
 * \return
 * */
void delete_files (const char * path);

/*
 *   traversing_one_folder 
 * N.B.: cannot support Multi-Set: Chinese, Japan, Korea Language ... ...
 * \brief     遍历指定目录下的所有文件
 * \param  path 指定目录
 * \param  files_cnt 遍历得到的文件总数
 * \return  filenames[][] 文件名组
 *              IF no file, NULL
 * */
char ** traversing_one_folder (const char * path, int * files_cnt);

void qt_traversing_one_folder (QString inpath, QStringList * p_files_list);

QStringList find_somefiles_one_folder(QString path, QString reg);

/*
 *      direct_empty
 * \brief   
 * \param dn 
 * \return  true   目录为空
 *          false  目录非空
 * */
bool direct_empty (char * dn);

/*
 *    is_exist_folder
 *  \brief    利用win和linux共有的stat相关函数检测指定路径是否是目录
 * \param path 指定输入路径 
 * \return true exist
 *              false not exist
 * */
bool is_exist_folder (QString path);

/*
 *    get_up_folder
 * \brief     get file's shangyiji folder
 * \example   "/home/hadoop/Desktop/index.cpp" --> "/home/hadoop/Desktop"
*/
QString get_up_folder(QString infile);


#define RD_UNIT_BLOCK 0x800000      //8MB
/*
 *    read_block_txt_to_mem
 * \brief     读取大文本文件固定块内容, 以固定块最后一行结束, 因此实际读取的块大小real_block_size <= block_size
 *              IF intxtfile == NULL, 表明大文本文件没有读完, 继续读取该文本文件!!!!
 * \param 全局结构体
 * \param intxtfile 大文本文件名
 * \param block_size 要求读取的固定块大小
 * \param lines_cnt 实际读取的行数
 * \param real_block_size 实际读取的块大小
 *              -1   表明大文本文件已经读取完成, 此时的buffss中没有内容
 *  \return buffss 输出到的内存缓冲区. like buffss[real_lines_in_block][len_linestr+1] 2维组, 每一行是文本文件的一行字符串内容
 *    使用模板:
 *     txtfile全局结构体初始化：
 *  txtfile_gdata_t global_data;
    global_data.fp = NULL;
    global_data.is_eof = false;
    global_data.is_first_read = false;
    global_data.remain_buf = NULL;
    global_data.len_remain_buf = 0;
    QString intxtfile = "./test.txt";
    int real_read_block_size = 0;
    int read_block_size = 0x100000;   // 1MB/block
 *  do {
 *      int n_lines = 0;
        u8 ** buffss = read_block_txt_to_mem(&global_data, intxtfile, read_block_size, &real_read_block_size, &n_lines);
        intxtfile = "";
        if (n_lines > 0)
        {
            // use buffss[n_lines][line_str]
            for (int i = 0; i < n_lines; i++)
            {
             // TO DO.
            }
            free_ss((void**)buffss, n_lines);
        }
    } while (real_read_block_size != -1);

    NOTE.  For (ubuntu)linux : infile must like this "//home/hadoop/Desktop/rockyou.txt", not "/home/hadoop/Desktop/rockyou.txt"!!
 * */
typedef struct
{
    QFile * fp;
    u8 * remain_buf;
    int len_remain_buf;
    bool is_first_read;
    bool is_eof;
} txtfile_gdata_t;
u8 ** read_block_txt_to_mem (txtfile_gdata_t * global_data, QString intxtfile, int  block_size,  int * real_block_size, int * lines_cnt);
 /*
  * 	read_block_bin_to_mem
  * \brief   读取大二进制文件固定块内容
  * \param inbinfile 指定的二进制文件
  * \param block_size 指定的块大小
  * \param real_block_size 实际读取的块大小
  * \return bufs(u8 *)  返回指定的二进制文件内容缓冲
  *
 *     binfile全局结构体初始化：
 *  binfile_gdata_t global_data;
    global_data.fp = NULL;
    global_data.is_eof = false;
  * */
typedef struct
{
    QFile * fp;
    bool is_eof;
} binfile_gdata_t;
u8 * read_block_bin_to_mem (binfile_gdata_t * global_data, QString inbinfile, int block_size, int * real_block_size);

/*
*          get_file_size
*/
u64 get_file_size (QString infile);

/*
*           get_file_name
*/
QString get_file_name(QString file_fullpath);
/////////////// .................................................................................................................

u64 swap2bigend_64(u8 * in);

u32 swap2bigend_32(u8 * in);

u16 swap2bigend_16(u8 * in);

#define MY_TMP_FILE    "kkkoo1234567890ookkktmp"



#endif // _LX_MY_COMMON_H_

