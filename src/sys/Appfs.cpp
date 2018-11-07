//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include <errno.h>
#include <sos/link.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "var/String.hpp"
#include "sys/Appfs.hpp"
#include "sys/Dir.hpp"
#include "sys/File.hpp"

using namespace sys;

void AppfsFileAttributes::apply(appfs_file_t * dest) const {
	memcpy(dest->hdr.name, m_name.str(), LINK_NAME_MAX);
	memcpy(dest->hdr.id, m_id.str(), LINK_NAME_MAX);
	dest->hdr.version = m_version;
	dest->exec.o_flags = m_o_flags;
	dest->hdr.mode = 0777;

	if( m_ram_size >= 4096 ){
		dest->exec.ram_size = m_ram_size;
	}

	if( dest->exec.ram_size < 4096 ){
		dest->exec.ram_size = 4096;
	}

	dest->exec.o_flags = m_o_flags;
}

#if defined __link
int Appfs::create(const var::ConstString & name, const void * buf, int nbyte, const char * mount, bool (*update)(void *, int, int), void * context, link_transport_mdriver_t * driver){
	File file(driver);
#else
int Appfs::create(const var::ConstString & name, const void * buf, int nbyte, const char * mount, bool (*update)(void *, int, int), void * context){
	File file;
#endif
	char buffer[LINK_PATH_MAX];
	int tmp;
	const char * p = (const char*)buf;
	appfs_createattr_t attr;
	int loc;
	unsigned int bw; //bytes written
	appfs_file_t f;
	strcpy(buffer, mount);
	strcat(buffer, "/flash/");
	strcat(buffer, name.str());


	//delete the settings if they exist
	strncpy(f.hdr.name, name.str(), LINK_NAME_MAX);
	f.hdr.mode = 0666;
	f.exec.code_size = nbyte + sizeof(f); //total number of bytes in file
	f.exec.signature = APPFS_CREATE_SIGNATURE;

#if defined __link
	File::remove(buffer, driver);
#else
	File::remove(buffer);
#endif


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

		//location gets modified by the driver so it needs to be fixed on each loop
		attr.loc = loc;

		if( (tmp = file.ioctl(I_APPFS_CREATE, &attr)) < 0 ){
			return tmp;
		}

		if( loc != 0 ){
			p += attr.nbyte;
		} else {
			p += (attr.nbyte - sizeof(f));
		}
		bw += attr.nbyte;
		loc += attr.nbyte;

		if( update ){
			update(context, bw, f.exec.code_size);
		}

	} while( bw < f.exec.code_size);

	return nbyte;
}

#if defined __link
int Appfs::get_info(const var::ConstString & path, appfs_info_t & info, link_transport_mdriver_t * driver){
	File f(driver);

	if( driver == 0 ){
		errno = ENOTSUP;
		return -1;
	}
#else
int Appfs::get_info(const var::ConstString & path, appfs_info_t & info){
	File f;
#endif
	var::String app_name;
	var::String path_name;
	appfs_file_t appfs_file_header;

	int ret;
	if( f.open(path, File::RDONLY) < 0 ){
		errno = ENOENT;
		return -1;
	}

	ret = f.read(&appfs_file_header, sizeof(appfs_file_header));
	if( ret == sizeof(appfs_file_header) ){
		//first check to see if the name matches -- otherwise it isn't an app file
		path_name = File::name(path).str();
		app_name = appfs_file_header.hdr.name;
		if( path_name == app_name ){
			info.mode = appfs_file_header.hdr.mode;
			info.version = appfs_file_header.hdr.version;
			memcpy(info.name, appfs_file_header.hdr.name, LINK_NAME_MAX);
			memcpy(info.id, appfs_file_header.hdr.id, LINK_NAME_MAX);
			info.ram_size = appfs_file_header.exec.ram_size;
			info.o_flags = appfs_file_header.exec.o_flags;
			info.signature = appfs_file_header.exec.signature;
		} else {
			errno = ENOEXEC;
			ret = -1;
		}
	} else {
		errno = ENOEXEC;
		ret = -1;
	}
	return ret;
}

#if defined __link
u16 Appfs::get_version(const var::ConstString & path, link_transport_mdriver_t * driver){
	appfs_info_t info;
	if( get_info(path, info, driver) < 0 ){
		return 0x0000;
	}
	return info.version;
}

#else
u16 Appfs::get_version(const var::ConstString & path){
	appfs_info_t info;
	if( get_info(path, info) < 0 ){
		return 0x0000;
	}
	return info.version;
}
#endif

#if defined __link

var::String Appfs::get_id(const var::ConstString & path, link_transport_mdriver_t * driver){
	appfs_info_t info;
	var::String result;
	if( get_info(path, info, driver) < 0 ){
		return result;
	}

#else

var::String Appfs::get_id(const var::ConstString & path){
	appfs_info_t info;
	var::String result;
	if( get_info(path, info) < 0 ){
		return result;
	}
#endif

	result = (const char*)info.id;
	return result;
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

