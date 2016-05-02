/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SON_H_
#define SON_H_

//#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <mcu/types.h>

#include "son_phy.h"


typedef uint32_t son_size_t;
typedef uint16_t son_count_t;

#define SON_ACCESS_NAME_CAPACITY 64
#define SON_ACCESS_NAME_SIZE (SON_ACCESS_NAME_CAPACITY-1)
#define SON_KEY_NAME_CAPACITY 8
#define SON_KEY_NAME_SIZE (SON_KEY_NAME_CAPACITY-1)

#define SON_FLAG_HAS_JOURNAL (1<<0)


typedef struct {
	son_size_t pos;
} son_stack_t;

typedef struct {
	son_phy_t phy;
	int state;
	son_stack_t * stack;
	size_t stack_size;
	size_t stack_loc;
} son_t;

typedef union {
	float * f;
	int * n;
	uint32_t * n_uint32_t;
	int32_t * n_int32_t;
	char * cdata;
	void * data;
} son_type_t;

typedef struct MCU_PACK {
	uint8_t name[SON_KEY_NAME_CAPACITY];
} son_key_t;

typedef struct MCU_PACK {
	uint8_t page;
	uint16_t page_offset;
} son_pos_t;

//an object has members that are accessed by using a unique identifier (key)
typedef struct MCU_PACK {
	uint8_t type_flags;
	son_pos_t pos;
	son_key_t key;
} son_store_t;


/*! \brief Value types */
typedef enum {
	SON_STRING,
	SON_FLOAT,
	SON_NUMBER_U32,
	SON_NUMBER_I32,
	SON_DATA,
	SON_OBJ,
	SON_ARRAY, //can be an array of distinct objects
	SON_TRUE,
	SON_FALSE,
	SON_NULL,
	SON_TOTAL
} son_marker_t;

#ifdef __cplusplus
extern "C" {
#endif

uint8_t son_type(const son_store_t * eon);
void son_set_type(son_store_t * eon, uint8_t type, uint8_t fixed_array_size);
uint32_t son_next(const son_store_t * eon);
void son_set_next(son_store_t * eon, uint32_t offset);

static inline void son_reset(son_t * h){ son_phy_lseek(&(h->phy), 0, SEEK_SET); }

#ifdef __link
#include <iface/link.h>
void son_set_handle(son_t * h, link_phy_t handle);
#endif

int son_create(son_t * h, const char * name, son_stack_t * stack, size_t stack_size);
int son_append(son_t * h, const char * name, son_stack_t * stack, size_t stack_size);
int son_open(son_t * h, const char * name);
int son_close(son_t * h, int close_all);
int son_to_json(son_t * h, const char * path);
int son_open_obj(son_t * h, const char * key);
int son_close_obj(son_t * h);
int son_open_array(son_t * h, const char * key, int fixed_size);
int son_close_array(son_t * h);
int son_open_data(son_t * h, const char * key);
int son_close_data(son_t * h);
int son_write_str(son_t * h, const char * key, const char * v);
int son_write_num(son_t * h, const char * key, int32_t v);
int son_write_unum(son_t * h, const char * key, uint32_t v);
int son_write_float(son_t * h, const char * key, float v);
int son_write_true(son_t * h, const char * key);
int son_write_false(son_t * h, const char * key);
int son_write_null(son_t * h, const char * key);
int son_write_data(son_t * h, const char * key, const void * v, son_size_t size);
int son_write_open_data(son_t * h, const void * v, son_size_t size);
int son_read_str(son_t * h, const char * access, char * str, son_size_t capacity);
int32_t son_read_num(son_t * h, const char * access);
uint32_t son_read_unum(son_t * h, const char * access);
float son_read_float(son_t * h, const char * access);
int son_read_data(son_t * h, const char * access, void * data, son_size_t size);

int son_seek(son_t * h, const char * access, son_size_t * data_size);


#ifdef __cplusplus
}
#endif


#endif /* SON_H_ */
