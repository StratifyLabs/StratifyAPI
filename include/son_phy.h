/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SON_PHY_H_
#define SON_PHY_H_

#include <stdint.h>

#if defined __link
#include <stdio.h>
#include <iface/link.h>

typedef struct {
	FILE * f;
	int fd;
	link_phy_t handle;
} son_phy_t;

#define SON_SEEK_SET LINK_SEEK_SET
#define SON_SEEK_CUR LINK_SEEK_CUR
#define SON_SEEK_END LINK_SEEK_END
#define SON_O_RDONLY LINK_O_RDONLY
#define SON_O_WRONLY LINK_O_WRONLY
#define SON_O_RDWR LINK_O_RDWR
#define SON_O_CREAT LINK_O_CREAT
#define SON_O_TRUNC LINK_O_TRUNC
#define SON_O_ACCESS (SON_O_RDWR|SON_O_WRONLY)


#define SON_PRINTF_INT "%d"

#else

#define SON_PRINTF_INT "%ld"

#define SON_SEEK_SET SEEK_SET
#define SON_SEEK_CUR SEEK_CUR
#define SON_SEEK_END SEEK_END
#define SON_O_RDONLY O_RDONLY
#define SON_O_RDWR O_RDWR
#define SON_O_CREAT O_CREAT
#define SON_O_TRUNC O_TRUNC

typedef struct {
	int fd;
} son_phy_t;

#endif


#if defined __cplusplus
extern "C" {
#endif

void son_phy_set_handle(son_phy_t * phy, void * handle);
int son_phy_open(son_phy_t * phy, const char * name, int32_t flags, int32_t mode);
int son_phy_read(son_phy_t * phy, void * buffer, uint32_t nbyte);
int son_phy_write(son_phy_t * phy, const void * buffer, uint32_t nbyte);
int son_phy_lseek(son_phy_t * phy, int32_t offset, int whence);
int son_phy_close(son_phy_t * phy);

#if defined __cplusplus
}
#endif

#endif /* SON_PHY_H_ */
