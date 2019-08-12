//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include <cstdio>
#include <cstring>
#include <fcntl.h>

#include "fs/File.hpp"
#include "chrono/Timer.hpp"
using namespace fs;
using namespace arg;

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

int File::open(const arg::FilePath & path, const fs::OpenFlags & flags){
	return open(path, flags, Permissions());
}

#if !defined __link
int File::remove(
		const arg::SourceFilePath path
		){
	return ::remove(path.argument().cstring());
}
#else
int File::remove(
		const arg::SourceFilePath path,
		link_transport_mdriver_t * driver
		){
	return link_unlink(driver, path.argument().cstring());
}
#endif


#if !defined __link
int File::copy(const SourceFilePath source_path,
		const DestinationFilePath dest_path
		){
	File source;
	File dest;

	if( source.open(
			 arg::FilePath(source_path.argument()),
			 OpenFlags::read_only()
			 ) < 0 ){
		return -1;
	}

	return copy(
				SourceFile(source),
				DestinationFile(dest),
				source_path,
				dest_path
				);
}
#else
int File::copy(
		const arg::SourceFilePath source_path,
		const arg::DestinationFilePath dest_path,
		link_transport_mdriver_t * driver){
	File source;
	File dest;
	source.set_driver(driver);
	dest.set_driver(driver);

	if( source.open(
			 arg::FilePath(source_path.argument()),
			 OpenFlags::read_only()
			 ) < 0 ){
		return -1;
	}

	return copy(
				arg::SourceFile(source),
				arg::DestinationFile(dest),
				source_path,
				dest_path
				);
}
#endif

#if !defined __link
int File::copy(const SourceFilePath source_path,
		const DestinationFilePath dest_path,
		const IsOverwrite is_overwrite){
	File source;
	File dest;

	if( source.open(
			 arg::FilePath(source_path.argument()),
			 OpenFlags::read_only()
			 ) < 0 ){
		return -1;
	}

	return copy(
				SourceFile(source),
				DestinationFile(dest),
				source_path,
				dest_path,
				is_overwrite);
}
#else
int File::copy(
		const arg::SourceFilePath source_path,
		const arg::DestinationFilePath dest_path,
		const arg::IsOverwrite is_overwrite,
		link_transport_mdriver_t * driver
		){
	File source;
	File dest;
	source.set_driver(driver);
	dest.set_driver(driver);

	if( source.open(
			 arg::FilePath(source_path.argument()),
			 OpenFlags::read_only()) < 0 ){
		return -1;
	}

	return copy(
				SourceFile(source),
				DestinationFile(dest),
				source_path,
				dest_path,
				is_overwrite);
}
#endif



#if !defined __link
int File::rename(const arg::SourceFilePath old_path, const arg::DestinationFilePath new_path){
	return ::rename(old_path.argument().cstring(), new_path.argument().cstring());
}
#else
int File::rename(
		const arg::SourceFilePath old_path,
		const arg::DestinationFilePath new_path,
		link_transport_mdriver_t * driver
		){
	if( driver == 0 ){
		return ::rename(old_path.argument().cstring(), new_path.argument().cstring());
	}
	return link_rename(driver, old_path.argument().cstring(), new_path.argument().cstring());
}
#endif


int File::copy(
		const arg::SourceFile source,
		const arg::DestinationFile dest,
		const arg::SourceFilePath source_path,
		const arg::DestinationFilePath dest_path,
		const IsOverwrite is_overwrite){

#if defined __link
	struct link_stat st;
#else
	struct stat st;
#endif

	if( File::stat(arg::SourceFilePath(source_path), &st) < 0 ){
		return -1;
	}

#if 0
	if( source.argument().open(
			 source_path.argument(),
			 fs::OpenFlags::read_only()
			 ) < 0 ){
		return -2;
	}
#endif


	if( dest.argument().create(
			 dest_path,
			 is_overwrite,
			 Permissions(st.st_mode & 0777)
			 ) < 0 ){
		return -3;
	}

	return dest.argument().write(
				source,
				PageSize(256)
				);
}


int File::copy(
		const arg::SourceFile source,
		const arg::DestinationFile dest,
		const arg::SourceFilePath source_path,
		const arg::DestinationFilePath dest_path){
	return copy(source, dest, source_path, dest_path, arg::IsOverwrite(true));
}


