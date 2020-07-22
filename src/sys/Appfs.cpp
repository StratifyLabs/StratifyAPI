/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#include <errno.h>
#include <sos/link.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "var/String.hpp"
#include "sys/Appfs.hpp"
#include "fs/Dir.hpp"
#include "fs/File.hpp"
#include "sys/Printer.hpp"

using namespace sys;

Printer& sys::operator << (Printer& printer, const appfs_file_t & a){
	printer.key("name", "%s", a.hdr.name);
	printer.key("id", "%s", a.hdr.id);
	printer.key("mode", "0%o", a.hdr.mode);
	printer.key("version", "%d.%d", a.hdr.version >> 8, a.hdr.version & 0xff);
	printer.key("startup", "%p", a.exec.startup);
	printer.key("codeStart", "%p", a.exec.code_start);
	printer.key("codeSize", "%p", a.exec.code_size);
	printer.key("ramStart", "%p", a.exec.ram_start);
	printer.key("ramSize", "%ld", a.exec.ram_size);
	printer.key("dataSize", "%ld", a.exec.data_size);
	printer.key("oFlags", "0x%lX", a.exec.o_flags);
	printer.key("signature", "0x%lX", a.exec.signature);
	return printer;
}

Printer& sys::operator << (Printer& printer, const AppfsFileAttributes & a){
	printer.key("name", a.name());
	printer.key("id", a.id());
	printer.key("version", "%d.%d", a.version() >> 8, a.version() & 0xff);
	printer.key("flash", a.is_flash());
	printer.key("codeExternal", a.is_code_external());
	printer.key("dataExternal", a.is_data_external());
	printer.key("codeTightlyCoupled", a.is_code_tightly_coupled());
	printer.key("dataTightlyCoupled", a.is_data_tightly_coupled());
	printer.key("startup", a.is_startup());
	printer.key("unique", a.is_unique());
	printer.key("ramSize", "%ld", a.ram_size());
	return printer;
}

Printer& sys::operator << (Printer& printer, const sys::AppfsInfo & a){
	printer.key("name", a.name());
	printer.key("mode", "0%o", a.mode());
	if( a.is_executable() ){
		printer.key("id", a.id());
		printer.key("version", "%d.%d", a.version() >> 8, a.version() & 0xff);
		printer.key("signature", F3208X, a.signature());
		printer.key("ram", F32U, a.ram_size());
		printer.key("orphan", a.is_orphan());
		printer.key("flash", a.is_flash());
		printer.key("startup", a.is_startup());
		printer.key("unique", a.is_unique());
	}
	return printer;
}

AppfsFileAttributes::AppfsFileAttributes(const appfs_file_t & appfs_file){
	m_name = appfs_file.hdr.name;
	m_id = appfs_file.hdr.id;
	m_ram_size = appfs_file.exec.ram_size;
	m_o_flags = appfs_file.exec.o_flags;
	m_version = appfs_file.hdr.version;
}

void AppfsFileAttributes::apply(appfs_file_t * appfs_file) const {

	if( m_name.is_empty() == false ){
		memcpy(appfs_file->hdr.name, m_name.cstring(), LINK_NAME_MAX);
	}

	if( m_id.is_empty() == false ){
		memcpy(appfs_file->hdr.id, m_id.cstring(), LINK_NAME_MAX);
	}

	if( m_version != 0 ){
		appfs_file->hdr.version = m_version;
	}

	if( m_access_mode != 0 ){
		appfs_file->hdr.mode = m_access_mode;
	}

	if( m_ram_size >= 4096 ){
		appfs_file->exec.ram_size = m_ram_size;
	}

	if( appfs_file->exec.ram_size < 4096 ){
		appfs_file->exec.ram_size = 4096;
	}

	appfs_file->exec.o_flags = m_o_flags;
}

int AppfsFileAttributes::apply(const fs::File & file) const {
	appfs_file_t appfs_file;
	var::Reference appfs_file_reference(appfs_file);

	int result;

	int location = file.seek(0, fs::File::whence_current);

	if( (result = file.read(
				 fs::File::Location(0),
				 appfs_file_reference
				 )) != (int)appfs_file_reference.size() ){
		return -1;
	}

	this->apply( appfs_file_reference.to<appfs_file_t>() );

	if( (result = file.write(
				 fs::File::Location(0),
				 appfs_file_reference
				 )) != (int)appfs_file_reference.size() ){
		return -1;
	}

	file.seek(location, fs::File::whence_set);

	return 0;
}

