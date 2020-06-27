#ifndef _VL_STL_TYPES_H_
#define _VL_STL_TYPES_H_

#include "../common.h"

/////////////////////// C built-in type ///////////////////////////
#define  STL_CHAR_TYPE                      "char"
#define  STL_SIGNED_CHAR_TYPE               "signed char"
#define  STL_UNSIGNED_CHAR_TYPE             "unsigned char"
#define  STL_SIGNED_SHORT_INT_TYPE          "signed short int"
#define  STL_SIGNED_SHORT_TYPE              "signed short"
#define  STL_SHORT_INT_TYPE                 "short int"
#define  STL_SHORT_TYPE                     "short"
#define  STL_UNSIGNED_SHORT_INT_TYPE        "unsigned short int"
#define  STL_UNSIGNED_SHORT_TYPE            "unsigned short"
#define  STL_INT_TYPE                       "int"
#define  STL_SIGNED_TYPE                    "signed"
#define  STL_SIGNED_INT_TYPE                "signed int"
#define  STL_UNSIGNED_INT_TYPE              "unsigned int"
#define  STL_UNSIGNED_TYPE                  "unsigned"
#define  STL_LONG_TYPE                      "long"
#define  STL_LONG_INT_TYPE                  "long int"
#define  STL_SIGNED_LONG_TYPE               "signed long"
#define  STL_SIGNED_LONG_INT_TYPE           "signed long int"
#define  STL_UNSIGNED_LONG_TYPE             "unsigned long"
#define  STL_UNSIGNED_LONG_INT_TYPE         "unsigned long int"
#define  STL_FLOAT_TYPE                     "float"
#define  STL_DOUBLE_TYPE                    "double"
#define  STL_LONG_DOUBLE_TYPE               "long double"
#define  STL_C_STRING_TYPE                  "char*"
#define  STL_POINTER_TYPE                   "void*"
#define  STL_BOOL_TYPE                      "bool"
#define  STL_LONG_LONG_TYPE                 "long long"
#define  STL_LONG_LONG_INT_TYPE             "long long int"
#define  STL_SIGNED_LONG_LONG_TYPE          "signed long long"
#define  STL_SIGNED_LONG_LONG_INT_TYPE      "signed long long int"
#define  STL_UNSIGNED_LONG_LONG_TYPE        "unsigned long long"
#define  STL_UNSIGNED_LONG_LONG_INT_TYPE    "unsigned long long int"
//////////////////////// STL type ///////////////////////////////
#define  STL_VECTOR_TYPE                    "vector_t"
#define  STL_LIST_TYPE                      "list_t"
#define  STL_SLIST_TYPE                     "slist_t"
#define  STL_DEQUE_TYPE                     "deque_t"
#define  STL_STACK_TYPE                     "stack_t"
#define  STL_QUEUE_TYPE                     "queue_t"
#define  STL_PRIORITY_QUEUE_TYPE            "priority_queue_t"
#define  STL_SET_TYPE                       "set_t"
#define  STL_MAP_TYPE                       "map_t"
#define  STL_MULTISET_TYPE                  "multiset_t"
#define  STL_MULTIMAP_TYPE                  "multimap_t"
#define  STL_HASH_SET_TYPE                  "hash_set_t"
#define  STL_HASH_MAP_TYPE                  "hash_map_t"
#define  STL_HASH_MULTISET_TYPE             "hash_multiset_t"
#define  STL_HASH_MULTIMAP_TYPE             "hash_multimap_t"
#define  STL_STRING_TYPE                    "string_t"
#define  STL_PAIR_TYPE                      "pair_t"
#define  STL_RANGE_TYPE                     "range_t"
#define  STL_BASIC_STRING_TYPE              "basic_string_t"
///////////////////// STL iterator type /////////////////////////////
#define  STL_ITERATOR_TYPE                  "iterator_t"
#define  STL_VECTOR_ITERATOR_TYPE           "vector_iterator_t"
#define  STL_LIST_ITERATOR_TYPE             "list_iterator_t"
#define  STL_SLIST_ITERATOR_TYPE            "slist_iterator_t"
#define  STL_DEQUE_ITERATOR_TYPE            "deque_iterator_t"
#define  STL_SET_ITERATOR_TYPE              "set_iterator_t"
#define  STL_MAP_ITERATOR_TYPE              "map_iterator_t"
#define  STL_MULTISET_ITERATOR_TYPE         "multiset_iterator_t"
#define  STL_MULTIMAP_ITERATOR_TYPE         "multimap_iterator_t"
#define  STL_HASH_SET_ITERATOR_TYPE         "hash_set_iterator_t"
#define  STL_HASH_MAP_ITERATOR_TYPE         "hash_map_iterator_t"
#define  STL_HASH_MULTISET_ITERATOR_TYPE    "hash_multiset_iterator_t"
#define  STL_HASH_MULTIMAP_ITERATOR_TYPE    "hash_multimap_iterator_t"
#define  STL_STRING_ITERATOR_TYPE           "string_iterator_t"
#define  STL_INPUT_ITERATOR_TYPE            "input_iterator_t"
#define  STL_OUTPUT_ITERATOR_TYPE           "output_iterator_t"
#define  STL_FORWARD_ITERATOR_TYPE          "forward_iterator_t"
#define  STL_BIDIRECTIONAL_ITERATOR_TYPE    "bidirectional_iterator_t"
#define  STL_RANDOM_ACCESS_ITERATOR_TYPE    "random_access_iterator_t"
#define  STL_BASIC_STRING_ITERATOR_TYPE     "basic_string_iterator_t"
////////////////////// structure type //////////////////////////////
#define  STL_STRUCT_TYPE                    "struct"
#define  STL_UNION_TYPE                     "union"
#define  STL_ENUM_TYPE                      "enum"
///////////////////// STL container type //////////////////////////////
#define  STL_LEFT_BRACKET                   "<"
#define  STL_RIGHT_BRACKET                  ">"
#define  STL_COMMA                          ","
#define  STL_POINTER                        "*"

