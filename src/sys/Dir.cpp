//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "sys/Dir.hpp"
using namespace sys;

#if defined __link
Dir::Dir(link_transport_mdriver_t * driver){
	m_dirp = 0;
	m_driver = driver;
}

#else
Dir::Dir(){
	m_dirp = 0;
}

#endif

int Dir::open(const char * name){
#if defined __link
	m_dirp = link_opendir(driver(), name);
#else
	m_dirp = opendir(name);
#endif
	if( m_dirp == 0 ){
		return -1;
	}

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
		return 0;
	}
#endif
	return m_entry.d_name;
}

int Dir::close(){
	if( m_dirp ){
#if defined __link
		link_closedir(driver(), m_dirp);
#else
		closedir(m_dirp);
#endif
		m_dirp = 0;
	}
	return 0;
}

