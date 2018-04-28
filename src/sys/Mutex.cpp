/* Copyright 2016 tgil All Rights Reserved */

#if !defined __link


#include <time.h>
#include "sys/Mutex.hpp"
#include "chrono.hpp"

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
	set_attr(attr);
}

int Mutex::set_attr(const MutexAttr & attr){
    return set_error_number_if_error(pthread_mutex_init(&m_item, &(attr.value())));
}

int Mutex::lock(){
    return set_error_number_if_error(pthread_mutex_lock(&m_item));
}

int Mutex::lock_timed(const chrono::ClockTime & clock_time){
    ClockTime calc_time;
    calc_time.get_time();
    calc_time += clock_time;
    return set_error_number_if_error(pthread_mutex_timedlock(&m_item, calc_time));
}

int Mutex::try_lock(){
    return set_error_number_if_error(pthread_mutex_trylock(&m_item));
}

int Mutex::unlock(){
    return set_error_number_if_error(pthread_mutex_unlock(&m_item));
}


#endif // !defined __link

