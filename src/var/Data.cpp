//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <stdint.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <mcu/types.h>

#if !defined __link
#include <reent.h>
#endif

#include "var/Data.hpp"
using namespace var;

const int Data::m_zero_value = 0;

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
	if( m_needs_free ){
		::free(m_mem_write);
	}
	zero();
	return 0;
}

Data::~Data(){
	free();
}

void Data::set(void * mem, size_t s, bool readonly){

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


int Data::alloc(size_t s, bool resize){

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

int Data::set_min_capacity(size_t s){
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