Appfs::Appfs(
		const CreateOptions& options
		SAPI_LINK_DRIVER_LAST
		) :
	m_file(
		#if defined __link
		link_driver
		#endif
		){

	create_asynchronous(options);
}

bool Appfs::is_flash_available(
		SAPI_LINK_DRIVER
		){
	fs::Dir dir
		#if defined __link
			(link_driver)
		#endif
			;
	if( dir.open("/app/flash") < 0 ){
		return false;
	}

	if( dir.read() != nullptr ){
		return true;
	}

	return false;
}

bool Appfs::is_ram_available(
		SAPI_LINK_DRIVER
		){
	fs::Dir dir
		#if defined __link
			(link_driver)
		#endif
			;
	if( dir.open(var::String("/app/ram")) < 0 ){
		return false;
	}

	if( dir.read() != nullptr ){
		return true;
	}

	return false;
}


int Appfs::create(
		const CreateOptions& options
		SAPI_LINK_DRIVER_LAST
		){

	Appfs appfs(options);

	if( !appfs.is_valid() ){
		return api::error_code_flag_sys_appfs_create_failed;
	}

	u8 buffer[APPFS_PAGE_SIZE];
	var::Blob buffer_blob(buffer);

	while( appfs.is_ready() ){
		int result = options.source().read(buffer_blob);
		if( result > 0 ){
			appfs.append(buffer_blob);
			if( appfs.return_value() < 0 ){
				return api::error_code_flag_sys_appfs_append_failed;
			}
		}
	}

	return 0;
}

int Appfs::create(
		Name name,
		const fs::File & source,
		MountPath mount_path,
		const ProgressCallback * progress_callback
		SAPI_LINK_DRIVER_LAST
		){
	fs::File file
		#if defined __link
			(link_driver)
		#endif
			;
	char buffer[LINK_PATH_MAX];
	int tmp;
	appfs_createattr_t attr;
	int loc;
	unsigned int bw; //bytes written
	appfs_file_t f;
	strncpy(buffer, mount_path.argument().cstring(), LINK_PATH_MAX-1);
	strncat(buffer, "/flash/", LINK_PATH_MAX-1);
	strncat(buffer, name.argument().cstring(), LINK_PATH_MAX-1);

	//delete the settings if they exist
	strncpy(f.hdr.name, name.argument().cstring(), LINK_NAME_MAX-1);
	f.hdr.mode = 0666;
	f.exec.code_size = source.size() + sizeof(f); //total number of bytes in file
	f.exec.signature = APPFS_CREATE_SIGNATURE;

	fs::File::remove(
				buffer
			#if defined __link
				, link_driver
			#endif
				);

	if( file.open(
				"/app/.install",
				fs::OpenFlags::write_only()
				) < 0 ){
		return -1;
	}

	var::Reference::memory_copy(
				var::Reference::SourceBuffer(&f),
				var::Reference::DestinationBuffer(attr.buffer),
				var::Reference::Size(sizeof(f))
				);

	//now copy some bytes
	attr.nbyte = APPFS_PAGE_SIZE - sizeof(f);
	if( source.size() < (u32)attr.nbyte ){
		attr.nbyte = source.size();
	}

	source.read(
				attr.buffer + sizeof(f),
				fs::File::Size(attr.nbyte)
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
			source.read(
						attr.buffer,
						fs::File::Size(attr.nbyte)
						);
		}

		//location gets modified by the driver so it needs to be fixed on each loop
		attr.loc = loc;

		if( (tmp = file.ioctl(
					 fs::File::IoRequest(I_APPFS_CREATE),
					 fs::File::IoArgument(&attr)
					 )) < 0 ){
			return tmp;
		}

		bw += attr.nbyte;
		loc += attr.nbyte;

		if( progress_callback ){
			progress_callback->update(bw, f.exec.code_size);
		}

	} while( bw < f.exec.code_size);
	if( progress_callback ){ progress_callback->update(0,0); }

	return f.exec.code_size;
}

int Appfs::create_asynchronous(const CreateOptions& options){
	appfs_file_t f;
	var::String path = options.mount() + "/flash/" + options.name();

	//delete the settings if they exist
	strncpy(f.hdr.name, options.name().cstring(), LINK_NAME_MAX-1);
	f.hdr.mode = 0666;
	f.exec.code_size = options.size() + sizeof(f); //total number of bytes in file
	f.exec.signature = APPFS_CREATE_SIGNATURE;

	fs::File::remove(
				path
			#if defined __link
				, link_driver
			#endif
				);

	if( m_file.open(
				"/app/.install",
				fs::OpenFlags::write_only()
				) < 0 ){
		return -1;
	}

	var::Blob::copy_memory(
				var::Blob::CopyOptions()
				.set_source(&f)
				.set_destination(m_create_attributes.buffer)
				.set_size(sizeof(f))
				);
	m_bytes_written = sizeof(f);
	m_data_size = f.exec.code_size;

	return 0;
}

