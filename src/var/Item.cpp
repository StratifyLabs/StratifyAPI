//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "fs.hpp"
#include "var/Item.hpp"

namespace var {

int ItemObject::save(const char * path) const {
	fs::File f;
	int ret;
	if( f.create(path, true) < 0 ){
		return -1;
	}
	ret = f.write(
				fs::SourceBuffer(data_void_const()),
				fs::Size(size())
				);
	return ret;
}

int ItemObject::load(const char * path){
	fs::File f;
	int ret;
	if( f.open(path, fs::OpenFlags::read_only()) < 0 ){
		return -1;
	}
	ret = f.read(
				fs::DestinationBuffer(data_void()),
				fs::Size(size())
				);
	return ret;
}


void ItemObject::clear(){ memset(data_void(), 0, size()); }

}

