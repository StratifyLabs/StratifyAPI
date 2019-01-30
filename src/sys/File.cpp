//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include <cstdio>
#include <cstring>
#include <fcntl.h>

#include "sys/File.hpp"
#include "chrono/Timer.hpp"
using namespace sys;

#if defined __link

link_transport_mdriver_t * File::m_default_driver = 0;

File::File(link_transport_mdriver_t * driver){
	m_fd = -1; //The file is not open
	set_keep_open(false);
	m_driver = driver;
}

#else
File::File() {
	m_fd = -1; //The file is not open
	set_keep_open(false);
}
#endif

File::~File(){
	if( is_keep_open() == false ){
		if( fileno() >= 0 ){
			close();
		}
	}
}

int File::open(const var::ConstString & name, int flags){
	return open(name, flags, 0);
}

#if !defined __link
int File::remove(const var::ConstString & path){
	return ::remove(path.cstring());
}
#else
int File::remove(const var::ConstString & name, link_transport_mdriver_t * driver){
	return link_unlink(driver, name.cstring());
}
#endif


#if !defined __link
int File::copy(const var::ConstString & source_path, const var::ConstString & dest_path){
	File source;
	File dest;
	return copy(source, dest, source_path, dest_path);
}
#else
int File::copy(const var::ConstString & source_path, const var::ConstString & dest_path, link_transport_mdriver_t * driver){
	File source;
	File dest;
	source.set_driver(driver);
	dest.set_driver(driver);
	return copy(source, dest, source_path, dest_path);
}
#endif

#if !defined __link
int File::copy(const var::ConstString & source_path, const var::ConstString & dest_path, bool is_overwrite){
	File source;
	File dest;
	return copy(source, dest, source_path, dest_path, is_overwrite);
}
#else
int File::copy(const var::ConstString & source_path, const var::ConstString & dest_path, bool is_overwrite, link_transport_mdriver_t * driver){
	File source;
	File dest;
	source.set_driver(driver);
	dest.set_driver(driver);
	return copy(source, dest, source_path, dest_path, is_overwrite);
}
#endif



#if !defined __link
int File::rename(const var::ConstString & old_path, const var::ConstString & new_path){
	return ::rename(old_path.cstring(), new_path.cstring());
}
#else
int File::rename(const var::ConstString & old_path, const var::ConstString & new_path, link_transport_mdriver_t * driver){
	if( driver == 0 ){
		return ::rename(old_path.cstring(), new_path.cstring());
	}
	return link_rename(driver, old_path.cstring(), new_path.cstring());
}
#endif


int File::copy(File & source, File & dest, const var::ConstString & source_path, const var::ConstString & dest_path, bool is_overwrite){

#if defined __link
	struct link_stat st;
#else
	struct stat st;
#endif

	if( File::stat(source_path, &st) < 0 ){
		return -1;
	}

	if( source.open(source_path, File::RDONLY) < 0 ){
		return -2;
	}


	if( dest.create(dest_path, is_overwrite, st.st_mode & 0777) < 0 ){
		return -3;
	}

	return dest.write(source, 256);
}


int File::copy(File & source, File & dest, const var::ConstString & source_path, const var::ConstString & dest_path){
	return copy(source, dest, source_path, dest_path, true);
}


