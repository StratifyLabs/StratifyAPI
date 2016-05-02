/*
 * son_phy.c
 *
 *  Created on: Dec 17, 2015
 *      Author: tgil
 */

#include "son_phy.h"


#if defined __link

#include <stdio.h>


void son_phy_set_handle(son_phy_t * phy, void * handle){
	phy->handle  = handle;
}

int son_phy_open(son_phy_t * phy, const char * name, int32_t flags, int32_t mode){
	if( phy->handle == 0 ){
		//create using fopen()
		char open_code[8];

		if( (flags & SON_O_ACCESS) == SON_O_RDONLY ){
			sprintf(open_code, "r");
		} else if( ((flags & SON_O_ACCESS) == SON_O_RDWR) && (flags & SON_O_CREAT) ){
			sprintf(open_code, "wb");
		} else {
			sprintf(open_code, "r+");
		}

		phy->f = fopen(name, open_code);
		if( phy->f != 0 ){
			return 0;
		}

		return -1;
	}
	phy->fd = link_open(phy->handle, name, flags, mode);
	if( phy->fd < 0 ){
		return -1;
	}
	return 0;
}

int son_phy_read(son_phy_t * phy, void * buffer, uint32_t nbyte){
	if( phy->handle == 0 ){
		//read using fread
		return fread(buffer, 1, nbyte, phy->f);
	}
	return link_read(phy->handle, phy->fd, buffer, nbyte);
}

int son_phy_write(son_phy_t * phy, const void * buffer, uint32_t nbyte){
	if( phy->handle == 0 ){
		//write using fwrite
		return fwrite(buffer, 1, nbyte, phy->f);
	}

	return link_write(phy->handle, phy->fd, buffer, nbyte);
}

int son_phy_lseek(son_phy_t * phy, int32_t offset, int whence){
	if( phy->handle == 0 ){
		if( fseek(phy->f, offset, whence) == 0 ){
			return ftell(phy->f);
		}
		return -1;
	}
	return link_lseek(phy->handle, phy->fd, offset, whence);
}

int son_phy_close(son_phy_t * phy){
	if( phy->handle == 0 ){
		int ret;
		ret = fclose(phy->f);
		phy->f = 0;
		return ret;
	}

	return link_close(phy->handle, phy->fd);
}


#else

#include <mcu/types.h>
#include <unistd.h>
#include <fcntl.h>


int son_phy_open(son_phy_t * phy, const char * name, int32_t flags, int32_t mode){
	phy->fd = open(name, flags, mode);
	if( phy->fd < 0 ){
		return -1;
	}
	return 0;
}
int son_phy_read(son_phy_t * phy, void * buffer, uint32_t nbyte){ return read(phy->fd, buffer, nbyte); }
int son_phy_write(son_phy_t * phy, const void * buffer, uint32_t nbyte){ return write(phy->fd, buffer, nbyte); }
int son_phy_lseek(son_phy_t * phy, int32_t offset, int whence){ return lseek(phy->fd, offset, whence); }
int son_phy_close(son_phy_t * phy){ return close(phy->fd); }





#endif
