#if !defined __link
#include <errno.h>
#endif

#include <cstdio>
#include <cstdlib>

#include "api/SObject.hpp"

using namespace api;

SObject::SObject(){
    set_error_number(ERROR_NONE);
}


void SObject::exit_fatal(const char * message){
    printf("FATAL:%s\n", message);
    exit(1);
}

int SObject::set_error_number_if_error(int ret) const {
    if( ret < 0 ){
#if defined __link
        set_error_number(link_errno);
#else
        set_error_number(errno);
#endif
    }
    return ret;
}
