/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
/* Copyright 2016 tgil All Rights Reserved */


#include <time.h>
#include "sys/Mutex.hpp"
#include "chrono.hpp"

using namespace sys;

MutexAttributes::MutexAttributes(){
	validate();
	if( pthread_mutexattr_init(&m_item) < 0 ){
		invalidate();
	}
}

MutexAttributes::~MutexAttributes(){
	pthread_mutexattr_destroy(&m_item);
	invalidate();
}

MutexAttributes & MutexAttributes::set_prio_ceiling(int value){
#if !defined __android
	if( pthread_mutexattr_setprioceiling(&m_item, value) < 0 ){
		invalidate();
	}
#endif
	return *this;
}

MutexAttributes & MutexAttributes::set_protocol(enum protocol value){
#if !defined __android
	if( pthread_mutexattr_setprotocol(&m_item, value) < 0 ){
		invalidate();
	}
#endif
	return *this;
}

MutexAttributes & MutexAttributes::set_pshared(bool value){
	if( pthread_mutexattr_setpshared(&m_item, value) < 0){
		invalidate();
	}
	return *this;
}

MutexAttributes & MutexAttributes::set_type(enum type value){
	if( pthread_mutexattr_settype(&m_item, value) < 0){
		invalidate();
	}
	return *this;
}

int MutexAttributes::get_prio_ceiling() const {
	int ret = 0;
#if !defined __android
	pthread_mutexattr_getprioceiling(&m_item, &ret);
#endif
	return ret;
}

int MutexAttributes::get_protocol() const {
	int ret = 0;
#if !defined __android
	pthread_mutexattr_getprotocol(&m_item, &ret);
#endif
	return ret;
}

int MutexAttributes::get_type() const {
	int ret;
	pthread_mutexattr_gettype(&m_item, &ret);
	return ret;
}

bool MutexAttributes::get_pshared() const {
	int ret;
	pthread_mutexattr_getpshared(&m_item, &ret);
	return ret;
}

Mutex::Mutex(){
	MutexAttributes attr;
	set_error_number_if_error( pthread_mutex_init(&m_item, &(attr.m_item)) );
}

Mutex::Mutex(const MutexAttributes & attr){
	set_attributes(attr);
}

int Mutex::set_attributes(const MutexAttr & attr){
	return set_error_number_if_error(pthread_mutex_init(&m_item, &(attr.m_item)));
}

int Mutex::lock(){
	return set_error_number_if_error(pthread_mutex_lock(&m_item));
}

#if !defined __link
int Mutex::lock_timed(const chrono::ClockTime & clock_time){
	ClockTime calc_time;
	calc_time = Clock::get_time();
	calc_time += clock_time;
	return set_error_number_if_error(pthread_mutex_timedlock(&m_item, calc_time));
}
#endif

int Mutex::try_lock(){
	return set_error_number_if_error(pthread_mutex_trylock(&m_item));
}

int Mutex::unlock(){
	return set_error_number_if_error(pthread_mutex_unlock(&m_item));
}