Appfs& Appfs::append(const var::Blob & blob){

	u32 bytes_written = 0;
	if( m_file.fileno() < 0 ){
		//already full -- can't write more
		return *this;
	}

	while(  m_bytes_written < m_data_size
					&& bytes_written < blob.size()
					){

		const u32 page_offset = m_bytes_written % APPFS_PAGE_SIZE;
		const u32 page_size_available = APPFS_PAGE_SIZE - page_offset;
		u32 page_size = blob.size() - bytes_written;
		if( page_size > page_size_available ){
			page_size = page_size_available;
		}

		var::Blob(m_create_attributes.buffer).fill<u8>(0xff);

		var::Blob::copy_memory(
					var::Blob::CopyOptions()
					.set_source(blob.to_const_u8() + bytes_written)
					.set_destination(m_create_attributes.buffer + page_offset)
					.set_size(page_size)
					);

		m_bytes_written += page_size;
		bytes_written += page_size;

		if( ((m_bytes_written % APPFS_PAGE_SIZE) == 0) //at page boundary
				|| (m_bytes_written == m_data_size) ){ //or to the end
			m_create_attributes.nbyte = page_size;

			if( set_error_number_if_error( m_file.ioctl(fs::File::IoctlOptions()
											 .set_request(I_APPFS_CREATE)
											 .set_argument(&m_create_attributes))) < 0 ){
				return *this;
			}

			m_create_attributes.loc += page_size;
		}
	}

	if( m_bytes_written == m_data_size ){
		//all done
		m_file.close();
	}

	return *this;
}

#if 0
//now copy some bytes
m_create_attributes.nbyte = APPFS_PAGE_SIZE - sizeof(f);
if( source.size() < (u32)attr.nbyte ){
	attr.nbyte = source.size();
}

source.read(
		attr.buffer + sizeof(f),
		fs::File::Size(attr.nbyte)
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
	source.read(
				attr.buffer,
				fs::File::Size(attr.nbyte)
				);
}

//location gets modified by the driver so it needs to be fixed on each loop
attr.loc = loc;

if( (tmp = file.ioctl(
			fs::File::IoRequest(I_APPFS_CREATE),
			fs::File::IoArgument(&attr)
			)) < 0 ){
	return tmp;
}

bw += attr.nbyte;
loc += attr.nbyte;

if( progress_callback ){
	progress_callback->update(bw, f.exec.code_size);
}

} while( bw < f.exec.code_size);
if( progress_callback ){ progress_callback->update(0,0); }

return f.exec.code_size;

return 0;
}
#endif

AppfsInfo Appfs::get_info(
		const var::String & path
		SAPI_LINK_DRIVER_LAST
		){
	fs::File f
		#if defined __link
			(link_driver)
		#endif
			;

	var::String app_name;
	var::String path_name;
	appfs_file_t appfs_file_header;
	appfs_info_t info;
	int result;

	if( f.open(
				path,
				fs::OpenFlags::read_only()
				) < 0 ){
		return AppfsInfo();
	}

	result = f.read(var::Reference(appfs_file_header));

	f.close();

	if( result == sizeof(appfs_file_header) ){
		//first check to see if the name matches -- otherwise it isn't an app file
		path_name = fs::File::name(path);

		if( path_name.find(".sys") == 0 ){
			errno = EINVAL;
			return AppfsInfo();
		}

		if( path_name.find(".free") == 0 ){
			errno = EINVAL;
			return AppfsInfo();
		}

		if( (appfs_file_header.hdr.mode & 0111) == 0 ){
			//return AppfsInfo();
		}

		app_name = appfs_file_header.hdr.name;

		memset(&info, 0, sizeof(info));
		if( path_name == app_name
		#if defined __link
				|| (path_name.find(app_name) == 0 )
		#endif

				){
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

u16 Appfs::get_version(
		const var::String & path
		SAPI_LINK_DRIVER_LAST
		){
	AppfsInfo info;
	info = get_info(path
								#if defined __link
									, link_driver
								#endif
									);
	return info.version();
}

var::String Appfs::get_id(
		const var::String & path
		SAPI_LINK_DRIVER_LAST
		){
	AppfsInfo info;
	info = get_info(path
								#if defined __link
									, link_driver
								#endif
									);

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

	if( dir.open("/app/ram") < 0 ){
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

