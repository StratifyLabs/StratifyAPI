//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include <cstdio>
#include <cstring>
#include <fcntl.h>

#include "fs/File.hpp"
#include "sys/Link.hpp"
#include "chrono/Timer.hpp"

using namespace fs;
using namespace arg;

#if !defined __link
#define link_open(w,x,y,z) ::rename(x,y,z)
#define link_rename(x,y,z) ::rename(y,z)
#define link_unlink(x,y) ::remove(y)
#define link_lseek(w,x,y,z) ::lseek(x,y,z)
#define link_write(w,x,y,z) ::write(x,y,z)
#define link_read(w,x,y,z) ::read(x,y,z)
#define link_close(x, y) ::close(y)
#define link_stat(x, y, z) ::stat(y, z)
#define link_fstat(x, y, z) ::fstat(y, z)
#define LINK_DRIVER_ARGUMENT
#define LINK_SET_DRIVER(x,y)

#else
link_transport_mdriver_t * File::m_default_driver = 0;
#define LINK_DRIVER_ARGUMENT link_driver.argument(),
#define LINK_SET_DRIVER(x,y) x.set_driver(y)
#endif

File::File(
		SAPI_LINK_DRIVER
		){
	m_fd = -1; //The file is not open
	set_keep_open(false);
#if defined __link
	m_driver = link_driver.argument();
#endif
}


File::~File(){
	if( is_keep_open() == false ){
		if( fileno() >= 0 ){
			close();
		}
	}
}

int File::open(
		const var::String & path,
		const fs::OpenFlags & flags
		){
	return open(path, flags, Permissions());
}

int File::remove(
		const var::String & path
		SAPI_LINK_DRIVER_LAST
		){
	return link_unlink(
				link_driver.argument(),
				path.cstring()
				);
}

int File::copy(
		SourcePath source_path,
		DestinationPath dest_path
		#if defined __link
		, SourceLinkDriver source_driver,
		DestinationLinkDriver destination_driver
		#endif
		){
	File source;
	File dest;

	LINK_SET_DRIVER(source, LinkDriver(source_driver.argument()));
	LINK_SET_DRIVER(dest, LinkDriver(destination_driver.argument()));

	if( source.open(
			 source_path.argument(),
			 OpenFlags::read_only()
			 ) < 0 ){
		return -1;
	}

	return copy(
				Source(source),
				Destination(dest),
				source_path,
				dest_path
				);
}

int File::copy(
		SourcePath source_path,
		DestinationPath dest_path,
		IsOverwrite is_overwrite
		#if defined __link
		, SourceLinkDriver source_driver,
		DestinationLinkDriver destination_driver
		#endif
		){

	File source;
	File dest;

	LINK_SET_DRIVER(source, LinkDriver(source_driver.argument()));
	LINK_SET_DRIVER(dest, LinkDriver(destination_driver.argument()));

	if( source.open(
			 source_path.argument(),
			 OpenFlags::read_only()) < 0 ){
		return -1;
	}

	return copy(
				Source(source),
				Destination(dest),
				source_path,
				dest_path,
				is_overwrite);
}


int File::rename(SourcePath old_path,
					  DestinationPath new_path
					  SAPI_LINK_DRIVER_LAST
					  ){
#if defined __link
	if( link_driver.argument() == 0 ){
		return ::rename(
					old_path.argument().cstring(),
					new_path.argument().cstring()
					);
	}
#endif
	return link_rename(
				link_driver.argument(),
				old_path.argument().cstring(),
				new_path.argument().cstring()
				);
}

int File::copy(
		Source source,
		Destination dest,
		SourcePath source_path,
		DestinationPath dest_path,
		IsOverwrite is_overwrite
		){

	struct SAPI_LINK_STAT st;

	if( File::stat(
			 source_path.argument(),
			 st
			 ) < 0 ){
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
			 dest_path.argument(),
			 is_overwrite,
			 Permissions(st.st_mode & 0777)
			 ) < 0 ){
		return -3;
	}

	return dest.argument().write(
				source.argument(),
				PageSize(256)
				);
}


int File::copy(
		Source source,
		Destination dest,
		SourcePath source_path,
		DestinationPath dest_path){
	return copy(
				source,
				dest,
				source_path,
				dest_path,
				IsOverwrite(true)
				);
}


bool File::exists(
		const var::String & path
		SAPI_LINK_DRIVER_LAST
		){

	File f;

	LINK_SET_DRIVER(f, link_driver);

	if( f.open(
			 path,
			 fs::OpenFlags::read_only()
			 ) < 0 ){ return false; }
	f.close();
	return true;
}

Stat File::get_info(
		const var::String & path
		SAPI_LINK_DRIVER_LAST
		){
	struct SAPI_LINK_STAT stat;
	memset(&stat, 0, sizeof(stat));
	File::stat(
				path,
				stat
			#if defined __link
				, link_driver
			#endif
				);
	return Stat(stat);
}

