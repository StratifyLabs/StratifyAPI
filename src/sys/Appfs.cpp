//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include <errno.h>
#include <iface/link.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sys/Appfs.hpp"
#include "sys/Dir.hpp"
#include "sys/File.hpp"

using namespace sys;

int Appfs::create(const char * name, const void * buf, int nbyte, const char * mount, int (*update)(void *, int, int), void * context, link_transport_mdriver_t * driver){
	char buffer[LINK_PATH_MAX];
	File file;
	int tmp;
	const char * p = (const char*)buf;
	appfs_createattr_t attr;
	int loc;
	unsigned int bw; //bytes written
	link_appfs_file_t f;
	strcpy(buffer, mount);
	strcat(buffer, "/flash/");
	strcat(buffer, name);
#ifdef __link
	file.set_driver(driver);
#endif

	//delete the settings if they exist
	tmp = errno;
	unlink(buffer);
	errno = tmp;

	strncpy(f.hdr.name, name, LINK_NAME_MAX);
	f.hdr.mode = 0666;
	f.exec.code_size = nbyte + sizeof(f); //total number of bytes in file
	f.exec.signature = APPFS_CREATE_SIGNATURE;

	File::remove(buffer, driver);


	if( file.open("/app/.install", File::WRONLY) < 0 ){
		return -1;
	}

	memcpy(attr.buffer, &f, sizeof(f));
	//now copy some bytes
	attr.nbyte = APPFS_PAGE_SIZE - sizeof(f);
	if( nbyte < (int)attr.nbyte ){
		attr.nbyte = nbyte;
	}
	memcpy(attr.buffer + sizeof(f), p, attr.nbyte);
	attr.nbyte += sizeof(f);
	loc = 0;
	bw = 0;
	do {
		if( loc != 0 ){ //when loc is 0 -- header is copied in
			if( (f.exec.code_size - bw) > APPFS_PAGE_SIZE ){
				attr.nbyte = APPFS_PAGE_SIZE;
			} else {
				attr.nbyte = f.exec.code_size - bw;
			}
			memcpy(attr.buffer, p, attr.nbyte);
		}
		attr.loc = loc;

		if( file.ioctl(I_APPFS_CREATE, &attr) < 0 ){
			file.close();
			return -1;
		}



		if( loc != 0 ){
			p += attr.nbyte;
		} else {
			p += (attr.nbyte - sizeof(f));
		}
		bw += attr.nbyte;
		loc += attr.nbyte;

		if( update ){
			update(context, bw, attr.nbyte);
		}

	} while( bw < f.exec.code_size);

	file.close();
	return nbyte;
}
#ifndef __link

int Appfs::cleanup(bool data){
	struct stat st;
	Dir dir;
	char buffer[LINK_PATH_MAX];
	const char * name;

	if( dir.open("/app/ram") < 0 ){
		perror("failed to open dir");
		return -1;
	}


	while( (name = dir.read()) != 0 ){
		strcpy(buffer, "/app/ram/");
		strcat(buffer, name);
		if( stat(buffer, &st) < 0 ){
			perror("Failed to stat");
		}

		if( ((st.st_mode & (LINK_S_IXUSR|LINK_S_IXGRP|LINK_S_IXOTH)) || data) && (name[0] != '.') ){
			if( unlink(buffer) < 0){
				dir.close();
				return -1;
			}
		}
	}

	dir.close();

	return 0;

}
#endif

