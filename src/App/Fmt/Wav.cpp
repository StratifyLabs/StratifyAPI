//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved



#include "Fmt/Wav.hpp"
using namespace Fmt;
using namespace Sys;

Wav::Wav(const char * name) {
	// TODO Auto-generated constructor stub
	open(name, File::READONLY);
	if( read(&hdr_, sizeof(hdr_)) < 0 ){
		close();
	}
}

