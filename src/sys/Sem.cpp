/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#if !defined __link

#include "sys/Sem.hpp"

using namespace sys;

Sem::Sem() {
	m_handle = 0;
}

int Sem::close(){ return sem_close(m_handle); }
int Sem::destroy(){ return sem_destroy(m_handle); }
int Sem::get_value() const {
	int value;
	int ret;
	ret = sem_getvalue(m_handle, &value);
	if( ret < 0 ){
		return ret;
	}
	return value;
}

int Sem::init(sem_t * sem, int pshared, unsigned int value){
	m_handle = sem;
	return set_error_number_if_error(sem_init(m_handle, pshared, value));
}

int Sem::open(const char * name, int o_flags, int mode, int value){
	m_handle = sem_open(name, o_flags, mode, value);
	if( m_handle != 0 ){
		return 0;
	}
	set_error_number_to_errno();
	return -1;
}

int Sem::create(const char * name, int value, bool exclusive){
	int o_flags = O_CREAT;
	if( exclusive ){
		o_flags |= exclusive;
	}
	return open(name, o_flags, 0666, value);
}

int Sem::post(){ return set_error_number_if_error(sem_post(m_handle)); }
int Sem::wait_timed(const chrono::ClockTime & timeout){ return set_error_number_if_error(sem_timedwait(m_handle, timeout.timespec())); }
int Sem::try_wait(){ return set_error_number_if_error(sem_trywait(m_handle)); }
int Sem::unlink(const char *name){ return sem_unlink(name); }
int Sem::wait(){ return set_error_number_if_error(sem_wait(m_handle)); }

#endif
