#include <cstring>
#include <mcu/types.h>
#include "api/ApiObject.hpp"

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