#if defined __link
bool File::exists(const var::ConstString & path, link_transport_mdriver_t * driver){
	File f(driver);
#else
bool File::exists(const var::ConstString & path){
	File f;
#endif
	if( f.open_readonly(path) < 0 ){ return false; }
	f.close();
	return true;
}

int File::open(const var::ConstString & name, int access, int perms){
	if( m_fd != -1 ){
		close(); //close first so the fileno can be changed
	}

#if defined __link
	m_fd = link_open(driver(), name.cstring(), access, perms);
#else
	m_fd = ::open(name.cstring(), access, perms);
#endif

	return set_error_number_if_error(m_fd);

}

int File::create(const var::ConstString & name, bool overwrite, int perms){
	int access = LINK_O_RDWR | LINK_O_CREAT;
	if( overwrite ){
		access |= LINK_O_TRUNC;
	} else {
		access |= LINK_O_EXCL;
	}
	return open(name, access, perms);
}

u32 File::size() const {
	u32 loc = seek(0, SEEK_CUR); //get current cursor
	u32 size = seek(0, SEEK_END); //seek to the end
	seek(loc, SEEK_SET); //restore the cursor
	return size;
}

#if defined __link
int File::stat(const var::ConstString & name, struct link_stat * st, link_transport_mdriver_t * driver){
	return link_stat(driver, name.cstring(), st);
}
#else
int File::stat(const var::ConstString & name, struct stat * st){
	return ::stat(name.cstring(), st);
}
#endif

#if defined __link
int File::fstat(struct link_stat * st) const {
	return link_fstat(driver(), m_fd, st);
}
#else
int File::fstat(struct stat * st) const {
	return ::fstat(m_fd, st);
}
#endif

#if !defined __link
u32 File::size(const var::ConstString & name){
	struct stat st;
	if( stat(name.cstring(), &st) < 0 ){
		return (s32)-1;
	}
	return st.st_size;
}
#else
u32 File::size(const var::ConstString & name, link_transport_mdriver_t * driver){
	struct link_stat st;
	if( stat(name.cstring(), &st, driver) < 0 ){
		return (s32)-1;
	}
	return st.st_size;
}

#endif

int File::read(int loc, void * buf, int nbyte) const {
	int result = seek(loc);
	if( result < 0 ){ return result; }
	return read(buf, nbyte);
}

int File::write(int loc, const void * buf, int nbyte) const {
	int result = seek(loc);
	if( result < 0 ){ return result; }
	return write(buf, nbyte);
}

int File::readline(char * buf, int nbyte, int timeout, char term) const {
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
#if !defined __link
			chrono::Timer::wait_milliseconds(1);
#endif
		}
	} while( (bytes_recv < nbyte) && (t < timeout) );

	return bytes_recv;
}

int File::close(){
	int ret = 0;
	if( m_fd >= 0 ){
#if defined __link
		ret = link_close(driver(), m_fd);
#else
		ret = ::close(m_fd);
#endif
		m_fd = -1;
		if( ret < 0 ){
			set_error_number_to_errno();
		}
	}
	return ret;
}

int File::read(void * buf, int nbyte) const {
#if defined __link
	return set_error_number_if_error( link_read(driver(), m_fd, buf, nbyte) );
#else
	return set_error_number_if_error( ::read(m_fd, buf, nbyte) );
#endif
}

int File::write(const void * buf, int nbyte) const {
#if defined __link
	return set_error_number_if_error( link_write(driver(), m_fd, buf, nbyte) );
#else
	return set_error_number_if_error( ::write(m_fd, buf, nbyte) );
#endif
}

int File::seek(int loc, int whence) const {
#if defined __link
	return set_error_number_if_error( link_lseek(driver(), m_fd, loc, whence) );
#else
	return set_error_number_if_error( ::lseek(m_fd, loc, whence) );
#endif
}

int File::fileno() const {
	return m_fd;
}

int File::loc() const {
	return seek(0, CURRENT);
}

