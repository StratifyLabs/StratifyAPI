//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include <sys/stat.h>
#if defined __win32
#include "dirent_windows.h"
#endif

#include "var/String.hpp"
#include "sys/File.hpp"
#include "sys/FileInfo.hpp"
#include "sys/Dir.hpp"
using namespace sys;



#if defined __link
Dir::Dir(link_transport_mdriver_t * driver){
	m_dirp = 0;
	m_driver = driver;

#if defined __win32
#else
	m_dirp_local = 0;
#endif

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
int Dir::remove(const var::ConstString & path, bool recursive){
#else
int Dir::remove(const var::ConstString & path, bool recursive, link_transport_mdriver_t * driver){
#endif
	int ret = 0;
	if( recursive ){
		Dir d;
		if( d.open(path) == 0 ){
			var::String entry;
			while( d.get_entry(entry) && (ret >= 0) ){
				FileInfo info;
				info.get_info(entry.str());
				if( info.is_directory() ){
					if( entry != "." && entry != ".." ){
#if defined __link
						ret = Dir::remove(entry.cstring(), true, driver);
#else
						ret = Dir::remove(entry.cstring(), true);
#endif
					}
				} else {
#if defined __link
					ret = File::remove(entry.cstring(), driver);
#else
					ret = File::remove(entry.cstring());
#endif
				}
			}
		}
		d.close();
	}

	if( ret >= 0 ){
		//this will remove an empty directory or a file
#if defined __link
		ret = File::remove(path, driver);
#else
		ret = File::remove(path);
#endif
	}

	return ret;
}

#if defined __link
int Dir::create(const var::ConstString & path, int mode, link_transport_mdriver_t * driver){
	int result;
	if( driver ){
		result = link_mkdir(driver, path.cstring(), mode);
	} else {
		//open a directory on the local system (not over link)
#if defined __win32
		result = mkdir(path.cstring());
#else
		result = mkdir(path.cstring(), mode);
#endif
	}
	return result;
}
#else
int Dir::create(const var::ConstString & path, int mode){
	return mkdir(path.cstring(), mode);
}
#endif

#if defined __link
bool Dir::exists(const var::ConstString & path, link_transport_mdriver_t * driver){
	Dir d(driver);
#else
bool Dir::exists(const var::ConstString & path){
	Dir d;
#endif
	if( d.open(path) < 0 ){ return false; }
	d.close();
	return true;
}


int Dir::open(const var::ConstString & name){
#if defined __link
	if( driver() ){
		m_dirp = link_opendir(driver(), name.str());
	} else {
		//open a directory on the local system (not over link)

		m_dirp_local = opendir(name.str());
		if( m_dirp_local == 0 ){
			return -1;
		}

		return 0;
	}
#else
	m_dirp = opendir(name.str());
#endif

	if( m_dirp == 0 ){
		set_error_number_to_errno();
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

var::Vector<var::String> Dir::read_list(const var::ConstString & path){
	Dir directory;
	var::Vector<var::String> result;
	if( directory.open(path) < 0 ){ return result; }
	result = directory.read_list();
	directory.close();
	return result;
}

var::Vector<var::String> Dir::read_list(){
	var::Vector<var::String> result;
	var::String entry;

	do {
		entry.clear();
		entry = read();
		if( !entry.is_empty() ){
			result.push_back(entry);
		}
	} while( entry.is_empty() == false );

	return result;

}

const char * Dir::read(){

#if defined __link
	if( driver() ){
		struct link_dirent * result;
		if( link_readdir_r(driver(), m_dirp, &m_entry, &result) < 0 ){
			return 0;
		}
#if defined __macosx
	} else {
		struct dirent * result_local;
		if( (readdir_r(m_dirp_local, &m_entry_local, &result_local) < 0) || (result_local == 0) ){
			return 0;
		}
		return m_entry_local.d_name;
#endif
	}
#else
	struct dirent * result;
	if( readdir_r(m_dirp, &m_entry, &result) < 0 ){
		set_error_number_to_errno();
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

	path_dest.assign(m_path.str());
	path_dest.append("/");
	path_dest.append(entry);
	return true;
}

var::String Dir::get_entry(){
	var::String entry;
	get_entry(entry);
	return entry;
}

int Dir::close(){
	m_path.clear();
	if( m_dirp ){
#if defined __link
		if( driver() ){
			link_closedir(driver(), m_dirp);
		} else {

#if defined __win32
#else
			DIR * dirp_copy = m_dirp_local;
			m_dirp_local = 0;
			if( closedir(dirp_copy) < 0 ){
				return -1;
			}
#endif
			return 0;
		}
#else //__link
		if( closedir(m_dirp) < 0 ){
			set_error_number_to_errno();
		}
#endif
		m_dirp = 0;
	}
	return 0;
}

