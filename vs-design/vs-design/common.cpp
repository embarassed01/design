#include "common.h"

////////////// ........................................... op about mem .........................................
////// ............................ 1. alloc aux ............................................
/** exported global variable definition section **/
void(*_gpfun_malloc_handler)(void) = NULL;

/** exported function implementation section **/
/**
 * Malloc for out of memory condition.
 */
void * _alloc_malloc_out_of_memory(size_t t_memsize)
{
	void * pv_allocmem = NULL;     /* point to allocated memory */
	for (;;)
	{
		if (_gpfun_malloc_handler == NULL)
		{
			fprintf(stderr, "FATAL ERROR: memory allocation error!\n");
			exit(EXIT_FAILURE);
			return NULL;
		}
		else
		{
			(*_gpfun_malloc_handler)();
			if ((pv_allocmem = (void*)malloc(t_memsize)) != NULL)
			{
				return pv_allocmem;
			}
		}
	}
}

/**
 * Malloc memory.
 */
void * _alloc_malloc(size_t t_memsize)
{
	void * pv_allocmem = NULL;    /* point to the allocated memory */
	if ((pv_allocmem = (void*)malloc(t_memsize)) == NULL)
	{
		pv_allocmem = _alloc_malloc_out_of_memory(t_memsize);
		assert(pv_allocmem != NULL);
	}
	return pv_allocmem;
}

/**
 * Free memory.
 */
void _alloc_free(void * pv_allocmem)
{
	assert(pv_allocmem != NULL);
	free(pv_allocmem);
}

/**
 * Apply a formated memory list.
 */
void _alloc_apply_formated_memory(_alloc_t * pt_allocator, size_t t_allocsize)
{
	size_t t_alloccount = _MEM_CHUNK_COUNT;
	_memlink_t * pt_link = NULL;
	size_t i = 0;
	assert(pt_allocator != NULL);
	pt_link = (_memlink_t*)_alloc_get_memory_chunk(pt_allocator, t_allocsize, &t_alloccount);
	assert(pt_link != NULL);
	/* format the apply memory block to memory list format */
	pt_allocator->_apt_memlink[_MEM_LINK_INDEX(t_allocsize)] = pt_link;
	for (i = 0; i < t_alloccount; ++i)
	{
		if (i == t_alloccount - 1)
		{
			pt_link->_pui_nextmem = NULL;
		}
		else
		{
			pt_link->_pui_nextmem = (_memlink_t*)((u8*)pt_link + t_allocsize);
		}
		pt_link = pt_link->_pui_nextmem;
	}
}

/**
 * Get memory chunk from memory pool.
 */