#define  STL_TYPE_NAME_SIZE     255

///////////////////// data of types /////////////////////////////////
// type style
typedef  enum
{
	STL_TYPE_INVALID = 0,
	STL_TYPE_C_BUILTIN,
	STL_TYPE_USER_DEFINE,
	STL_TYPE_STL_BUILTIN
} stl_type_style_t;
// type tag
typedef struct
{
	size_t              type_size;
	char                type_name[STL_TYPE_NAME_SIZE + 1];
	stl_type_style_t    _style;
	bfun_t              type_f_copy;
	bfun_t              type_f_less;
	ufun_t              type_f_init;
	ufun_t              type_f_destroy;
} stl_type_t;
// type register node
typedef struct  _tag_type_node
{
	char                    type_name[STL_TYPE_NAME_SIZE + 1];
	struct _tag_type_node * pt_next;        // next node
	stl_type_t  *           pt_type;        // the registered type
} stl_type_node_t;
// type register table
#define  STL_TYPE_REGISTER_BUCKET_COUNT  997        // register hash table bucket count
typedef struct
{
	bool                is_init;    // is intial for built in types
	stl_type_node_t *   apt_bucket[STL_TYPE_REGISTER_BUCKET_COUNT];  // hash table
	_alloc_t            allocator;
} stl_type_register_t;
// type info
typedef struct
{
	char                type_name[STL_TYPE_NAME_SIZE + 1];
	stl_type_t *        pt_type;
	stl_type_style_t    _style;
} stl_type_info_t;

///////////////////////////////// global data of types ///////////////////
extern stl_type_register_t _gt_type_register;
/*
 * _gt_type_register
 * +--------------------------------------------------------------------+
 * |     |    |    |    |    |  ...  |    |    |    |    |    |    |    |
 * +-------+--------------------------------+----+----------------------+
 *         |                                |    |
 *         V                                V    V
 *      +-------------+            +--------+    +----------+
 *      | type_node_t |            |        |    |          |
 *      +--+----------+            +--------+    +----------+
 *         |                                |    |
 *         V                                V    V
 *      +-------------+                  NULL    +----------+
 *      | "abc_t"     | major name       +-------|"my_abc_t"| duplicated name
 *      +--+----------+                  |       +----------+
 *         |                             |       |
 *         V                             |       V
 *      +-------------+                  |       NULL
 *      |             |----------+-------+
 *      +--+----------+          |
 *         |                     |
 *         V                     |
 *         NULL                  V
 *                             +------------------------------+
 *                             | _t_typesize = ???            |
 *                             | _s_typename = "abc_t"        |
 *                             | _t_typecopy = abc_copy       | "registered type abc_t"
 *                             | _t_typeless = abc_less       |
 *                             | _t_typeinit = abc_init       |
 *                             | _t_typedestroy = abc_destroy |
 *                             +------------------------------+
 */

 ////////////////////////////////// op of types //////////////////////////
 // initial:
 //      the register + register c builtin type + stl builtin type
