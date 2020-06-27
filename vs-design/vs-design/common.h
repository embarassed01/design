#ifndef _VL_COMMON_H_

#define _VL_COMMON_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <stddef.h>

typedef  uint8_t   u8;
typedef  uint16_t  u16;
typedef  uint32_t  u32;
typedef  uint64_t  u64;
typedef  int8_t    i8;
typedef  int16_t   i16;
typedef  int32_t   i32;
typedef  int64_t   i64;

/*
 *  type define of unary func, binary func
 * N.B.:  the last parameter is for output
*/
typedef void(*unary_function_t)(const void*, void*);          // the 1st "void*" for output
typedef void(*binary_function_t)(const void*, const void*, void*);  // the 2nd "void*" for output
typedef unary_function_t  ufun_t;
typedef binary_function_t bfun_t;

/////////////// ............................................. op about mem .................................................
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
extern void(*_gpfun_malloc_handler)(void);

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
extern void(*_alloc_set_malloc_handler(void(*pfun_newhandler)(void)))(void);


#endif  // _VL_COMMON_H_
