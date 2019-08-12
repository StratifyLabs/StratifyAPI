//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include <errno.h>
#include <sos/link.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "var/String.hpp"
#include "sys/Appfs.hpp"
#include "fs/Dir.hpp"
#include "fs/File.hpp"

using namespace sys;

void AppfsFileAttributes::apply(appfs_file_t * dest) const {
	memcpy(dest->hdr.name, m_name.cstring(), LINK_NAME_MAX);
	memcpy(dest->hdr.id, m_id.cstring(), LINK_NAME_MAX);
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
Appfs::Appfs(link_transport_mdriver_t * driver) : m_file(driver){

}
#else
Appfs::Appfs(){}
#endif

int Appfs::create(const arg::FileName & name,
		const arg::SourceFile & source_data,
		const arg::SourceDirectoryPath & mount,
		const ProgressCallback * progress_callback
#if defined __link
		, link_transport_mdriver_t * driver
		){
	fs::File file(driver);
#else
){
	fs::File file;
#endif
	char buffer[LINK_PATH_MAX];
	int tmp;
	appfs_createattr_t attr;
	int loc;
	unsigned int bw; //bytes written
	appfs_file_t f;
	strcpy(buffer, mount.argument().cstring());
	strcat(buffer, "/flash/");
	strcat(buffer, name.argument().cstring());

	//delete the settings if they exist
	strncpy(f.hdr.name, name.argument().cstring(), LINK_NAME_MAX);
	f.hdr.mode = 0666;
	f.exec.code_size = source_data.argument().size() + sizeof(f); //total number of bytes in file
	f.exec.signature = APPFS_CREATE_SIGNATURE;

#if defined __link
	fs::File::remove(
				arg::SourceFilePath(buffer),
				driver
				);
#else
	fs::File::remove(
				arg::SourceFilePath(buffer)
				);
#endif


	if( file.open(
			 arg::FilePath("/app/.install"),
			 fs::OpenFlags::write_only()
			 ) < 0 ){
		return -1;
	}

	memcpy(attr.buffer, &f, sizeof(f));
	//now copy some bytes
	attr.nbyte = APPFS_PAGE_SIZE - sizeof(f);
	if( source_data.argument().size() < (u32)attr.nbyte ){
		attr.nbyte = source_data.argument().size();
	}

	source_data.argument().read(
				arg::DestinationBuffer(attr.buffer + sizeof(f)),
				arg::Size(attr.nbyte)
				);
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
			source_data.argument().read(
						arg::DestinationBuffer(attr.buffer),
						arg::Size(attr.nbyte)
						);
		}

		//location gets modified by the driver so it needs to be fixed on each loop
		attr.loc = loc;

		if( (tmp = file.ioctl(
				  arg::IoRequest(I_APPFS_CREATE),
				  arg::IoArgument(&attr)
				  )) < 0 ){
			return tmp;
		}

		bw += attr.nbyte;
		loc += attr.nbyte;

		if( progress_callback ){
			progress_callback->update(bw, f.exec.code_size);
		}

	} while( bw < f.exec.code_size);

	return f.exec.code_size;
}

#if defined __link
AppfsInfo Appfs::get_info(
		const arg::SourceFilePath path,
		link_transport_mdriver_t * driver
		){
	fs::File f(driver);
	if( driver == 0 ){
		errno = ENOTSUP;
		return AppfsInfo();
	}
#else
AppfsInfo Appfs::get_info(const arg::SourceFilePath path){
	fs::File f;
#endif
	var::String app_name;
	var::String path_name;
	appfs_file_t appfs_file_header;
	appfs_info_t info;
	int ret;
	if( f.open(
			 arg::FilePath(path.argument()),
			 fs::OpenFlags::read_only()
			 ) < 0 ){
		return AppfsInfo();
	}

	ret = f.read(
				arg::DestinationBuffer(&appfs_file_header),
				arg::Size(sizeof(appfs_file_header))
				);
	f.close();

	if( ret == sizeof(appfs_file_header) ){
		//first check to see if the name matches -- otherwise it isn't an app file
		path_name = fs::File::name(path.argument());
		if( path_name.find(arg::StringToFind(".sys")) == 0 ){
			return AppfsInfo();
		}

		if( path_name.find(arg::StringToFind(".free")) == 0 ){
			return AppfsInfo();
		}

		if( (appfs_file_header.hdr.mode & 0111) == 0 ){
			return AppfsInfo();
		}

		app_name = appfs_file_header.hdr.name;
		memset(&info, 0, sizeof(info));
		if( path_name == app_name ){
			memcpy(info.name, appfs_file_header.hdr.name, LINK_NAME_MAX);
			info.mode = appfs_file_header.hdr.mode;
			info.version = appfs_file_header.hdr.version;
			memcpy(info.id, appfs_file_header.hdr.id, LINK_NAME_MAX);
			info.ram_size = appfs_file_header.exec.ram_size;
			info.o_flags = appfs_file_header.exec.o_flags;
			info.signature = appfs_file_header.exec.signature;
		} else {
			errno = ENOEXEC;
			return AppfsInfo();
		}
	} else {
		errno = ENOEXEC;
		return AppfsInfo();
	}
	return AppfsInfo(info);
}

#if defined __link
u16 Appfs::get_version(
		const arg::SourceFilePath path,
		link_transport_mdriver_t * driver){
	AppfsInfo info;
	info = get_info(path, driver);
#else
u16 Appfs::get_version(const arg::SourceFilePath path){
	AppfsInfo info;
	info = get_info(path);
#endif
	return info.version();
}

#if defined __link

var::String Appfs::get_id(
		const arg::SourceFilePath path,
		link_transport_mdriver_t * driver
		){
	AppfsInfo info;
	info = get_info(path, driver);
#else
var::String Appfs::get_id(const arg::SourceFilePath path){
	AppfsInfo info;
	info = get_info(path);
#endif
	if( info.is_valid() == false ){
		return var::String();
	}

	return info.id();
}


#ifndef __link

int Appfs::cleanup(bool data){
	struct stat st;
	fs::Dir dir;
	char buffer[LINK_PATH_MAX];
	const char * name;

	if( dir.open(
			 arg::SourceDirectoryPath("/app/ram")
			 ) < 0 ){
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

