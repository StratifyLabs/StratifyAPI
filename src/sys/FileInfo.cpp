/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved

#if defined __link
#include <sys/stat.h>
#endif

#include "sys/File.hpp"
#include "sys/FileInfo.hpp"

using namespace sys;
#if defined __link
FileInfo::FileInfo(bool is_local) {
	memset(&m_stat, 0, sizeof(m_stat));
	m_is_local = is_local;
}
#else

FileInfo::FileInfo() {
	memset(&m_stat, 0, sizeof(m_stat));
}
#endif


bool FileInfo::is_directory() const {
#if defined __link
	if( m_is_local ){
		return (m_stat.st_mode & S_IFMT) == S_IFDIR;
	}
#endif
	return (m_stat.st_mode & File::FORMAT) == File::DIRECTORY;
}

bool FileInfo::is_file() const {
#if defined __link
	if( m_is_local ){
		return (m_stat.st_mode & S_IFMT) == S_IFREG;
	}
#endif
	return (m_stat.st_mode & File::FORMAT) == File::REGULAR;
}

bool FileInfo::is_device() const {
	return is_block_device() || is_character_device();

}

bool FileInfo::is_block_device() const {
#if defined __link
	if( m_is_local ){
		return (m_stat.st_mode & S_IFMT) == S_IFBLK;
	}
#endif
	return (m_stat.st_mode & (File::FORMAT)) == File::BLOCK;
}

bool FileInfo::is_character_device() const {
#if defined __link
	if( m_is_local ){
		return (m_stat.st_mode & S_IFMT) == S_IFCHR;
	}
#endif
	return (m_stat.st_mode & (File::FORMAT)) == File::CHARACTER;
}

bool FileInfo::is_socket() const {
#if defined __link
	if( m_is_local ){
#if !defined __win32
		return (m_stat.st_mode & S_IFMT) == S_IFSOCK;
#else
		return false;
#endif
	}
#endif
	return (m_stat.st_mode & File::FORMAT) == File::FILE_SOCKET;
}


u32 FileInfo::size() const {
	return m_stat.st_size;
}

bool FileInfo::is_executable() const {
	return false;
}