int File::flags() const{
#if defined __link
	return -1;
#else
	if( fileno() < 0 ){
		return -1;
	}
	return _global_impure_ptr->procmem_base->open_file[m_fd].flags;
#endif
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

var::String File::gets(char term) const {
	var::String ret;
	gets(ret, term);
	return ret;
}

char * File::gets(var::String & s, char term) const {
	int ret;
	char c;
	s.clear();
	do {
		ret = read(&c, 1);
		if( ret > 0 ){
			s.append(c);
		} else {
			return 0;
		}
	} while(c != term);

	return s.cdata();
}


var::ConstString File::name(const var::ConstString & path){
	int len;
	int i;
	len = path.length();
	for(i = len; i >= 0; i--){
		if( path.at(i) == '/' ){
			return path.cstring() + i + 1;
		}
	}
	return path;
}

var::String File::parent_directory(const var::ConstString & path){
	var::String result;
	result << path;
	u32 pos = result.rfind("/");
	if( pos == var::String::npos ){
		return var::String();
	}
	result.erase(pos);
	return result;
}

#if !defined __link
int File::access(const var::ConstString & path, int o_access){
	return ::access(path.cstring(), o_access);
}
#endif


var::ConstString File::suffix(const var::ConstString & path){
	int len;
	int i;
	len = path.length();
	for(i = len; i >= 0; i--){
		if( path.at(i) == '.' ){
			return path.cstring() + i + 1;
		}
	}
	return 0;
}


int File::ioctl(int req, void * arg) const {
#if defined __link
	return set_error_number_if_error( link_ioctl(driver(), m_fd, req, arg) );
#else
	return set_error_number_if_error( ::ioctl(m_fd, req, arg) );
#endif
}


int File::write(const sys::File & source_file, u32 chunk_size, u32 size) const {
	return write(source_file, chunk_size, size, 0);
}

int File::write(const sys::File & source_file, u32 chunk_size, u32 size, const ProgressCallback * progress_callback) const {
	u32 size_processed = 0;
	u8 buffer[chunk_size];
	int result;
	do {
		if( size - size_processed < chunk_size ){
			chunk_size = size - size_processed;
		}

		result = source_file.read(buffer, chunk_size);
		if( result > 0 ){
			result = write(buffer, result);
			if( result > 0 ){
				size_processed += result;
			}
		}

		if( progress_callback ){
			//abort the transaction
			if( progress_callback->update(size_processed, size) == true ){
				return size_processed;
			}
		}

	} while( (result > 0) && (size > size_processed) );

	//this will terminate the progress operation
	if( progress_callback ){ progress_callback->update(0,0); }

	return size_processed;
}


int DataFile::open(const var::ConstString & name, int flags){
	MCU_UNUSED_ARGUMENT(name);
	MCU_UNUSED_ARGUMENT(flags);
	return 0;
}

int DataFile::read(void * buf, int nbyte) const {
	if( (m_o_flags & File::ACCESS_MODE) == File::WRITEONLY ){
		return -1;
	}

	int size_ready = m_data.size() - m_location;
	if( size_ready > nbyte ){
		size_ready = nbyte;
	}

	memcpy(buf, m_data.to_u8() + m_location, size_ready);
	m_location += size_ready;
	return size_ready;
}

int DataFile::write(const void * buf, int nbyte) const {
	if( (m_o_flags & File::ACCESS_MODE) == File::READONLY ){
		return -1;
	}

	int size_ready = 0;
	if( m_o_flags & File::APPEND ){
		//make room in the m_data object for more bytes
		m_location = m_data.size();
		if( m_data.set_size(m_data.size() + nbyte) < 0 ){
			set_error_number_to_errno();
			return -1;
		}
		size_ready = nbyte;
	} else {
		//limit writes to the current size of the data
		size_ready = m_data.size() - m_location;
		if( size_ready > nbyte ){
			size_ready = nbyte;
		}
	}

	memcpy(m_data.to_u8() + m_location, buf, size_ready);
	m_location += size_ready;
	return size_ready;
}

int DataFile::seek(int loc, int whence) const {
	switch(whence){
		case LINK_SEEK_CUR:
			m_location += loc;
			break;
		case LINK_SEEK_END:
			m_location = m_data.size();
			break;
		case LINK_SEEK_SET:
			m_location = loc;
			break;
	}

	if( m_location > (int)size() ){
		m_location = m_data.size();
	} else if ( m_location < 0 ){
		m_location = 0;
	}

	return m_location;
}

