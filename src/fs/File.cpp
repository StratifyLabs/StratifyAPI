/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc


#include <cstdio>
#include <cstring>
#include <fcntl.h>

#include "fs/File.hpp"
#include "sys/Link.hpp"
#include "chrono/Timer.hpp"

using namespace fs;
using namespace arg;

#if !defined __link
#define link_open(w,x,y,z) ::open(x,y,z)
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
	if( link_unlink(
				link_driver.argument(),
				path.cstring()
				) < 0 ){
		return api::error_code_fs_failed_to_unlink;
	}
	return 0;
}

int File::copy(
		SourcePath source_path,
		DestinationPath dest_path,
		const sys::ProgressCallback * progress_callback
		#if defined __link
		, SourceLinkDriver source_driver,
		DestinationLinkDriver destination_driver
		#endif
		){
	File source;
	File dest;

	LINK_SET_DRIVER(source, source_driver.argument());
	LINK_SET_DRIVER(dest, destination_driver.argument());

	if( source.open(
				source_path.argument(),
				OpenFlags::read_only()
				) < 0 ){
		return api::error_code_fs_failed_to_open;
	}

	return copy(
				Source(source),
				Destination(dest),
				source_path,
				dest_path,
				progress_callback
				);
}

int File::copy(
		SourcePath source_path,
		DestinationPath dest_path,
		IsOverwrite is_overwrite,
		const sys::ProgressCallback * progress_callback
		#if defined __link
		, SourceLinkDriver source_driver,
		DestinationLinkDriver destination_driver
		#endif
		){

	File source;
	File dest;

	LINK_SET_DRIVER(source, source_driver.argument());
	LINK_SET_DRIVER(dest, destination_driver.argument());

	if( source.open(
				source_path.argument(),
				OpenFlags::read_only()
				) < 0 ){
		return api::error_code_fs_failed_to_open;
	}

	return copy(
				Source(source),
				Destination(dest),
				source_path,
				dest_path,
				is_overwrite,
				progress_callback);
}

int File::save_copy(
		const var::String& file_path,
		IsOverwrite is_overwrite
		) const {
	File copy_file;
	if(
		 set_error_number_if_error(
			 copy_file.create(file_path, IsOverwrite(is_overwrite))
			 ) < 0 ){
		return return_value();
	}
	return copy_file.write(*this);
}

int File::touch(const var::String& path){
	File touch_file;
	if( touch_file.open(path, OpenFlags::read_write()) < 0 ){
		return api::error_code_fs_failed_to_open;
	} else {
		char c;
		touch_file.read(var::Reference(c));
		touch_file.write(File::Location(0), var::Reference(c));
	}
	return 0;
}


int File::rename(SourcePath old_path,
								 DestinationPath new_path
								 SAPI_LINK_DRIVER_LAST
								 ){
#if defined __link
	if( link_driver.argument() == nullptr ){
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
		IsOverwrite is_overwrite,
		const sys::ProgressCallback * progress_callback
		){

	MCU_UNUSED_ARGUMENT(source_path);
	struct SAPI_LINK_STAT st;

	if( source.argument().fstat(&st) < 0 ){
		return api::error_code_fs_failed_to_stat;
	}

	if( (st.st_mode & 0666) == 0 ){
		st.st_mode = 0666;
	}

	if( dest.argument().create(
				dest_path.argument(),
				is_overwrite,
				Permissions(st.st_mode & 0777)
				) < 0 ){
		return api::error_code_fs_failed_to_create;
	}

	int result = dest.argument().write(
				source.argument(),
				PageSize(SAPI_LINK_DEFAULT_PAGE_SIZE),
				Size(size_t(-1)),
				progress_callback
				);
	if( result < 0 ){
		return api::error_code_fs_failed_to_write;
	}
	return result;
}


int File::copy(
		Source source,
		Destination dest,
		SourcePath source_path,
		DestinationPath dest_path,
		const sys::ProgressCallback * progress_callback
		){
	return copy(
				source,
				dest,
				source_path,
				dest_path,
				IsOverwrite(true),
				progress_callback
				);
}


bool File::exists(
		const var::String & path
		SAPI_LINK_DRIVER_LAST
		){

	File f;

	LINK_SET_DRIVER(f, link_driver.argument());

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

Stat File::get_info(
		Descriptor fd
		SAPI_LINK_DRIVER_LAST
		){
	struct SAPI_LINK_STAT stat;
	memset(&stat, 0, sizeof(stat));
	link_fstat(
				link_driver.argument(),
				fd.argument(),
				&stat
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
				m_fd =
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
				perms
				);
}

u32 File::size() const {
	int loc = seek(Location(0), whence_current); //get current cursor
	u32 size = static_cast<u32>(seek(Location(0), whence_end)); //seek to the end
	seek(Location(loc), whence_set); //restore the cursor
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
		const var::String & name
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
		return static_cast<u32>(-1);
	}
	return st.st_size;
}

#endif

int File::read(
		Location location,
		void * buf,
		Size size) const {
	int result = seek(location, whence_set);
	if( result < 0 ){ return result; }
	return read(buf, size);
}

int File::write(Location location,
								const void * buf,
								Size size
								) const {
	int result = seek(location, whence_set);
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
		ret = set_error_number_if_error(
					link_close(driver(), m_fd)
					);
		m_fd = -1;
	}
	return ret;
}