u8 * _alloc_get_memory_chunk(_alloc_t* pt_allocator, size_t t_allocsize, size_t* pt_alloccount)
{
	size_t t_totalsize = 0;     /* total size of alloc */
	u8 * pby_allocmem = NULL;   /* the allocated memory block */
	size_t t_getmemsize = 0;    /* bytes to get memory from system heap */
	size_t i = 0;
	assert(pt_allocator != NULL);
	assert(pt_alloccount != NULL);
	t_totalsize = t_allocsize * (*pt_alloccount);
	if (pt_allocator->_t_mempoolsize >= t_totalsize)
	{
		pby_allocmem = pt_allocator->_pby_mempool;
		pt_allocator->_pby_mempool = pby_allocmem + t_totalsize;
		pt_allocator->_t_mempoolsize -= t_totalsize;
		return pby_allocmem;
	}
	else if (pt_allocator->_t_mempoolsize >= t_allocsize)
	{
		*pt_alloccount = pt_allocator->_t_mempoolsize / t_allocsize;
		t_totalsize = t_allocsize * (*pt_alloccount);
		pby_allocmem = pt_allocator->_pby_mempool;
		pt_allocator->_pby_mempool = pby_allocmem + t_totalsize;
		pt_allocator->_t_mempoolsize -= t_totalsize;
		return pby_allocmem;
	}
	else
	{
		/* if the memory pool container is full */
		assert(pt_allocator->_t_mempoolindex <= pt_allocator->_t_mempoolcount);
		/* take the small memory block to the memory list */
		if (pt_allocator->_t_mempoolsize > 0)
		{
			((_memlink_t*)pt_allocator->_pby_mempool)->_pui_nextmem = pt_allocator->_apt_memlink[_MEM_LINK_INDEX(pt_allocator->_t_mempoolsize)];
			pt_allocator->_apt_memlink[_MEM_LINK_INDEX(pt_allocator->_t_mempoolsize)] = (_memlink_t*)pt_allocator->_pby_mempool;
			pt_allocator->_t_mempoolsize = 0;
		}
		t_getmemsize = 2 * t_totalsize;
		pt_allocator->_pby_mempool = (u8*)malloc(t_getmemsize);
		if (pt_allocator->_pby_mempool == NULL)
		{
			/* search the memory list for unuse memory that meet the size */
			for (i = t_allocsize; i < _MEM_SMALL_MEM_SIZE_MAX; i += _MEM_ALIGNMENT)
			{
				pt_allocator->_pby_mempool = (u8*)pt_allocator->_apt_memlink[_MEM_LINK_INDEX((i))];
				if (pt_allocator->_pby_mempool != NULL)
				{
					pt_allocator->_apt_memlink[_MEM_LINK_INDEX(i)] = ((_memlink_t*)pt_allocator->_pby_mempool)->_pui_nextmem;
					pt_allocator->_t_mempoolsize = i;
					return _alloc_get_memory_chunk(pt_allocator, t_allocsize, pt_alloccount);
				}
			}
			pt_allocator->_pby_mempool = (u8*)_alloc_malloc_out_of_memory(t_getmemsize);
		}
		/* if the memory pool container is full */
		if (pt_allocator->_t_mempoolindex == pt_allocator->_t_mempoolcount)
		{
			u8 ** ppby_oldmempool = pt_allocator->_ppby_mempoolcontainer;
			pt_allocator->_t_mempoolcount += _MEM_POOL_DEFAULT_COUNT;
			pt_allocator->_ppby_mempoolcontainer = (u8**)malloc(pt_allocator->_t_mempoolcount * sizeof(u8*));
			if (pt_allocator->_ppby_mempoolcontainer != NULL)
			{
				for (i = 0; i < pt_allocator->_t_mempoolcount; ++i)
				{
					pt_allocator->_ppby_mempoolcontainer[i] = NULL;
				}
				memcpy(pt_allocator->_ppby_mempoolcontainer, ppby_oldmempool, (pt_allocator->_t_mempoolcount - _MEM_POOL_DEFAULT_COUNT) * sizeof(u8*));
				free(ppby_oldmempool);
				assert(pt_allocator->_t_mempoolindex < pt_allocator->_t_mempoolcount);
			}
			else
			{
				fprintf(stderr, "FATAL ERROR: memory allocation error!\n");
				exit(EXIT_FAILURE);
			}
		}
		pt_allocator->_ppby_mempoolcontainer[pt_allocator->_t_mempoolindex++] = pt_allocator->_pby_mempool;
		/* apply memory from system heap success or call _alloc_malloc_out_of_memory success */
		pt_allocator->_t_mempoolsize = t_getmemsize;
		pby_allocmem = pt_allocator->_pby_mempool;
		pt_allocator->_pby_mempool = pby_allocmem + t_totalsize;
		pt_allocator->_t_mempoolsize -= t_totalsize;
		return pby_allocmem;
	}
}

////// ............................ 2. alloc     ............................................
/**
 * Initialize the _alloc_t.
 **/
