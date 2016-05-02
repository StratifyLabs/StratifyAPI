//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <stdio.h>

#include "Sys/File.hpp"
using namespace Sys;

File::File() {
	// TODO Auto-generated constructor stub
	fd = -1; //The file is not open
}

int File::remove(const char * name, link_transport_phy_t handle){
	handle = 0;
	return ::remove(name);
}


int File::open(const char * name, int access, int perms){
	if( fd != -1 ){
		close(); //close and re-open
	}

	fd = ::open(name, access, perms);
	if( fd < 0 ){
		return -1;
	}

	return 0;
}

int File::ioctl(int req, void * arg){
	return ::ioctl(fd, req, arg);
}

int File::create(const char * name, bool overwrite, int perms){
	int access = O_RDWR | O_CREAT;
	if( overwrite ){
		access |= O_TRUNC;
	}
	return open(name, access, perms);
}

int File::read(void * buf, int nbyte) const {
	return ::read(fd, buf, nbyte);
}

int File::write(const void * buf, int nbyte) const {
	return ::write(fd, buf, nbyte);
}

int File::read(int loc, void * buf, int nbyte) const {
	lseek(fd, loc, SEEK_SET);
	return ::read(fd, buf, nbyte);
}

int File::write(int loc, const void * buf, int nbyte) const {
	lseek(fd, loc, SEEK_SET);
	return ::write(fd, buf, nbyte);
}

int File::seek(int loc, int whence) const {
	return lseek(fd, loc, whence);
}

int File::fileno(void) const {
	return fd;
}

int File::size(void) const {
	struct stat st;
	if ( fstat(fd, &st) < 0 ){
		return -1;
	}
	return st.st_size;
}

int File::close(void){
	int ret = 0;
	if( fd != -1 ){
		ret = ::close(fd);
		fd = -1;
	}
	return ret;
}

int File::loc(void) const {
	return seek(0, File::CURRENT);
}

int File::flags() const{
	if( fd < 0 ){
		return -1;
	}
	return _global_impure_ptr->procmem_base->open_file[fd].flags;
}

const char * File::name(const char * path){
	int len;
	int i;
	len = strlen(path);
	for(i = len; i >= 0; i--){
		if( path[i] == '/' ){
			return &(path[i+1]);
		}
	}
	return 0;
}

/*! \brief Get file stat (pointer) */
int File::stat(const char * name, struct link_stat * st, link_transport_phy_t handle){
	return ::stat(name, (struct stat*)st);
}

/*! \brief Get file stat (reference) */
ssize_t File::size(const char * name, link_transport_phy_t handle){
	struct link_stat st;
	if( stat(name, &st) < 0 ){
		return (ssize_t)-1;
	}
	return st.st_size;
}

char * File::gets(char * s, int n, char term) const {
	int t;
	int ret;
	int i;
	char buffer[GETS_BUFFER_SIZE];

	if( n < 1 ){
		return 0;
	}

	s[0] = 0;
	t = 0;
	do {
		ret = read(buffer, GETS_BUFFER_SIZE);
		for(i=0; i < ret; i++){
			s[t] = buffer[i];
			s[t+1] = 0;
			if( s[t] == '\n'){
				break;
			}
			if( t == (n-1) ){
				break;
			}
			t++;
		}
	} while( (t < (n-1)) && (s[t] != term) && (ret > 0) );

	if( t == 0 ){
		return 0;
	}

	if( ret > 0 ){
		seek(i - ret + 1, CURRENT);
	}

	return s;

}
