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

int Sched::get_priority(ProcessId pid){
	struct sched_param param;
	if( sched_getparam(pid.argument(), &param) < 0 ){
		return -1;
	}
	return param.sched_priority;
}

int Sched::get_rr_interval(ProcessId pid){
	struct timespec t;
	memset(&t, 0, sizeof(t));
	sched_rr_get_interval(pid.argument(), &t);
	return t.tv_nsec/1000;
}

int Sched::set_scheduler(
		ProcessId pid,
		enum policy value,
		Priority priority
		){
	struct sched_param param;
	param.sched_priority = priority.argument();
	return sched_setscheduler(pid.argument(), value, &param);
}

} /* namespace sys */