void _alloc_init(_alloc_t * pt_allocator)
{
	size_t i = 0;
	assert(pt_allocator != NULL);
	pt_allocator->_t_mempoolsize = 0;
	pt_allocator->_t_mempoolindex = 0;
	pt_allocator->_pby_mempool = NULL;
	for (i = 0; i < _MEM_LINK_COUNT; i++)
	{
		pt_allocator->_apt_memlink[i] = NULL;
	}
	/* initialize memory pool */
	pt_allocator->_t_mempoolcount = _MEM_POOL_DEFAULT_COUNT;
	pt_allocator->_ppby_mempoolcontainer = (u8**)malloc(pt_allocator->_t_mempoolcount * sizeof(u8*));
	if (pt_allocator->_ppby_mempoolcontainer != NULL)
	{
		for (i = 0; i < pt_allocator->_t_mempoolcount; i++)
		{
			pt_allocator->_ppby_mempoolcontainer[i] = NULL;
		}
	}
	else
	{
		fprintf(stderr, "FATAL ERROR: memory allocation error!\n");
		exit(EXIT_FAILURE);
	}
}

/**
 * Destroy the _alloc_t.
 */
void _alloc_destroy(_alloc_t * pt_allocator)
{
	size_t i = 0;
	assert(pt_allocator != NULL);
	/* destroy memory pool */
	for (i = 0; i < pt_allocator->_t_mempoolcount; ++i)
	{
		free(pt_allocator->_ppby_mempoolcontainer[i]);
		pt_allocator->_ppby_mempoolcontainer[i] = NULL;
	}
	free(pt_allocator->_ppby_mempoolcontainer);
	pt_allocator->_ppby_mempoolcontainer = NULL;
	for (i = 0; i < _MEM_LINK_COUNT; ++i)
	{
		pt_allocator->_apt_memlink[i] = NULL;
	}
	pt_allocator->_pby_mempool = NULL;
	pt_allocator->_t_mempoolindex = 0;
	pt_allocator->_t_mempoolsize = 0;
	pt_allocator->_t_mempoolcount = 0;
}

/**
 * Allocate to user specified amount of memory.
 */
void * _alloc_allocate(_alloc_t * pt_allocator, size_t t_size, size_t t_count)
{
	void * pv_allocmem = NULL;        /* allocated memory pointer */
	_memlink_t * pt_link = NULL;      /* memory link */
	size_t t_allocsize = t_size * t_count;    /* allocated memory size */
	if (t_allocsize == 0)
	{
		return NULL;
	}
	assert(pt_allocator != NULL);
	if (t_allocsize > _MEM_SMALL_MEM_SIZE_MAX)   // mem>128
	{
		pv_allocmem = _alloc_malloc(t_allocsize);
		assert(pv_allocmem != NULL);
	}
	else
	{
		pt_link = pt_allocator->_apt_memlink[_MEM_LINK_INDEX(t_allocsize)];
		if (pt_link == NULL)
		{
			_alloc_apply_formated_memory(pt_allocator, _MEM_ROUND_UP(t_allocsize));
			pt_link = pt_allocator->_apt_memlink[_MEM_LINK_INDEX(t_allocsize)];
			assert(pt_link != NULL);
		}
		pt_allocator->_apt_memlink[_MEM_LINK_INDEX(t_allocsize)] = pt_link->_pui_nextmem;
		pv_allocmem = (void*)pt_link;
	}

	return pv_allocmem;
}

/**
 * Release allocated memory
 */
void _alloc_deallocate(_alloc_t * pt_allocator, void * pv_allocmem, size_t t_size, size_t t_count)
{
	size_t t_allocsize = t_size * t_count;    /* allocated memory size */
	if (t_allocsize == 0)
	{
		return;
	}
	assert(pt_allocator != NULL);
	assert(pv_allocmem != NULL);
	if (t_allocsize > _MEM_SMALL_MEM_SIZE_MAX)
	{
		_alloc_free(pv_allocmem);
	}
	else
	{
		((_memlink_t*)pv_allocmem)->_pui_nextmem = pt_allocator->_apt_memlink[_MEM_LINK_INDEX((t_allocsize))];
		pt_allocator->_apt_memlink[_MEM_LINK_INDEX((t_allocsize))] = ((_memlink_t*)pv_allocmem);
	}
}

/**
 * Set the out of memory handler and return the old handler.
 */
void(*_alloc_set_malloc_handler(void(*pfun_newhandler)(void)))(void)
{
	void(*pfun_oldhandler)(void) = _gpfun_malloc_handler;
	_gpfun_malloc_handler = pfun_newhandler;
	return pfun_oldhandler;
}