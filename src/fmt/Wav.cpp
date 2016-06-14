//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved



#include "fmt/Wav.hpp"
using namespace fmt;
using namespace sys;

Wav::Wav(const char * name) {
	// TODO Auto-generated constructor stub
	open(name, File::READONLY);
	if( read(&m_hdr, sizeof(m_hdr)) < 0 ){
		close();
	}
}

