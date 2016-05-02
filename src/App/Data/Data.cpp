//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <stdint.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#if !defined __link
#include <reent.h>
#endif

#include "Var/Data.hpp"
using namespace Var;

const int Data::zero_value = 0;

#define MIN_CHUNK_SIZE 56

size_t Data::min_size(){
	return MIN_CHUNK_SIZE;
}




Data::Data(){
	zero();
}

Data::Data(void * mem, size_t s, bool readonly){
	set(mem, s, readonly);
}

Data::Data(size_t s){
	alloc(s);
}

int Data::free(){
#ifndef __HWPL_ONLY__
	if( needs_free ){
		::free(mem_write);
	}
#endif
	zero();
	return 0;
}

#ifndef __HWPL_ONLY__
Data::~Data(){
	free();
}
#endif

void Data::set(void * mem, size_t s, bool readonly){
	mem_write = mem;
	needs_free = false;
	_capacity = s;
	if( mem_write ){
		this->mem = mem_write;
	} else {
		mem = (void*)&zero_value;
	}
	if( readonly ){
		mem_write = 0;
	}
}

void Data::zero(void){
	mem = &zero_value;
	mem_write = 0;
	needs_free = false;
	_capacity = 0;
}


int Data::alloc(size_t s, bool resize){
#ifndef __HWPL_ONLY__

	void * new_data;
	if( (needs_free == false) && (mem != &zero_value) ){
		//this data object can't be resized -- it was created using a pointer (not dyn memory)
		return -1;
	}

	if( s < _capacity + MIN_CHUNK_SIZE ){
		s = _capacity + MIN_CHUNK_SIZE;
	}

	new_data = malloc(s);
	if( new_data == 0 ){
		return -1;
	}

	if( resize ){
		memcpy(new_data, mem, s > _capacity ? _capacity : s);
	}

	free();

	mem_write = new_data;
	needs_free = true;
	mem = mem_write;
	_capacity = s;
	return 0;
#else
	return -1;
#endif
}

int Data::set_capacity(size_t s){
	if( s <= capacity() ){
		return 0;
	}

	return alloc(s, true);
}

void Data::clear(void){ fill(0); }

void Data::fill(unsigned char d){
	if( mem_write ){
		memset(mem_write, d, capacity());
	}
}

