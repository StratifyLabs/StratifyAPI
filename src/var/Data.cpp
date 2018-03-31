//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include <errno.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#if !defined __link
#include <reent.h>
#endif

#include "var/Data.hpp"
using namespace var;

//This is here so that by default that data can point to a null value rather than be a null value
const int Data::m_zero_value = 0;

//this value corresponds to the malloc chunk size used in Stratify OS
//this may be something that could be determined through a system call
#define MIN_CHUNK_SIZE 56

u32 Data::minimum_size(){
	return MIN_CHUNK_SIZE;
}

Data::Data(){
	zero();
}

Data::Data(void * mem, u32 s, bool readonly){
	zero();
	set(mem, s, readonly);
}

Data::Data(u32 s){
	zero();
	alloc(s);
}

int Data::free(){
	if( m_needs_free ){
		::free(m_mem_write);
	}
	zero();
	return 0;
}

Data::~Data(){
	free();
}

void Data::set(void * mem, u32 s, bool readonly){

	//free the data if it was previously allocated dynamically
	free();

	m_mem_write = mem;
	m_needs_free = false;
	m_capacity = s;
	if( m_mem_write ){
		this->m_mem = m_mem_write;
	} else {
		mem = (void*)&m_zero_value;
	}

	if( readonly ){
		m_mem_write = 0;
	}
}

void Data::zero(){
	m_mem = &m_zero_value;
	m_mem_write = 0;
	m_needs_free = false;
	m_capacity = 0;
}


int Data::alloc(u32 s, bool resize){

	void * new_data;
	if( (m_needs_free == false) && (m_mem != &m_zero_value) ){
		//this data object can't be resized -- it was created using a pointer (not dyn memory)
		return -1;
	}

	if( s < m_capacity + MIN_CHUNK_SIZE ){
		s = m_capacity + MIN_CHUNK_SIZE;
	}

	new_data = malloc(s);
	if( new_data == 0 ){
        set_error_number(errno);
		return -1;
	}

	if( resize ){
		memcpy(new_data, m_mem, s > m_capacity ? m_capacity : s);
	}

	free();

	m_mem_write = new_data;
	m_needs_free = true;
	m_mem = m_mem_write;
	m_capacity = s;

	return 0;
}

int Data::set_capacity(u32 s){
	if( s <= capacity() ){
		return 0;
	}

	return alloc(s, true);
}

void Data::clear(){ fill(0); }

void Data::fill(unsigned char d){
	if( m_mem_write ){
		memset(m_mem_write, d, capacity());
	}
}

