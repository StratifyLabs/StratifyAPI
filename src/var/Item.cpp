//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "sys.hpp"
#include "var/Item.hpp"

namespace var {

int ItemObject::save(const char * path) const {
	File f;
	int ret;
	if( f.create(path, true) < 0 ){
		return -1;
	}
	ret = f.write(data_void_const(), size());
	return ret;
}

int ItemObject::load(const char * path){
	File f;
	int ret;
	if( f.open_readonly(path) < 0 ){
		return -1;
	}
	ret = f.read(data_void(), size());
	return ret;
}


void ItemObject::clear(){ memset(data_void(), 0, size()); }

}

