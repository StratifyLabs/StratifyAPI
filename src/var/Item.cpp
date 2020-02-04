//Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#include "fs.hpp"
#include "var/Item.hpp"

namespace var {

int ItemObject::save(const var::String & path) const {
	fs::File f;
	int ret;
	if( f.create(
			 path,
			 fs::File::IsOverwrite(true)
			 ) < 0 ){
		return -1;
	}
	ret = f.write(
				data_void_const(),
				fs::File::Size(size())
				);
	return ret;
}

int ItemObject::load(const String & path){
	fs::File f;
	int ret;
	if( f.open(
			 path,
			 fs::OpenFlags::read_only()
			 ) < 0 ){
		return -1;
	}
	ret = f.read(
				data_void(),
				fs::File::Size(size())
				);
	return ret;
}


void ItemObject::clear(){ memset(data_void(), 0, size()); }

}

