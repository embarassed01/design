#include "types.h"

////
stl_type_register_t _gt_type_register = { false, {NULL}, {{NULL}, NULL, NULL, 0, 0, 0} };
//                                      {false, {NULL}, {0}}
// the pt_type, pt_node and t_pos must defined before use these macro
#define TYPE_REGISTER_BEGIN()\
    stl_type_node_t * pt_node = NULL;\
    stl_type_t * pt_type = NULL;\
    size_t t_pos = 0
#define TYPE_REGISTER_TYPE(type, type_text, type_suffix, type_style) \
    do{\
        pt_type = (stl_type_t*)_alloc_allocate(&_gt_type_register.allocator, sizeof(stl_type_t), 1);\
        assert(pt_type != NULL);\
        pt_type->type_size = sizeof(type);\
        memset(pt_type->type_name, 0x00, STL_TYPE_NAME_SIZE+1);\
        strncpy(pt_type->type_name, type_text, STL_TYPE_NAME_SIZE);\
        assert(type_style != STL_TYPE_INVALID);\
        pt_type->_style = type_style;\
        pt_type->type_f_init = _type_init_##type_suffix;\
        pt_type->type_f_copy = _type_copy_##type_suffix;\
        pt_type->type_f_less = _type_less_##type_suffix;\
        pt_type->type_f_destroy = _type_destroy_##type_suffix;\
    }while(false)
#define TYPE_REGISTER_TYPE_NODE(type, type_text) \
    do{\
        pt_node = (stl_type_node_t*)_alloc_allocate(&_gt_type_register.allocator, sizeof(stl_type_node_t), 1);\
        assert(pt_node != NULL);\
        memset(pt_node->type_name, 0x00, STL_TYPE_NAME_SIZE+1);\
        strncpy(pt_node->type_name, type_text, STL_TYPE_NAME_SIZE);\
        t_pos = _type_hash(type_text);\
        pt_node->pt_next = _gt_type_register.apt_bucket[t_pos];\
        _gt_type_register.apt_bucket[t_pos] = pt_node;\
        pt_node->pt_type = pt_type;\
    }while(false)
#define TYPE_REGISTER_END()
//
static void _type_register_c_builtin(void);
static void _type_register_stl_builtin(void);
//
void _type_init()
{
	size_t i = 0;
	// set register hash table:
	for (i = 0; i < STL_TYPE_REGISTER_BUCKET_COUNT; ++i)
	{
		_gt_type_register.apt_bucket[i] = NULL;
	}
	// init allocator
	_alloc_init(&_gt_type_register.allocator);
	//
	_type_register_c_builtin();
	_type_register_stl_builtin();
	_gt_type_register.is_init = true;
}


static void _type_init_default(const void * cpv_input, void * pv_output);
static void _type_copy_default(const void * cpv_first, const void * cpv_second, void * pv_output);
static void _type_less_default(const void * cpv_first, const void * cpv_second, void * pv_output);
static void _type_destroy_default(const void * cpv_input, void * pv_output);
static void _type_get_type(stl_type_info_t * pt_typeinfo, const char * type_name);
static void _type_get_type_pair(stl_type_info_t * pt_typeinfo_first, stl_type_info_t * pt_typeinfo_second, const char * type_name);
static bool _type_is_same(const char * type_name1, const char * type_name2);
static bool _type_is_same_ex(const stl_type_info_t * pt_first, const stl_type_info_t * pt_second);
static void _type_get_varg_value(stl_type_info_t * pt_typeinfo, va_list val_elemlist, void * pv_output);
static void _type_get_elem_typename(const char * type_name, char * elem_typename);
//
bool _type_register(size_t type_size, const char * type_name,
	ufun_t type_init, bfun_t type_copy,
	bfun_t type_less, ufun_t type_destroy)
{
	char formal_name[STL_TYPE_NAME_SIZE + 1];
	stl_type_style_t style = STL_TYPE_INVALID;
	//    if
	return false;
}

