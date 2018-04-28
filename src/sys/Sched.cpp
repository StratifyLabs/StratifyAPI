/*
 * Sched.cpp
 *
 *  Created on: Jul 27, 2016
 *      Author: tgil
 */

#include <unistd.h>
#include <cstring>
#include "sys/Sched.hpp"

namespace sys {

int Sched::get_priority_max(enum policy value){
    return sched_get_priority_max(value);
}

int Sched::get_priority_min(enum policy value){
    return sched_get_priority_min(value);
}

int Sched::get_priority(pid_t pid){
	struct sched_param param;
    if( sched_getparam(pid, &param) < 0 ){
		return -1;
	}
	return param.sched_priority;
}

int Sched::get_rr_interval(pid_t pid){
	struct timespec t;
    memset(&t, 0, sizeof(t));
    sched_rr_get_interval(pid, &t);
	return t.tv_nsec/1000;
}

int Sched::set_scheduler(pid_t pid, enum policy value, int priority){
	struct sched_param param;
	param.sched_priority = priority;
    return sched_setscheduler(pid, value, &param);
}

} /* namespace sys */
