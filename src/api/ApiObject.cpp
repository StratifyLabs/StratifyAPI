#include "api/ApiObject.hpp"

using namespace api;

const char * ApiObject::api_git_hash(){
#if defined SOS_GIT_HASH
	return SOS_GIT_HASH;
#else
	return "unknown";
#endif
}