void _type_register_c_builtin(void)
{
	TYPE_REGISTER_BEGIN();
	//
	// register: char/signed char  type   text         suffix   style
	TYPE_REGISTER_TYPE(char, STL_CHAR_TYPE, char, STL_TYPE_C_BUILTIN);
	TYPE_REGISTER_TYPE_NODE(char, STL_CHAR_TYPE);
	TYPE_REGISTER_TYPE_NODE(signed char, STL_SIGNED_CHAR_TYPE);
	// register: unsigned char
	TYPE_REGISTER_TYPE(unsigned char, STL_UNSIGNED_CHAR_TYPE, uchar, STL_TYPE_C_BUILTIN);
	TYPE_REGISTER_TYPE_NODE(unsigned char, STL_UNSIGNED_CHAR_TYPE);
	// register: short/short int/ signed short/ signed short int
	TYPE_REGISTER_TYPE(short, STL_SHORT_TYPE, short, STL_TYPE_C_BUILTIN);
	TYPE_REGISTER_TYPE_NODE(short, STL_SHORT_TYPE);
	TYPE_REGISTER_TYPE_NODE(short int, STL_SHORT_INT_TYPE);
	TYPE_REGISTER_TYPE_NODE(signed short, STL_SIGNED_SHORT_TYPE);
	TYPE_REGISTER_TYPE_NODE(signed short int, STL_SIGNED_SHORT_INT_TYPE);
	// register: unsigned short/ unsigned short int
	TYPE_REGISTER_TYPE(unsigned short, STL_UNSIGNED_SHORT_TYPE, ushort, STL_TYPE_C_BUILTIN);
	TYPE_REGISTER_TYPE_NODE(unsigned short, STL_UNSIGNED_SHORT_TYPE);
	TYPE_REGISTER_TYPE_NODE(unsigned short int, STL_UNSIGNED_SHORT_INT_TYPE);
	// register: int/ signed / signed int
	TYPE_REGISTER_TYPE(int, STL_INT_TYPE, int, STL_TYPE_C_BUILTIN);
	TYPE_REGISTER_TYPE_NODE(int, STL_INT_TYPE);
	TYPE_REGISTER_TYPE_NODE(signed, STL_SIGNED_TYPE);
	TYPE_REGISTER_TYPE_NODE(signed int, STL_SIGNED_INT_TYPE);
	// register: unsigned int/ unsigned
	TYPE_REGISTER_TYPE(unsigned int, STL_UNSIGNED_INT_TYPE, uint, STL_TYPE_C_BUILTIN);
	TYPE_REGISTER_TYPE_NODE(unsigned int, STL_UNSIGNED_INT_TYPE);
	TYPE_REGISTER_TYPE_NODE(unsigned, STL_UNSIGNED_TYPE);
	// register: long/ long int/ signed long/ signed long int
	TYPE_REGISTER_TYPE(long, STL_LONG_TYPE, long, STL_TYPE_C_BUILTIN);
	TYPE_REGISTER_TYPE_NODE(long, STL_LONG_TYPE);
	TYPE_REGISTER_TYPE_NODE(long int, STL_LONG_INT_TYPE);
	TYPE_REGISTER_TYPE_NODE(signed long, STL_SIGNED_LONG_TYPE);
	TYPE_REGISTER_TYPE_NODE(signed long int, STL_SIGNED_LONG_INT_TYPE);
	// register: unsigned long / unsigned long int
	TYPE_REGISTER_TYPE(unsigned long, STL_UNSIGNED_LONG_TYPE, ulong, STL_TYPE_C_BUILTIN);
	TYPE_REGISTER_TYPE_NODE(unsigned long, STL_UNSIGNED_LONG_TYPE);
	TYPE_REGISTER_TYPE_NODE(unsigned long int, STL_UNSIGNED_LONG_INT_TYPE);
	// register: float
	TYPE_REGISTER_TYPE(float, STL_FLOAT_TYPE, float, STL_TYPE_C_BUILTIN);
	TYPE_REGISTER_TYPE_NODE(float, STL_FLOAT_TYPE);
	// register: double
	TYPE_REGISTER_TYPE(double, STL_DOUBLE_TYPE, double, STL_TYPE_C_BUILTIN);
	TYPE_REGISTER_TYPE_NODE(double, STL_DOUBLE_TYPE);
	// register: long double
	TYPE_REGISTER_TYPE(long double, STL_LONG_DOUBLE_TYPE, long_double, STL_TYPE_C_BUILTIN);
	TYPE_REGISTER_TYPE_NODE(long double, STL_LONG_DOUBLE_TYPE);
	// register: bool
	TYPE_REGISTER_TYPE(bool, STL_BOOL_TYPE, bool, STL_TYPE_C_BUILTIN);
	TYPE_REGISTER_TYPE_NODE(bool, STL_BOOL_TYPE);
	// register: char*
	TYPE_REGISTER_TYPE(char*, STL_C_STRING_TYPE, cstr, STL_TYPE_C_BUILTIN);
	TYPE_REGISTER_TYPE_NODE(char*, STL_C_STRING_TYPE);
	// register: void*
	TYPE_REGISTER_TYPE(void*, STL_POINTER_TYPE, pointer, STL_TYPE_C_BUILTIN);
	TYPE_REGISTER_TYPE_NODE(void*, STL_POINTER_TYPE);
	// register: long long/ long long int/ signed long long/ signed long long int
	TYPE_REGISTER_TYPE(long long, STL_LONG_LONG_TYPE, long_long, STL_TYPE_C_BUILTIN);
	TYPE_REGISTER_TYPE_NODE(long long, STL_LONG_LONG_TYPE);
	TYPE_REGISTER_TYPE_NODE(long long int, STL_LONG_LONG_INT_TYPE);
	TYPE_REGISTER_TYPE_NODE(signed long long, STL_SIGNED_LONG_LONG_TYPE);
	TYPE_REGISTER_TYPE_NODE(signed long long int, STL_SIGNED_LONG_LONG_INT_TYPE);
	// register: unsigned long long/ unsigned long long int
	TYPE_REGISTER_TYPE(unsigned long long, STL_UNSIGNED_LONG_LONG_TYPE, ulong_long, STL_TYPE_C_BUILTIN);
	TYPE_REGISTER_TYPE_NODE(unsigned long long, STL_UNSIGNED_LONG_LONG_TYPE);
	TYPE_REGISTER_TYPE_NODE(unsigned long long int, STL_UNSIGNED_LONG_LONG_INT_TYPE);
	//
	TYPE_REGISTER_END();
}

