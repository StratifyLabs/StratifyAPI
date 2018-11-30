#include "api/ApiObject.hpp"

using namespace api;

const char * ApiObject::api_git_hash(){
#if defined SOS_GIT_HASH
	return SOS_GIT_HASH;
#else
	return "unknown";
#endif
}

const char * ApiObject::operating_system_name(){
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