#if defined __link
bool File::exists(
		const arg::SourceFilePath path,
		link_transport_mdriver_t * driver
		){
	File f(driver);
#else
bool File::exists(
		const SourceFilePath path
		){
	File f;
#endif
	if( f.open(
			 arg::FilePath(path.argument()),
			 fs::OpenFlags::read_only()) < 0 ){ return false; }
	f.close();
	return true;
}

#if defined __link
Stat File::get_info(
		const arg::SourceFilePath path,
		link_transport_mdriver_t * driver
		){
#else
Stat File::get_info(
		const SourceFilePath path
		){
#endif
#if defined __link
	struct link_stat stat;
	memset(&stat, 0, sizeof(stat));
	File::stat(path, &stat, driver);
#else
	struct stat stat;
	memset(&stat, 0, sizeof(stat));
	File::stat(path, &stat);
#endif

	return Stat(stat);
}

#if defined __link
Stat File::get_info(
		const arg::FileDescriptor fd,
		link_transport_mdriver_t * driver
		){
#else
Stat File::get_info(const FileDescriptor fd){
#endif
#if defined __link
	struct link_stat stat;
	memset(&stat, 0, sizeof(stat));
	::link_fstat(driver, fd.argument(), &stat);
#else
	struct stat stat;
	memset(&stat, 0, sizeof(stat));
	::fstat(fd.argument(), &stat);
#endif

	return Stat(stat);
}

int File::open(const arg::FilePath & name, const fs::OpenFlags & flags, const Permissions & permissions){
	if( m_fd != -1 ){
		close(); //close first so the fileno can be changed
	}

#if defined __link
	m_fd = link_open(driver(), name.argument().cstring(), flags.o_flags(), permissions.permissions());
#else
	m_fd = ::open(name.argument().cstring(), flags.o_flags(), permissions.permissions());
#endif

	return set_error_number_if_error(m_fd);

}

int File::create(
		const arg::DestinationFilePath & name,
		const IsOverwrite & is_overwrite,
		const Permissions & perms
		){
	fs::OpenFlags flags = fs::OpenFlags::create();
	if( is_overwrite.argument() ){
		flags.set_truncate();
	} else {
		flags.set_exclusive();
	}
	return open(
				arg::FilePath(name.argument()),
				flags,
				perms);
}

u32 File::size() const {
	u32 loc = seek(Location(0), CURRENT); //get current cursor
	u32 size = seek(Location(0), END); //seek to the end
	seek(Location(loc), SET); //restore the cursor
	return size;
}

#if defined __link
int File::stat(
		const arg::SourceFilePath & name,
		struct link_stat * st,
		link_transport_mdriver_t * driver
		){
	return link_stat(driver, name.argument().cstring(), st);
}
#else
int File::stat(
		const arg::SourceFilePath & name,
		struct stat * st
		){
	return ::stat(name.argument().cstring(), st);
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
u32 File::size(
		const arg::SourceFilePath & name
		){
	struct stat st;
	if( stat(name, &st) < 0 ){
		return (s32)-1;
	}
	return st.st_size;
}
#else
u32 File::size(
		const arg::SourceFilePath & name,
		link_transport_mdriver_t * driver
		){
	struct link_stat st;
	if( stat(name, &st, driver) < 0 ){
		return (s32)-1;
	}
	return st.st_size;
}

#endif

int File::read(const Location location, const DestinationBuffer buf, const Size size) const {
	int result = seek(location);
	if( result < 0 ){ return result; }
	return read(buf, size);
}

int File::write(const Location location, const SourceBuffer buf, const Size size) const {
	int result = seek(location);
	if( result < 0 ){ return result; }
	return write(buf, size);
}

int File::readline(char * buf, int nbyte, int timeout, char term) const {
	int t;
	int bytes_recv;
	char c;
	t = 0;
	bytes_recv = 0;
	do {
		if( read(
				 DestinationBuffer(&c),
				 Size(1)
				 ) == 1 ){
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

int File::read(const DestinationBuffer buf, const Size size) const {
#if defined __link
	return set_error_number_if_error( link_read(driver(), m_fd, buf.argument(), size.argument()) );
#else
	return set_error_number_if_error( ::read(m_fd, buf.argument(), size.argument()) );
#endif
}

int File::write(const SourceBuffer buf, const Size size) const {
#if defined __link
	return set_error_number_if_error( link_write(driver(), m_fd, buf.argument(), size.argument()) );
#else
	return set_error_number_if_error( ::write(m_fd, buf.argument(), size.argument() ) );
#endif
}

int File::seek(const Location location, enum whence whence) const {
#if defined __link
	return set_error_number_if_error( link_lseek(driver(), m_fd, location.argument(), whence) );
#else
	return set_error_number_if_error( ::lseek(m_fd, location.argument(), whence) );
#endif
}

int File::fileno() const {
	return m_fd;
}

int File::location() const {
	return seek(Location(0), CURRENT);
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
		ret = read(
					DestinationBuffer(buffer),
					Size(GETS_BUFFER_SIZE)
					);
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
		seek(Location(i - ret + 1), CURRENT);
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
		ret = read(
					DestinationBuffer(&c),
					Size(1)
					);
		if( ret > 0 ){
			s.append(c);
		} else {
			return 0;
		}
	} while(c != term);

	return s.cdata();
}


var::ConstString File::name(
		const ImplicitFilePath & path
		){
	int len;
	int i;
	len = path.argument().length();
	for(i = len; i >= 0; i--){
		if( path.argument().at(i) == '/' ){
			return path.argument().cstring() + i + 1;
		}
	}
	return path.argument();
}

var::String File::parent_directory(
		const arg::ImplicitFilePath & path
		){
	var::String result;
	result << path.argument();
	u32 pos = result.rfind(arg::StringToFind("/"));
	if( pos == var::String::npos ){
		return var::String();
	}
	result.erase(arg::Position(pos));
	return result;
}

#if !defined __link
int File::access(const SourceFilePath path,
		const Access & access
		){
	return ::access(path.argument().cstring(), access.o_access());
}
#endif


var::ConstString File::suffix(
		const arg::ImplicitFilePath & path
		){
	int len;
	int i;
	len = path.argument().length();
	for(i = len; i >= 0; i--){
		if( path.argument().at(i) == '.' ){
			return path.argument().cstring() + i + 1;
		}
	}
	return 0;
}


int File::ioctl(const IoRequest request,
		const IoArgument argument
		) const {
#if defined __link
	return set_error_number_if_error(
				link_ioctl(
					driver(),
					m_fd,
					request.argument(),
					argument.argument()) );
#else
	return set_error_number_if_error(
				::ioctl(
					m_fd,
					request.argument(),
					argument.argument()) );
#endif
}


int File::write(const SourceFile source_file,
		const PageSize page_size,
		const Size size
		) const {
	return write(source_file, page_size, size, 0);
}


int File::write(
		const arg::SourceFile & source_file,
		const PageSize & page_size,
		const Size & size,
		const sys::ProgressCallback * progress_callback
		) const {
	u32 size_processed = 0;
	u32 page_size_value = page_size.argument();

	var::Data buffer = var::Data( arg::Size(page_size_value) );

	if( buffer.size() == 0 ){
		return -1;
	}

	int result;
	do {
		if( size.argument() - size_processed < page_size_value ){
			page_size_value = size.argument() - size_processed;
		}

		buffer.set_size(page_size_value);
		if( buffer.size()	!= page_size_value ){
			return -1;
		}

		result = source_file.argument().read(buffer);
		if( result > 0 ){
			result = write(buffer);
			if( result > 0 ){
				size_processed += result;
			}
		}

		if( progress_callback ){
			//abort the transaction
			if( progress_callback->update(size_processed, size.argument()) == true ){
				progress_callback->update(0,0);
				return size_processed;
			}
		}

	} while( (result > 0) && (size.argument() > size_processed) );

	//this will terminate the progress operation
	if( progress_callback ){ progress_callback->update(0,0); }

	return size_processed;
}


int DataFile::open(const arg::FilePath & name, const OpenFlags & flags){
	MCU_UNUSED_ARGUMENT(name);
	MCU_UNUSED_ARGUMENT(flags);
	return 0;
}

int DataFile::read(void * buf, int nbyte) const {

	if( flags().is_write_only() ){
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

	if( flags().is_read_only() ){
		return -1;
	}

	int size_ready = 0;
	if( flags().is_append() ){
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

int DataFile::seek(const arg::Location & location, enum whence whence) const {
	switch(whence){
		case CURRENT:
			m_location += location.argument();
			break;
		case END:
			m_location = m_data.size();
			break;
		case SET:
			m_location = location.argument();
			break;
	}

	if( m_location > (int)size() ){
		m_location = m_data.size();
	} else if ( m_location < 0 ){
		m_location = 0;
	}

	return m_location;
}

int NullFile::open(const var::ConstString & name, const OpenFlags & flags){
	MCU_UNUSED_ARGUMENT(name);
	MCU_UNUSED_ARGUMENT(flags);
	return 0;
}

int NullFile::read(void * buf, int nbyte) const {
	MCU_UNUSED_ARGUMENT(buf);
	MCU_UNUSED_ARGUMENT(nbyte);
	return -1;
}

int NullFile::write(const void * buf, int nbyte) const {
	MCU_UNUSED_ARGUMENT(buf);
	return nbyte;
}

int NullFile::seek(const arg::Location & location, enum whence whence) const {
	MCU_UNUSED_ARGUMENT(location);
	MCU_UNUSED_ARGUMENT(whence);
	return -1;

}

