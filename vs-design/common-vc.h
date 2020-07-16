#ifndef _SDK_COMMON_VC_H_
#define _SDK_COMMON_VC_H_

#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <stddef.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

#include <tchar.h>
#include <wchar.h>         // for unicode-set

/*
 *	得到文件大小(不超过2G)
 * @par
 * @ret 
 */
int w_get_file_size(const wchar_t * in_file);

/*
 *	宽字节路径操作文件——读取文件内容
 * @par  
 * @ret 
*/
void w_read_file(const wchar_t * in_file, u8 * out_buf, const int out_buf_size);

/*
 *	宽字节路径操作文件——写入文件内容
 * @par  
 * @ret 
*/
void w_write_file(const wchar_t * out_file, const u8 * in_buf, const int towrite_size);


#endif    //