//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "Sys/Dir.hpp"
using namespace Sys;

#if defined __link
Dir::Dir(link_transport_mdriver_t * driver){
	dirp = 0;
	_driver = driver;
}

#else
Dir::Dir(){
	dirp = 0;
}

#endif

int Dir::open(const char * name){
#if defined __link
	dirp = link_opendir(driver(), name);
#else
	dirp = opendir(name);
#endif
	if( dirp == 0 ){
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
	if( link_readdir_r(driver(), dirp, &entry, &result) < 0 ){
		return 0;
	}
#else
	struct dirent * result;
	if( readdir_r(dirp, &entry, &result) < 0 ){
		return 0;
	}
#endif
	return entry.d_name;
}

int Dir::close(){
	if( dirp ){
#if defined __link
		link_closedir(driver(), dirp);
#else
		closedir(dirp);
#endif
		dirp = 0;
	}
	return 0;
}

