//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "Sys/Dir.hpp"
using namespace Sys;


Dir::Dir(){
	dirp = 0;
}

int Dir::open(const char * name){
	dirp = opendir(name);
	if( dirp == 0 ){
		return -1;
	}

	return 0;
}

int Dir::count(void){
	long loc;
	int count;

	loc = tell();

	rewind();

	count = 0;
	while( read() != 0 ){
		count++;
	}

	seek(loc);

	return count;

}

int Dir::size(){
	int i;
	if( !is_open() ){
		return -1;
	}
	rewind();
	i = 0;
	while( read() != 0 ){
		i++;
	}
	rewind();
	return i;
}



const char * Dir::read(void){
	struct dirent * result;
	if( readdir_r(dirp, &entry, &result) < 0 ){
		return 0;
	}
	return entry.d_name;
}

int Dir::close(void){
	if( dirp ){
		closedir(dirp);
		dirp = 0;
	}
	return 0;
}

