#if !defined __link
#include <errno.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <sos/link.h>

#include "api/WorkObject.hpp"

using namespace api;

WorkObject::WorkObject(){
	set_error_number(ERROR_NONE);
}


void WorkObject::exit_fatal(const char * message){
	printf("\nFATAL:%s\n", message);
	exit(1);
}

int WorkObject::set_error_number_if_error(int ret) const {
	if( ret < 0 ){
#if defined __link
		set_error_number(link_errno);
#else
		set_error_number(errno);
#endif
	}
	return ret;
}

void * WorkObject::set_error_number_if_null(void * ret) const {
	if( ret == 0 ){
#if defined __link
		set_error_number(link_errno);
#else
		set_error_number(errno);
#endif
	}
	return ret;
}

void WorkObject::set_error_number_to_errno() const {
#if defined __link
	set_error_number(link_errno);
#else
	set_error_number(errno);
#endif
}

