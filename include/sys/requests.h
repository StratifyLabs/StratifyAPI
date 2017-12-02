/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


#ifndef SYS_REQUESTS_H_
#define SYS_REQUESTS_H_

#include <mcu/types.h>

typedef struct {
	const void * list;
	u16 count;
} sapi_request_asset_t;

typedef struct {
	int seconds;
} sapi_request_hibernate_t;

enum {
	SAPI_REQUEST_FLAG = (1<<30),
	SAPI_REQUEST_ASSET_FONTS = (SAPI_REQUEST_FLAG) | 1,
	SAPI_REQUEST_ASSET_VECTOR_ICONS = (SAPI_REQUEST_FLAG) | 2,
	SAPI_REQUEST_ASSET_BITMAP_ICONS = (SAPI_REQUEST_FLAG) | 3,
	SAPI_REQUEST_HIBERNATE = (SAPI_REQUEST_FLAG) | 4
};


#endif /* SYS_REQUESTS_H_ */
