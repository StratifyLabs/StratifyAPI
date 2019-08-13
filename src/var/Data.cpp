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
#if defined __link
#define MIN_CHUNK_SIZE 1024
#define MALLOC_CHUNK_SIZE 1024
#else
#define MIN_CHUNK_SIZE 52
#define MALLOC_CHUNK_SIZE 64
#endif

u32 Data::minimum_size(){
	return MIN_CHUNK_SIZE;
}

u32 Data::block_size(){
	return MALLOC_CHUNK_SIZE;
}

Data::Data(){
	zero();
}

Data::Data(arg::DestinationBuffer mem,
			  const arg::Size s
			  ){
	zero();
	refer_to(
				mem,
				s,
				arg::IsReadOnly(false)
				);
}

Data::Data(const arg::SourceBuffer mem, const arg::Size s){
	zero();
	refer_to(
				arg::DestinationBuffer((void*)mem.argument()),
				s,
				arg::IsReadOnly(true)
				);
}

Data::Data(const arg::Size s){
	zero();
	allocate(s);
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

int Data::copy_contents(const arg::ImplicitSourceData a){
	return copy_contents(
				arg::SourceData(a.argument()),
				arg::Location(0),
				arg::Size(a.argument().size()));
}


int Data::copy_contents(const arg::SourceData a,
								const arg::Size size
								){
	return copy_contents(
				a,
				arg::Location(0),
				arg::Size(size));
}

int Data::copy_contents(const arg::SourceData a,
		const arg::Location destination,
		const arg::Size size){
	u32 size_value = size.argument();
	if( size_value > a.argument().size() ){ size_value = a.argument().size(); }
	if( capacity() < (size.argument() + destination.argument()) ){
		if( set_size(size_value + destination.argument()) < 0 ){
			return -1;
		}
	} else {
		m_size = size_value + destination.argument();
	}
	::memcpy(to_u8() + destination.argument(), a.argument().to_void(), size_value);
	return 0;
}

void Data::move_object(Data & a){
	if( this != &a ){
		if( a.is_internally_managed() ){
			//set this memory to the memory of a
			refer_to(
						arg::DestinationBuffer(a.to_void()),
						arg::Size(a.capacity()),
						arg::IsReadOnly(false)
						);
			m_size = a.size();

			//setting needs free on this and clearing it on a will complete the transfer
			set_needs_free();
			a.clear_needs_free();
		} else {
			refer_to(
						arg::DestinationBuffer(a.to_void()),
						arg::Size(a.size()),
						arg::IsReadOnly(a.is_read_only())
						);
		}
	}
}

void Data::copy_object(const Data & a){
	if( this != &a ){
		if( a.is_internally_managed() ){
			//copy the contents of a to this object
			copy_contents(
						arg::SourceData(a),
						arg::Size(a.capacity())
						);
			m_size = a.size();
		} else {
			refer_to(
						arg::DestinationBuffer((void*)a.to_void()),
						arg::Size(a.capacity()),
						arg::IsReadOnly(a.is_read_only())
						);
			m_size = a.size();
		}
	}
}

void Data::refer_to(const arg::DestinationBuffer mem,
						  const arg::Size s,
						  const arg::IsReadOnly readonly){

	//free the data if it was previously allocated dynamically
	free();

	m_mem_write = mem.argument();
	m_capacity = s.argument();
	m_size = s.argument();
	if( mem.argument() ){
		this->m_mem = m_mem_write;
	} else {
		//read should always point to a non-null
		this->m_mem = (void*)&m_zero_value;
	}

	if( readonly.argument() ){
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


int Data::allocate(
		const arg::Size s,
		const arg::IsResize resize){

	void * new_data;
	u32 original_size = s.argument();
	u32 size_value = s.argument();

	if( ( !needs_free() ) && (m_mem != &m_zero_value) ){
		//this data object can't be resized -- it was created using a pointer (not dynanmic memory allocation)
		return -1;
	}

	if( size_value == 0 ){
		zero();
		return 0;
	}

	if( size_value <= minimum_size() ){
		size_value = minimum_size();
	} else {
		//change s to allocate an integer multiple of minimum_size()
		u32 blocks = (size_value - minimum_size() + block_size() - 1) / block_size();
		size_value = minimum_size() + blocks * block_size();
	}

	new_data = malloc(size_value);
	if( set_error_number_if_null(new_data) == 0 ){
		return -1;
	}

	if( resize.argument() && m_capacity ){
		::memcpy(
					new_data,
					m_mem,
					size_value > m_capacity ? m_capacity : size_value
													  );
	}

	free();

	m_size = original_size;
	m_mem_write = new_data;
	set_needs_free();
	m_mem = m_mem_write;
	m_capacity = size_value;

	return 0;
}

int Data::set_size(const arg::ImplicitSize s){
	if( s.argument() <= capacity() ){
		m_size = s.argument();
		return 0;
	} //no need to increase size

	return allocate(arg::Size(s.argument()), arg::IsResize(true));
}

void Data::fill(unsigned char d){
	if( m_mem_write ){
		memset(m_mem_write, d, capacity());
	}
}

void Data::swap_byte_order(const arg::ImplicitSize size){

	if( data() ){
		if( size.argument() == 4 ){
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