int File::read(void * buf, Size size) const {
	return set_error_number_if_error(
				link_read(
					driver(),
					m_fd,
					buf,
					static_cast<int>(size.argument()))
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
					static_cast<int>(size.argument())
					)
				);
}

int File::seek(
		int location,
		enum whence whence) const {
	return set_error_number_if_error(
				link_lseek(
					driver(),
					m_fd,
					location,
					whence)
				);

}

int File::fileno() const {
	return m_fd;
}

int File::location() const {
	return seek(Location(0), whence_current);
}

int File::flags() const{
#if defined __link
	return set_error_number_if_error(api::error_code_fs_unsupported_operation);
#else
	if( fileno() < 0 ){
		return set_error_number_if_error(
					api::error_code_fs_bad_descriptor
					);
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
		return nullptr;
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
		return nullptr;
	}

	if( ret > 0 ){
		seek(Location(i - ret + 1), whence_current);
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
			return nullptr;
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

	return path.create_sub_string(
				var::String::Position(pos+1)
				);
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
int File::access(const var::String & path,
								 const Access & access
								 ){
	return ::access(
				path.cstring(),
				access.o_access()
				);

}
#endif


const var::String File::suffix(
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
	u32 file_size = size.argument();
	if( file_size == static_cast<u32>(-1) ){
		file_size = source_file.size();
	}

	u32 page_size_value = page_size.argument();
	if(page_size_value == 0 ){
		page_size_value = SAPI_LINK_DEFAULT_PAGE_SIZE;
	}

	var::Data buffer(page_size_value);

	if( buffer.size() == 0 ){
		return set_error_number_if_error(-1);
	}

	if( file_size == 0 ){
		if( progress_callback ){
			progress_callback->update(0,100);
			progress_callback->update(100,100);
			progress_callback->update(0,0);
		}
		return 0;
	}

	int result;
	do {
		if( file_size - size_processed < page_size_value ){
			page_size_value = file_size - size_processed;
		}

		buffer.resize(page_size_value);
		if( buffer.size()	!= page_size_value ){
			return set_error_number_if_error(-1);
		}

		result = source_file.read(buffer);
		if( result > 0 ){
			result = write(
						buffer.to_const_void(),
						Size(static_cast<size_t>(result))
						);
			if( result > 0 ){
				size_processed += static_cast<u32>(result);
			} else if( result < 0 ){
				return set_error_number_if_error(result);
			}
		}

		if( progress_callback ){
			//abort the transaction
			if( progress_callback->update(
						static_cast<int>(size_processed),
						static_cast<int>(file_size)
						) == true ){
				progress_callback->update(0,0);
				return static_cast<int>(size_processed);
			}
		}

	} while( (result > 0) && (file_size > size_processed) );

	//this will terminate the progress operation
	if( progress_callback ){ progress_callback->update(0,0); }
	if( (result < 0) && (size_processed == 0) ){
		return set_error_number_if_error(result);
	}
	return set_error_number_if_error(static_cast<int>(size_processed));
}

DataFile::DataFile(
		fs::File::Path file_path
		){
	//read the contents of file_path into this object
	FileInfo info = File::get_info(file_path.argument());
	m_open_flags = OpenFlags::append_write_only();
	m_fd = 0;
	m_location = 0;
	if( info.is_valid() && info.is_file() ){
		File f;
		if( f.open(
					file_path.argument(),
					OpenFlags::read_only()
					) >= 0 ){
			write(f);
			f.close();
		}
	}
	m_location = 0;
	m_open_flags = OpenFlags::read_write();
}

DataFile::DataFile(
		const fs::File & file_to_load
		){
	m_fd = 0;
	m_location = 0;
	m_open_flags = OpenFlags::append_read_write();
	write(file_to_load);
	seek(0);
	m_open_flags = OpenFlags::read_write();
}


int DataFile::read(
		void * buf,
		Size nbyte
		) const {

	if( flags().is_write_only() ){
		return set_error_number_if_error(api::error_code_fs_cant_read);
	}

	int size_ready = static_cast<int>(m_data.size()) - m_location;
	if( size_ready > static_cast<int>(nbyte.argument()) ){
		size_ready = static_cast<int>(nbyte.argument());
	}

	if( size_ready < 0 ){
		//EOF
		return set_error_number_if_error(-1);
	}

	var::Reference::memory_copy(
				SourceBuffer(m_data.to_const_u8() + m_location),
				DestinationBuffer(buf),
				Size(static_cast<size_t>(size_ready))
				);

	m_location += size_ready;
	return set_error_number_if_error(size_ready);
}

int DataFile::write(
		const void * buf,
		Size nbyte
		) const {

	if( flags().is_read_only() ){
		return set_error_number_if_error(api::error_code_fs_cant_write);
	}

	u32 size_ready = 0;
	if( flags().is_append() ){
		//make room in the m_data object for more bytes
		m_location = static_cast<int>(m_data.size());
		if( m_data.resize(m_data.size() + nbyte.argument()) < 0 ){
			set_error_number_to_errno();
			return set_error_number_if_error(-1);
		}
		size_ready = nbyte.argument();
	} else {
		//limit writes to the current size of the data
		if( static_cast<int>(m_data.size()) > m_location ){
			size_ready = m_data.size() - static_cast<u32>(m_location);
			if( size_ready > nbyte.argument() ){
				size_ready = nbyte.argument();
			}
		} else {
			return set_error_number_if_error(-1);
		}
	}


	var::Reference::memory_copy(
				SourceBuffer(buf),
				DestinationBuffer(data().to_u8() + m_location),
				Size(size_ready)
				);

	m_location += size_ready;
	return set_error_number_if_error(static_cast<int>(size_ready));
}

int DataFile::seek(
		int location,
		enum whence whence
		) const {
	switch(whence){
		case whence_current:
			m_location += location;
			break;
		case whence_end:
			m_location = static_cast<int>(m_data.size());
			break;
		case whence_set:
			m_location = location;
			break;
	}

	if( m_location > static_cast<int>(size()) ){
		m_location = static_cast<int>(m_data.size());
	} else if ( m_location < 0 ){
		m_location = 0;
	}

	return m_location;
}

int ReferenceFile::read(void * buf,
												Size nbyte
												) const {

	if( flags().is_write_only() ){
		return set_error_number_if_error(api::error_code_fs_cant_read);
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
		return set_error_number_if_error(api::error_code_fs_cant_write);
	}

	if( reference().is_read_only() ){
		return set_error_number_if_error(api::error_code_fs_cant_write_read_only);
	}

	int size_ready = 0;
	if( flags().is_append() ){
		return set_error_number_if_error(
					api::error_code_fs_cant_write_append_only
					);
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

int ReferenceFile::seek(int location,
												enum whence whence
												) const {
	switch(whence){
		case whence_current:
			m_location += location;
			break;
		case whence_end:
			m_location = reference().size();
			break;
		case whence_set:
			m_location = location;
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

int NullFile::seek(int location,
									 enum whence whence
									 ) const {
	MCU_UNUSED_ARGUMENT(location);
	MCU_UNUSED_ARGUMENT(whence);
	return -1;

}