void _type_init(void);

// BKDR hash seed
#define STL_TYPE_HASH_BKDR_SEED   131

/**
 *  calculate hash from type name
 * @param type_name
 * @return hash
 * @remarks type_name must not NULL
*/
size_t _type_hash(const char * type_name);

#define type_register(type, type_init, type_copy, type_less, type_destroy) \
    _type_register(sizeof(type), #type, (type_init), (type_copy), (type_less), (type_destroy))
bool _type_register(size_t type_size, const char * type_name,
	ufun_t type_init, bfun_t type_copy,
	bfun_t type_less, ufun_t type_destroy);

#define type_duplicate(type1, type2) \
    _type_duplicate(sizeof(type1), #type1, sizeof(type2), #type2)
bool _type_duplicate(size_t type_size1, const char * type_name1,
	size_t type_size2, const char * type_name2);

void _type_debug(void);

//////////////////////////////// op of types builtin /////////////////
// char/ signed char
void _type_init_char(const void * cpv_input, void * pv_output);
void _type_copy_char(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_less_char(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_destroy_char(const void * cpv_input, void * pv_output);
// unsigned char
void _type_init_uchar(const void * cpv_input, void * pv_output);
void _type_copy_uchar(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_less_uchar(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_destroy_uchar(const void * cpv_input, void * pv_output);
// short/short int/ signed short/ signed short int
void _type_init_short(const void * cpv_input, void * pv_output);
void _type_copy_short(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_less_short(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_destroy_short(const void * cpv_input, void * pv_output);
// unsigned short / unsigned short int
void _type_init_ushort(const void * cpv_input, void * pv_output);
void _type_copy_ushort(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_less_ushort(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_destroy_ushort(const void * cpv_input, void * pv_output);
// int/ signed / signed int
void _type_init_int(const void * cpv_input, void * pv_output);
void _type_copy_int(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_less_int(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_destroy_int(const void * cpv_input, void * pv_output);
// unsigned int/ unsigned
void _type_init_uint(const void * cpv_input, void * pv_output);
void _type_copy_uint(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_less_uint(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_destroy_uint(const void * cpv_input, void * pv_output);
// long/ long int/ signed long/ signed long int
void _type_init_long(const void * cpv_input, void * pv_output);
void _type_copy_long(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_less_long(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_destroy_long(const void * cpv_input, void * pv_output);
// unsigned long / unsigned long int
void _type_init_ulong(const void * cpv_input, void * pv_output);
void _type_copy_ulong(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_less_ulong(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_destroy_ulong(const void * cpv_input, void * pv_output);
// float
void _type_init_float(const void * cpv_input, void * pv_output);
void _type_copy_float(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_less_float(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_destroy_float(const void * cpv_input, void * pv_output);
// double
void _type_init_double(const void * cpv_input, void * pv_output);
void _type_copy_double(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_less_double(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_destroy_double(const void * cpv_input, void * pv_output);
// long double
void _type_init_long_double(const void * cpv_input, void * pv_output);
void _type_copy_long_double(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_less_long_double(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_destroy_long_double(const void * cpv_input, void * pv_output);
// bool
void _type_init_bool(const void * cpv_input, void * pv_output);
void _type_copy_bool(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_less_bool(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_destroy_bool(const void * cpv_input, void * pv_output);
// char*
void _type_init_cstr(const void * cpv_input, void * pv_output);
void _type_copy_cstr(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_less_cstr(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_destroy_cstr(const void * cpv_input, void * pv_output);
// void*
void _type_init_pointer(const void * cpv_input, void * pv_output);
void _type_copy_pointer(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_less_pointer(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_destroy_pointer(const void * cpv_input, void * pv_output);
// long long/ long long int/ signed long long/ signed long long int
void _type_init_long_long(const void * cpv_input, void * pv_output);
void _type_copy_long_long(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_less_long_long(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_destroy_long_long(const void * cpv_input, void * pv_output);
// unsigned long long/ unsigned long long int
void _type_init_ulong_long(const void * cpv_input, void * pv_output);
void _type_copy_ulong_long(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_less_ulong_long(const void * cpv_first, const void * cpv_second, void * pv_output);
void _type_destroy_ulong_long(const void * cpv_input, void * pv_output);


#endif // _VL_STL_TYPES_H_