int File::open(
		const var::String & path,
		const fs::OpenFlags & flags,
		const Permissions & permissions
		){
	if( m_fd != -1 ){
		close(); //close first so the fileno can be changed
	}

	return set_error_number_if_error(
				link_open(
					driver(),
					path.cstring(),
					flags.o_flags(),
					permissions.permissions()
					)
				);
}

int File::create(
		const var::String & path,
		IsOverwrite is_overwrite,
		const Permissions & perms
		){
	fs::OpenFlags flags = fs::OpenFlags::create();
	if( is_overwrite.argument() ){
		flags.set_truncate();
	} else {
		flags.set_exclusive();
	}
	return open(
				path,
				flags,
				perms);
}

u32 File::size() const {
	u32 loc = seek(Location(0), CURRENT); //get current cursor
	u32 size = seek(Location(0), END); //seek to the end
	seek(Location(loc), SET); //restore the cursor
	return size;
}

int File::stat(
		const var::String & name,
		struct SAPI_LINK_STAT & st
		SAPI_LINK_DRIVER_LAST
		){
	return link_stat(
				link_driver.argument(),
				name.cstring(),
				&st
				);
}


int File::fstat(
		struct SAPI_LINK_STAT * st
		) const {
	return link_fstat(
				driver(),
				m_fd,
				st);
}

#if !defined __link
u32 File::size(
		const arg::SourceFilePath & name
		){
	struct stat st;
	if( stat(name, st) < 0 ){
		return (s32)-1;
	}
	return st.st_size;
}
#else
u32 File::size(
		const var::String & name
		SAPI_LINK_DRIVER_LAST
		){
	struct SAPI_LINK_STAT st;
	if( stat(
			 name,
			 st
		 #if defined __link
			 , link_driver
		 #endif
			 ) < 0 ){
		return (s32)-1;
	}
	return st.st_size;
}

#endif

int File::read(
		Location location,
		void * buf,
		Size size) const {
	int result = seek(location);
	if( result < 0 ){ return result; }
	return read(buf, size);
}

int File::write(Location location,
		const void * buf,
		Size size
		) const {
	int result = seek(location);
	if( result < 0 ){ return result; }
	return write(
				buf,
				size
				);
}