void _type_register_stl_builtin(void)
{
	TYPE_REGISTER_BEGIN();
	//
	// register: basic_string_t
	;
	// register: iterator_t/ basic_string_iterator_t
	;
	;
	//
	TYPE_REGISTER_END();
}

// calculate hash from type name:
size_t _type_hash(const char * type_name)
{
	// use BKDR hash func:
	size_t hash = 0;
	assert(type_name != NULL);
	while (*type_name)
	{
		hash = hash * STL_TYPE_HASH_BKDR_SEED + (*type_name++);
	}
	return hash % STL_TYPE_REGISTER_BUCKET_COUNT;
}

//////////////////////////////////////// op of types builtin ////////////////////////
// char / signed char:
void _type_init_char(const void * cpv_input, void * pv_output)
{
	;
}
void _type_copy_char(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_less_char(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_destroy_char(const void * cpv_input, void * pv_output)
{
	;
}
// unsigned char:
void _type_init_uchar(const void * cpv_input, void * pv_output)
{
	;
}
void _type_copy_uchar(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_less_uchar(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_destroy_uchar(const void * cpv_input, void * pv_output)
{
	;
}
// short/short int/ signed short/ signed short int
void _type_init_short(const void * cpv_input, void * pv_output)
{
	;
}
void _type_copy_short(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_less_short(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_destroy_short(const void * cpv_input, void * pv_output)
{
	;
}
// unsigned short / unsigned short int
void _type_init_ushort(const void * cpv_input, void * pv_output)
{
	;
}
void _type_copy_ushort(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_less_ushort(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_destroy_ushort(const void * cpv_input, void * pv_output)
{
	;
}
// int/ signed / signed int
void _type_init_int(const void * cpv_input, void * pv_output)
{
	;
}
void _type_copy_int(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_less_int(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_destroy_int(const void * cpv_input, void * pv_output)
{
	;
}
// unsigned int/ unsigned
void _type_init_uint(const void * cpv_input, void * pv_output)
{
	;
}
void _type_copy_uint(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_less_uint(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_destroy_uint(const void * cpv_input, void * pv_output)
{
	;
}
// long/ long int/ signed long/ signed long int
void _type_init_long(const void * cpv_input, void * pv_output)
{
	;
}
void _type_copy_long(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_less_long(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_destroy_long(const void * cpv_input, void * pv_output)
{
	;
}
// unsigned long / unsigned long int
void _type_init_ulong(const void * cpv_input, void * pv_output)
{
	;
}
void _type_copy_ulong(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_less_ulong(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_destroy_ulong(const void * cpv_input, void * pv_output)
{
	;
}
// float
void _type_init_float(const void * cpv_input, void * pv_output)
{
	;
}
void _type_copy_float(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_less_float(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_destroy_float(const void * cpv_first, void * pv_output)
{
	;
}
// double
void _type_init_double(const void * cpv_input, void * pv_output)
{
	;
}
void _type_copy_double(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_less_double(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_destroy_double(const void * cpv_input, void * pv_output)
{
	;
}
// long double
void _type_init_long_double(const void * cpv_input, void * pv_output)
{
	;
}
void _type_copy_long_double(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_less_long_double(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_destroy_long_double(const void * cpv_input, void * pv_output)
{
	;
}
// bool
void _type_init_bool(const void * cpv_input, void * pv_output)
{
	;
}
void _type_copy_bool(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_less_bool(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_destroy_bool(const void * cpv_input, void * pv_output)
{
	;
}
// char*
void _type_init_cstr(const void * cpv_input, void * pv_output)
{
	;
}
void _type_copy_cstr(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_less_cstr(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_destroy_cstr(const void * cpv_input, void * pv_output)
{
	;
}
// void*
void _type_init_pointer(const void * cpv_input, void * pv_output)
{
	;
}
void _type_copy_pointer(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_less_pointer(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_destroy_pointer(const void * cpv_input, void * pv_output)
{
	;
}
// long long/ long long int/ signed long long/ signed long long int
void _type_init_long_long(const void * cpv_input, void * pv_output)
{
	;
}
void _type_copy_long_long(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_less_long_long(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_destroy_long_long(const void * cpv_input, void * pv_output)
{
	;
}
// unsigned long long/ unsigned long long int
void _type_init_ulong_long(const void * cpv_input, void * pv_output)
{
	;
}
void _type_copy_ulong_long(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_less_ulong_long(const void * cpv_first, const void * cpv_second, void * pv_output)
{
	;
}
void _type_destroy_ulong_long(const void * cpv_input, void * pv_output)
{
	;
}

