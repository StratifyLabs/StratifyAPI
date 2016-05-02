//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <mcu/task.h>
#include "Sys/Thread.hpp"
using namespace Sys;

Thread::Thread(int stack_size, bool detached) {
	// TODO Auto-generated constructor stub
	if( pthread_attr_init(&pthread_attr) < 0 ){
		set_id_var_error();
		return;
	}

	if( pthread_attr_setstacksize(&pthread_attr, stack_size) < 0 ){
		set_id_var_error();
		return;
	}

	if( detached == true ){
		if( pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_DETACHED) < 0 ){
			set_id_var_error();
			return;
		}
	} else {
		if( pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_JOINABLE) < 0 ){
			set_id_var_error();
			return;
		}
	}

	set_id_var_default();

}

int Thread::setstacksize(int size){
	return pthread_attr_setstacksize(&pthread_attr, size);
}

int Thread::stacksize(void) const {
	size_t stacksize;
	pthread_attr_getstacksize(&pthread_attr, &stacksize);
	return stacksize;
}

int Thread::setpriority(int prio, int policy){
	struct sched_param param;
	if( (int)id_var == -1 ){
		return -1;
	}
	param.sched_priority = prio;

	return pthread_setschedparam(id_var, policy, &param);
}

void Thread::yield(void){
	//sched_yield();
}

int Thread::priority(void) const {
	struct sched_param param;
	int policy;
	if( pthread_getschedparam(id_var, &policy, &param) < 0 ){
		return -1;
	}
	return param.sched_priority;
}

int Thread::policy(void) const {
	struct sched_param param;
	int policy;
	if( pthread_getschedparam(id_var, &policy, &param) < 0 ){
		return -1;
	}
	return policy;
}


int Thread::id(void) const {
	return id_var;
}

int Thread::create(void * (*func)(void*), void * args, int prio, int policy){
	if( (int)id_var != -1 ){
		return -1;
	}

	//First create the thread
	if( pthread_create(&id_var, &pthread_attr, func, args) < 0 ){
		return -1;
	}

	//now set the priority
	if( setpriority(prio, policy) < 0 ){
		return -1;
	}

	return 0;
}

bool Thread::isrunning(void) const {
	//check to see if the thread is running
	if( pthread_kill(id_var, 0) == 0 ){
		return true;
	}
	return false;
}

int Thread::wait(void**ret, int interval){

	//if thread is joinable, then join it

	//else just keep sampling until the thread completes

	return 0;
}

int Thread::join(int ident, void ** value_ptr){
	return pthread_join(ident, value_ptr);
}