int File::readline(
		char * buf,
		int nbyte,
		int timeout,
		char term
		) const {
	int t;
	int bytes_recv;
	char c;
	t = 0;
	bytes_recv = 0;
	do {
		if( read(
				 &c,
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
			chrono::wait(chrono::Milliseconds(1));
#endif
		}
	} while( (bytes_recv < nbyte) && (t < timeout) );

	return bytes_recv;
}

int File::close(){
	int ret = 0;
	if( m_fd >= 0 ){
		ret = link_close(driver(), m_fd);
		m_fd = -1;
		if( ret < 0 ){
			set_error_number_to_errno();
		}
	}
	return ret;
}

int File::read(void * buf, Size size) const {
	return set_error_number_if_error(
				link_read(
					driver(),
					m_fd,
					buf,
					size.argument())
				);
}

int File::write(
		const void * buf,
		Size size
		) const {
	return set_error_number_if_error(
				link_write(
					driver(),
					m_fd,
					buf,
					size.argument())
				);
}

int File::seek(
		Location location,
		enum whence whence) const {
	return set_error_number_if_error(
				link_lseek(
					driver(),
					m_fd,
					location.argument(),
					whence)
				);

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
					buffer,
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

const char * File::gets(var::String & s, char term) const {
	int ret;
	char c;
	s.clear();
	do {
		ret = read(
					&c,
					Size(1)
					);
		if( ret > 0 ){
			s.append(c);
		} else {
			return 0;
		}
	} while(c != term);

	return s.cstring();
}


const var::String File::name(
		const var::String & path
		){
	size_t pos = path.rfind('/');
	if( pos == var::String::npos ){
		return path;
	}

	return path.cstring() + pos + 1;
}

var::String File::parent_directory(
		const var::String & path
		){
	var::String result(path);
	size_t pos = result.rfind('/');
	if( pos == var::String::npos ){
		return var::String();
	}
	result.erase(var::String::Position(pos));
	return result;
}

#if !defined __link
int File::access(const SourceFilePath path,
					  const Access & access
					  ){
	return ::access(
				path.argument().cstring(),
				access.o_access()
				);

}
#endif


const char * File::suffix(
		const var::String & path
		){
	size_t pos = path.rfind('.');
	if( pos != var::String::npos ){
		return path.cstring() + pos + 1;
	}
	return nullptr;
}


int File::ioctl(
		IoRequest request,
		IoArgument argument
		) const {
	return set_error_number_if_error(
			#if defined __link
				link_ioctl(
					driver(),
				#else
				::ioctl(
				#endif
					m_fd,
					request.argument(),
					argument.argument()) );
}


int File::write(
		const File & source_file,
		PageSize page_size,
		Size size
		) const {
	return write(source_file, page_size, size, nullptr);
}


int File::write(
		const File & source_file,
		PageSize page_size,
		Size size,
		const sys::ProgressCallback * progress_callback
		) const {
	u32 size_processed = 0;
	u32 page_size_value = page_size.argument();

	var::Data buffer(page_size_value);

	if( buffer.size() == 0 ){
		return set_error_number_if_error(-1);
	}

	int result;
	do {
		if( size.argument() - size_processed < page_size_value ){
			page_size_value = size.argument() - size_processed;
		}

		buffer.resize(page_size_value);
		if( buffer.size()	!= page_size_value ){
			return set_error_number_if_error(-1);
		}

		result = source_file.read(buffer);
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
	if( (result < 0) && (size_processed == 0) ){
		return result;
	}
	return set_error_number_if_error(size_processed);
}


int DataFile::read(void * buf,
						 Size nbyte
						 ) const {

	if( flags().is_write_only() ){
		return set_error_number_if_error(-1);
	}

	int size_ready = m_data.size() - m_location;
	if( size_ready > (int)nbyte.argument() ){
		size_ready = nbyte.argument();
	}

	if( size_ready < 0 ){
		return set_error_number_if_error(-1);
	}

	var::Reference::memory_copy(
				SourceBuffer(var::Reference(m_data).to_const_u8() + m_location),
				DestinationBuffer(buf),
				Size(size_ready)
				);

	m_location += size_ready;
	return set_error_number_if_error(size_ready);
}

int DataFile::write(const void * buf,
						  Size nbyte
						  ) const {

	if( flags().is_read_only() ){
		return set_error_number_if_error(-1);
	}

	int size_ready = 0;
	if( flags().is_append() ){
		//make room in the m_data object for more bytes
		m_location = m_data.size();
		if( m_data.resize(m_data.size() + nbyte.argument()) < 0 ){
			set_error_number_to_errno();
			return set_error_number_if_error(-1);
		}
		size_ready = nbyte.argument();
	} else {
		//limit writes to the current size of the data
		size_ready = m_data.size() - m_location;
		if( size_ready > (int)nbyte.argument() ){
			size_ready = nbyte.argument();
		}
	}

	if( size_ready < 0 ){
		return set_error_number_if_error(-1);
	}

	var::Reference::memory_copy(
				SourceBuffer(buf),
				DestinationBuffer(var::Reference(data()).to_u8() + m_location),
				Size(size_ready)
				);

	m_location += size_ready;
	return set_error_number_if_error(size_ready);
}

int DataFile::seek(
		Location location,
		enum whence whence) const {
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

int ReferenceFile::read(void * buf,
								Size nbyte
								) const {

	if( flags().is_write_only() ){
		return set_error_number_if_error(-1);
	}

	int size_ready = reference().size() - m_location;
	if( size_ready > (int)nbyte.argument() ){
		size_ready = nbyte.argument();
	}

	if( size_ready < 0 ){
		return set_error_number_if_error(-1);
	}

	var::Reference::memory_copy(
				SourceBuffer(reference().to_const_u8() + m_location),
				DestinationBuffer(buf),
				Size(size_ready)
				);

	m_location += size_ready;
	return set_error_number_if_error(size_ready);
}

int ReferenceFile::write(
		const void * buf,
		Size nbyte
		) const {

	if( flags().is_read_only() ){
		return set_error_number_if_error(-1);
	}

	if( reference().is_read_only() ){
		return set_error_number_if_error(-1);
	}

	int size_ready = 0;
	if( flags().is_append() ){
		return set_error_number_if_error(-1);
	} else {
		//limit writes to the current size of the data
		size_ready = reference().size() - m_location;
		if( size_ready > (int)nbyte.argument() ){
			size_ready = nbyte.argument();
		}
	}

	if( size_ready < 0 ){
		return set_error_number_if_error(-1);
	}

	var::Reference::memory_copy(
				SourceBuffer(buf),
				DestinationBuffer(reference().to_u8() + m_location),
				Size(size_ready)
				);
	m_location += size_ready;
	return set_error_number_if_error(size_ready);
}

int ReferenceFile::seek(
		Location location,
		enum whence whence
		) const {
	switch(whence){
		case CURRENT:
			m_location += location.argument();
			break;
		case END:
			m_location = reference().size();
			break;
		case SET:
			m_location = location.argument();
			break;
	}

	if( m_location > (int)size() ){
		m_location = reference().size();
	} else if ( m_location < 0 ){
		m_location = 0;
	}

	return m_location;
}

int NullFile::open(
		const var::String & name,
		const OpenFlags & flags
		){
	MCU_UNUSED_ARGUMENT(name);
	MCU_UNUSED_ARGUMENT(flags);
	return 0;
}

int NullFile::read(
		void * buf,
		Size nbyte
		) const {
	MCU_UNUSED_ARGUMENT(buf);
	MCU_UNUSED_ARGUMENT(nbyte);
	return -1; //end of file
}

int NullFile::write(
		const void * buf,
		Size nbyte
		) const {
	MCU_UNUSED_ARGUMENT(buf);
	return nbyte.argument();
}

int NullFile::seek(
		Location location,
		enum whence whence
		) const {
	MCU_UNUSED_ARGUMENT(location);
	MCU_UNUSED_ARGUMENT(whence);
	return -1;

}

