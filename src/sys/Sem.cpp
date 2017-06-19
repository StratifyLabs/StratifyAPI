//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#if !defined __link

#include "sys/Sem.hpp"

using namespace sys;

Sem::Sem() { m_handle = 0; }
int Sem::close(){ return sem_close(m_handle); }
int Sem::destroy(){ return sem_destroy(m_handle); }
int Sem::get_value(int *sval){ return sem_getvalue(m_handle, sval); }
int Sem::init(sem_t * sem, int pshared, unsigned int value){
	m_handle = sem;
	return sem_init(m_handle, pshared, value);
}
int Sem::open(const char * name, int oflag, int mode, int value){
	m_handle = sem_open(name, oflag, mode, value);
	if( m_handle != 0 ){
		return 0;
	}
	return -1;
}
int Sem::post(){ return sem_post(m_handle); }
int Sem::timedwait(const struct timespec * timeout){ return sem_timedwait(m_handle, timeout); }
int Sem::trywait(){ return sem_trywait(m_handle); }
int Sem::unlink(const char *name){ return sem_unlink(name); }
int Sem::wait(){ return sem_wait(m_handle); }

#endif
