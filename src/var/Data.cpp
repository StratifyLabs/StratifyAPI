//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include <errno.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#if !defined __link
#include <reent.h>
#include <mcu/arch.h>
#endif

#include "var/Data.hpp"
using namespace var;

//This is here so that by default that data can point to a null value rather than be a null value
const int Data::m_zero_value MCU_ALIGN(4) = 0;

//this value corresponds to the malloc chunk size used in Stratify OS
//this may be something that could be determined through a system call
#define MIN_CHUNK_SIZE 52
#define MALLOC_CHUNK_SIZE 64

u32 Data::minimum_size(){
	return MIN_CHUNK_SIZE;
}

u32 Data::block_size(){
	return MALLOC_CHUNK_SIZE;
}

Data::Data(){
	zero();
}

Data::Data(void * mem, u32 s){
	zero();
	set(mem, s, false);
}

Data::Data(const void * mem, u32 s){
	zero();
	set((void*)mem, s, true);
}

Data::Data(u32 s){
	zero();
	alloc(s);
}


Data::Data(const Data & a){
	zero();
	copy_object(a);
}

Data::Data(Data && a){
	zero();
	move_object(a);
}

Data& Data::operator=(const Data & a){
	copy_object(a);
	return *this;
}

Data& Data::operator=(Data && a){
	move_object(a);
	return *this;
}


int Data::free(){
	if( needs_free() ){
		::free(m_mem_write);
	}
	zero();
	return 0;
}

Data::~Data(){
	free();
}

int Data::copy_data(const void * buffer, u32 size){
	if( set_size(size) < 0 ){
		return -1;
	}
	::memcpy(to_u8(), buffer, size);
	return 0;
}

int Data::copy_cstring(const char * str){
	int length = strlen(str);
	if( set_size(length) < 0 ){
		return -1;
	}
	::memcpy(to_char(), str, length);
	return 0;
}

int Data::copy_contents(const Data & a){
	return copy_contents(a, 0, a.size());
}


int Data::copy_contents(const Data & a, u32 size){
	return copy_contents(a, 0, size);
}

int Data::copy_contents(const Data & a, u32 destination_position, u32 size){
	if( size > a.size() ){ size = a.size(); }
	if( capacity() < (size + destination_position) ){
		if( set_size(size + destination_position) < 0 ){
			return -1;
		}
	} else {
		m_size = size + destination_position;
	}
	::memcpy(to_u8() + destination_position, a.to_void(), size);
	return 0;
}

void Data::move_object(Data & a){
	if( a.is_internally_managed() ){
		//set this memory to the memory of a
		set(a.to_void(), a.capacity(), false);
		m_size = a.size();

		//setting needs free on this and clearing it on a will complete the transfer
		set_needs_free();
		a.clear_needs_free();
	} else {
		set(a.to_void(), a.size(), a.is_read_only());
	}
}

void Data::copy_object(const Data & a){
	if( a.is_internally_managed() ){
		//copy the contents of a to this object
		copy_contents(a, a.capacity());
		m_size = a.size();
	} else {
		set((void*)a.data(), a.capacity(), a.is_read_only());
		m_size = a.size();
	}
}

void Data::refer_to(void * mem, u32 s, bool readonly){

	//free the data if it was previously allocated dynamically
	free();

	m_mem_write = mem;
	m_capacity = s;
	m_size = s;
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
	clear_needs_free();
	m_capacity = 0;
	m_size = 0;
	m_o_flags = 0;
}


int Data::allocate(u32 s, bool resize){

	void * new_data;
	u32 original_size = s;

	if( ( !needs_free() ) && (m_mem != &m_zero_value) ){
		//this data object can't be resized -- it was created using a pointer (not dynanmic memory allocation)
		return -1;
	}

	if( s == 0 ){
		zero();
		return 0;
	}


	if( s <= minimum_size() ){
		s = minimum_size();
	} else {
		//change s to allocate an integer multiple of minimum_size()
		u32 blocks = (s - minimum_size() + block_size() - 1) / block_size();
		s = minimum_size() + blocks * block_size();
	}

	new_data = malloc(s);
	if( set_error_number_if_null(new_data) == 0 ){
		return -1;
	}

	if( resize && m_capacity ){
		::memcpy(new_data, m_mem, s > m_capacity ? m_capacity : s);
	}

	free();

	m_size = original_size;
	m_mem_write = new_data;
	set_needs_free();
	m_mem = m_mem_write;
	m_capacity = s;

	return 0;
}

int Data::set_size(u32 s){
	if( s <= capacity() ){
		m_size = s;
		return 0;
	} //no need to increase size

	return allocate(s, true);
}

void Data::clear(){ fill(0); }

void Data::fill(unsigned char d){
	if( m_mem_write ){
		memset(m_mem_write, d, capacity());
	}
}

void Data::swap_byte_order(int size){

	if( data() ){
		if( size == 4 ){
			u32 * p = to_u32();
			if( p ){
				u32 i;
				for(i=0; i < capacity()/4; i++){
#if !defined __link
					p[i] = __REV(p[i]);
#else
					//swap manually
#endif

				}
			} else {
				set_error_number(EINVAL);
			}
		} else {
			u16 * p = to_u16();
			if( p ){
				u16 i;
				for(i=0; i < capacity()/2; i++){
#if !defined __link
					p[i] = __REV16(p[i]);
#else
					//swap manually
#endif

				}
			} else {
				set_error_number(EINVAL);
			}
		}
	}

}

void Data::print(unsigned int value) const {
	MCU_UNUSED_ARGUMENT(value);
}

