//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <mcu/task.h>
#include "Sys/Thread.hpp"
using namespace Sys;

Thread::Thread(int stack_size, bool detached) {
	// TODO Auto-generated constructor stub
	if( pthread_attr_init(&m_pthread_attr) < 0 ){
		set_id_error();
		return;
	}

	if( pthread_attr_setstacksize(&m_pthread_attr, stack_size) < 0 ){
		set_id_error();
		return;
	}

	if( detached == true ){
		if( pthread_attr_setdetachstate(&m_pthread_attr, PTHREAD_CREATE_DETACHED) < 0 ){
			set_id_error();
			return;
		}
	} else {
		if( pthread_attr_setdetachstate(&m_pthread_attr, PTHREAD_CREATE_JOINABLE) < 0 ){
			set_id_error();
			return;
		}
	}

	set_id_default();

}

int Thread::set_stacksize(int size){
	return pthread_attr_setstacksize(&m_pthread_attr, size);
}

int Thread::get_stacksize() const {
	size_t stacksize;
	pthread_attr_getstacksize(&m_pthread_attr, &stacksize);
	return stacksize;
}

int Thread::set_priority(int prio, int policy){
	struct sched_param param;
	if( (int)m_id == -1 ){
		return -1;
	}
	param.sched_priority = prio;

	return pthread_setschedparam(m_id, policy, &param);
}

void Thread::yield(){
	//sched_yield();
}

int Thread::get_priority() const {
	struct sched_param param;
	int policy;
	if( pthread_getschedparam(m_id, &policy, &param) < 0 ){
		return -1;
	}
	return param.sched_priority;
}

int Thread::get_policy() const {
	struct sched_param param;
	int policy;
	if( pthread_getschedparam(m_id, &policy, &param) < 0 ){
		return -1;
	}
	return policy;
}



int Thread::create(void * (*func)(void*), void * args, int prio, int policy){
	if( (int)m_id != -1 ){
		return -1;
	}

	//First create the thread
	if( pthread_create(&m_id, &m_pthread_attr, func, args) < 0 ){
		return -1;
	}

	//now set the priority
	if( set_priority(prio, policy) < 0 ){
		return -1;
	}

	return 0;
}

bool Thread::is_running() const {
	//check to see if the thread is running
	if( pthread_kill(m_id, 0) == 0 ){
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

