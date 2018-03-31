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
