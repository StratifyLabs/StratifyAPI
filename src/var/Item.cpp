//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "fs.hpp"
#include "var/Item.hpp"

namespace var {

int ItemObject::save(const arg::DestinationFilePath path) const {
	fs::File f;
	int ret;
	if( f.create(
			 path,
			 arg::IsOverwrite(true)
			 ) < 0 ){
		return -1;
	}
	ret = f.write(
				arg::SourceBuffer(data_void_const()),
				arg::Size(size())
				);
	return ret;
}

int ItemObject::load(const arg::SourceFilePath & path){
	fs::File f;
	int ret;
	if( f.open(
			 arg::FilePath(path.argument()),
			 fs::OpenFlags::read_only()
			 ) < 0 ){
		return -1;
	}
	ret = f.read(
				arg::DestinationBuffer(data_void()),
				arg::Size(size())
				);
	return ret;
}


void ItemObject::clear(){ memset(data_void(), 0, size()); }

}

