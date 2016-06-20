/* Copyright 2016 tgil All Rights Reserved */

#if !defined __link


#include <time.h>
#include "sys/Mutex.hpp"

using namespace sys;


MutexAttr::MutexAttr(){
	pthread_mutexattr_init(&m_item);
}

MutexAttr::MutexAttr(enum type t, int prio_ceiling, bool pshared){
	pthread_mutexattr_init(&m_item);
	set_type(t);
	set_prio_ceiling(prio_ceiling);
	set_pshared(pshared);
}

MutexAttr::~MutexAttr(){
	pthread_mutexattr_destroy(&m_item);
}

int MutexAttr::set_prio_ceiling(int value){
	return pthread_mutexattr_setprioceiling(&m_item, value);
}

int MutexAttr::set_protocol(enum protocol value){
	return pthread_mutexattr_setprotocol(&m_item, value);
}

int MutexAttr::set_pshared(bool value){
	return pthread_mutexattr_setpshared(&m_item, value);
}

int MutexAttr::set_type(enum type value){
	return pthread_mutexattr_settype(&m_item, value);
}

int MutexAttr::get_prio_ceiling() const {
	int ret;
	pthread_mutexattr_getprioceiling(&m_item, &ret);
	return ret;
}

int MutexAttr::get_protocol() const {
	int ret;
	pthread_mutexattr_getprotocol(&m_item, &ret);
	return ret;
}

int MutexAttr::get_type() const {
	int ret;
	pthread_mutexattr_gettype(&m_item, &ret);
	return ret;
}

bool MutexAttr::get_pshared() const {
	int ret;
	pthread_mutexattr_getpshared(&m_item, &ret);
	return ret;
}

Mutex::Mutex(){
	MutexAttr attr;
	pthread_mutex_init(&m_item, &(attr.value()));
}

Mutex::Mutex(const MutexAttr & attr){
	pthread_mutex_init(&m_item, &(attr.value()));
}



int Mutex::lock(){
	return pthread_mutex_lock(&m_item);
}

int Mutex::lock_timed(u32 sec, u32 usec){
	struct timespec abs_timeout;

	if( clock_gettime(CLOCK_REALTIME, &abs_timeout) < 0 ){
		return -1;
	}

	abs_timeout.tv_nsec += usec*1000;
	if( abs_timeout.tv_nsec > 1000000000 ){
		abs_timeout.tv_sec++;
		abs_timeout.tv_nsec -= 1000000000;
	}

	abs_timeout.tv_sec += sec;

	return pthread_mutex_timedlock(&m_item, &abs_timeout);

}

int Mutex::try_lock(){
	return pthread_mutex_trylock(&m_item);
}

int Mutex::unlock(){
	return pthread_mutex_unlock(&m_item);
}


#endif // !defined __link

