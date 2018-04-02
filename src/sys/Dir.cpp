//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "var/String.hpp"
#include "sys/File.hpp"
#include "sys/FileInfo.hpp"
#include "sys/Dir.hpp"
using namespace sys;



#if defined __link
#define errno link_errno
Dir::Dir(link_transport_mdriver_t * driver){
	m_dirp = 0;
	m_driver = driver;
}

#else
Dir::Dir(){
	m_dirp = 0;
}

#endif

Dir::~Dir(){
	close();
}

#if !defined __link
int Dir::remove(const char * path, bool recursive){
	int ret = 0;
	if( recursive ){
		Dir d;
		if( d.open(path) == 0 ){
			var::String entry;
			while( d.get_entry(entry) && (ret >= 0) ){
				FileInfo info;
				info.get_info(entry);
				if( info.is_dir() ){
					ret = Dir::remove(entry, true);
				} else {
					ret = File::remove(entry);
				}
			}
		}
		d.close();
	}

	if( ret >= 0 ){
		//this will remove an empty directory or a file
		ret = File::remove(path);
	}

	return ret;
}
#else
int Dir::remove(const char * path, bool recursive, link_transport_mdriver_t * d){
	return -1;
}
#endif

int Dir::open(const char * name){
#if defined __link
	m_dirp = link_opendir(driver(), name);
#else
	m_dirp = opendir(name);
#endif
	if( m_dirp == 0 ){
        set_error_number(errno);
		return -1;
	}

	m_path.assign(name);

	return 0;
}


#if !defined __link
int Dir::count(){
	long loc;
	int count;

	if( !is_open() ){
		return -1;
	}

#if defined __link

#else
	loc = tell();
#endif

#if defined __link

#else
	rewind();
#endif

	count = 0;
	while( read() != 0 ){
		count++;
	}

	seek(loc);

	return count;

}

#endif


const char * Dir::read(){

#if defined __link
	struct link_dirent * result;
	if( link_readdir_r(driver(), m_dirp, &m_entry, &result) < 0 ){
		return 0;
	}
#else
	struct dirent * result;
	if( readdir_r(m_dirp, &m_entry, &result) < 0 ){
        set_error_number(errno);
		return 0;
	}
#endif
	return m_entry.d_name;
}

bool Dir::get_entry(var::String & path_dest){
	const char * entry = read();

	if( entry == 0 ){
		return false;
	}

	path_dest.assign(m_path);
	path_dest.append("/");
	path_dest.append(entry);
	return true;
}

int Dir::close(){
	m_path.clear();
	if( m_dirp ){
#if defined __link
		link_closedir(driver(), m_dirp);
#else
        if( closedir(m_dirp) < 0 ){
            set_error_number(errno);
        }
#endif
		m_dirp = 0;
	}
	return 0;
}

