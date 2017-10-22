/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#ifndef SYS_ASSETS_H_
#define SYS_ASSETS_H_

#include <mcu/types.h>

typedef struct {
	void * list;
	u16 count;
} asset_request_t;

enum {
	ASSETS_KERNEL_REQUEST_FLAG = (1<<16),
	ASSETS_KERNEL_REQUEST_FONTS = (ASSETS_KERNEL_REQUEST_FLAG) | 1,
	ASSETS_KERNEL_REQUEST_VECTOR_ICONS = (ASSETS_KERNEL_REQUEST_FLAG) | 2,
	ASSETS_KERNEL_REQUEST_BITMAP_ICONS = (ASSETS_KERNEL_REQUEST_FLAG) | 3,

};




#endif /* SYS_ASSETS_H_ */
