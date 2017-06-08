//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include "hal/Dev.hpp"
#include "sys/Timer.hpp"
using namespace sys;
using namespace hal;

Dev::Dev() {
	// TODO Auto-generated constructor stub
	m_fd = -1;
#if defined __link
	_driver = 0;
#endif
}


int Dev::read(int loc, void * buf, int nbyte) const {
	if( seek(loc) < 0 ){
		return -1;
	}

	return read(buf, nbyte);
}

int Dev::write(int loc, const void * buf, int nbyte) const {
	if( seek(loc) < 0 ){
		return -1;
	}
	return write(buf, nbyte);
}


int Dev::readline(char * buf, int nbyte, int timeout, char term) const {
	int t;
	int bytes_recv;
	char c;
	t = 0;
	bytes_recv = 0;
	do {
		if( read(&c, 1) == 1 ){
			*buf = c;
			buf++;
			bytes_recv++;
			if( c == term ){
				return bytes_recv;
			}
		} else {
			t++;
			Timer::wait_msec(1);
		}
	} while( (bytes_recv < nbyte) && (t < timeout) );

	return bytes_recv;
}

int Dev::open(const char * name, int flags){

	if( m_fd != -1 ){
		close(); //close and re-open
	}

#if defined __link
	m_fd = link_open(driver(), name, flags);
#else
	m_fd = ::open(name, flags);
#endif

	if( m_fd < 0 ){
		return -1;
	}
	return 0;
}

int Dev::close(){
	int ret = 0;
	if( m_fd >= 0 ){
#if defined __link
		ret = link_close(driver(), m_fd);
#else
		ret = ::close(m_fd);
#endif
		m_fd = -1;
	}
	return ret;
}

int Dev::read(void * buf, int nbyte) const {
#if defined __link
	return link_read(driver(), m_fd, buf, nbyte);
#else
	return ::read(m_fd, buf, nbyte);
#endif
}

int Dev::write(const void * buf, int nbyte) const {
#if defined __link
	return link_write(driver(), m_fd, buf, nbyte);
#else
	return ::write(m_fd, buf, nbyte);
#endif
}

#ifndef __link
int Dev::read(sys::Aio & aio) const {
	aio.aio_var.aio_fildes = m_fd;
	return ::aio_read(&(aio.aio_var));
}

int Dev::write(sys::Aio & aio) const {
	aio.aio_var.aio_fildes = m_fd;
	return ::aio_write(&(aio.aio_var));
}
#endif

int Dev::ioctl(int req, void * arg) const {
#if defined __link
	return link_ioctl(driver(), m_fd, req, arg);
#else
	return ::ioctl(m_fd, req, arg);
#endif
}

int Dev::seek(int loc, int whence) const {
#if defined __link
	return link_lseek(driver(), m_fd, loc, whence);
#else
	return ::lseek(m_fd, loc, whence);
#endif
}

int Dev::fileno() const {
	return m_fd;
}

int Dev::loc() const {
	return seek(0, CURRENT);
}

int Dev::flags() const{
	if( m_fd < 0 ){
		return -1;
	}
#if defined __link
	return -1;
#else
	return _global_impure_ptr->procmem_base->open_file[m_fd].flags;
#endif
}

char * Dev::gets(char * s, int n, char term) const {
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


const char * Dev::name(const char * path){
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


