//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include "Sys/Sem.hpp"

using namespace Sys;

Sem::Sem() { handle = 0; }
int Sem::close(){ return sem_close(handle); }
int Sem::destroy(){ return sem_destroy(handle); }
int Sem::getvalue(int *sval){ return sem_getvalue(handle, sval); }
int Sem::init(sem_t * sem, int pshared, unsigned int value){
	handle = sem;
	return sem_init(handle, pshared, value);
}
int Sem::open(const char * name, int oflag, int mode, int value){
	handle = sem_open(name, oflag, mode, value);
	if( handle != 0 ){
		return 0;
	}
	return -1;
}
int Sem::post(){ return sem_post(handle); }
int Sem::timedwait(const struct timespec * timeout){ return sem_timedwait(handle, timeout); }
int Sem::trywait(){ return sem_trywait(handle); }
int Sem::unlink(const char *name){ return sem_unlink(name); }
int Sem::wait(){ return sem_wait(handle); }
