/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include <cstring>
#include <mcu/types.h>
#include "api/ApiObject.hpp"

#include "var/String.hpp"

#if defined __link
#define API_MINIMUM_CHUNK_SIZE 1024
#define API_MALLOC_CHUNK_SIZE 1024
#else
#define API_MINIMUM_CHUNK_SIZE 52
#define API_MALLOC_CHUNK_SIZE 64
#endif

using namespace api;

const char * ApiInfo::git_hash(){
	return SOS_GIT_HASH;
}

const char * ApiInfo::operating_system_name(){
#if defined __link

#if defined __macosx
	return "macosx";
#endif
#if defined __win32
	return "windows";
#endif

#else
	return "stratifyos";
#endif

	return "unknown";
}

bool ApiInfo::is_windows(){ return strcmp(operating_system_name(), "windows") == 0; }
bool ApiInfo::is_macosx(){ return strcmp(operating_system_name(), "macosx") == 0; }
bool ApiInfo::is_stratify_os(){ return strcmp(operating_system_name(), "stratifyos") == 0; }


u32 ApiInfo::malloc_start_chunk_size(){ return API_MINIMUM_CHUNK_SIZE; }
u32 ApiInfo::malloc_chunk_size(){ return API_MALLOC_CHUNK_SIZE; }


const char * ApiInfo::user_data_path(){

#if defined __link
	static var::String result;
	result.clear();
	char * path;
	if( is_windows() ){
		path = getenv("LocalAppData");
		result << path;
	}

	if( is_macosx() ){
		//read env home variable
		path = getenv("HOME");
		result << path << "/Library/Application Support";
	}

	return result.cstring();
#else
	return "/home";
#endif
}
