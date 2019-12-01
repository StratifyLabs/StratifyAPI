//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include <errno.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include "sys/Trace.hpp"
#include "var/Data.hpp"

#if !defined __link
#include <reent.h>
#include <mcu/arch.h>
#endif

#include "var/Reference.hpp"
using namespace var;

//This is here so that by default that data can point to a null value rather than be a null value
const int Reference::m_zero_value MCU_ALIGN(4) = 0;

//this value corresponds to the malloc chunk size used in Stratify OS
//this may be something that could be determined through a system call
#if defined __link
#define MIN_CHUNK_SIZE 1024
#define MALLOC_CHUNK_SIZE 1024
#else
#define MIN_CHUNK_SIZE 52
#define MALLOC_CHUNK_SIZE 64
#endif

Reference::Reference(){
	set_size_internally(0);
}

Reference::Reference(const Data & data){
	set_reference(
				ReadOnlyBuffer(data.read_data()),
				ReadWriteBuffer(nullptr),
				Size(data.size())
				);
}

Reference::Reference(Data & data){
	set_reference(
				ReadOnlyBuffer(data.read_data()),
				ReadWriteBuffer(data.write_data()),
				Size(data.size())
				);
}

void Reference::set_reference(
		ReadOnlyBuffer read_data,
		ReadWriteBuffer write_data,
		Size size
		){
	set_reference_internally(
				read_data,
				write_data,
				size);
}

void Reference::set_reference_internally(
		ReadOnlyBuffer read_data,
		ReadWriteBuffer write_data,
		Size size
		){
	m_data_read = read_data.argument();
	if( m_data_read == 0 ){
		m_data_read = &Reference::m_zero_value;
	}

	m_data_write = write_data.argument();
	m_size = size.argument();
}

void Reference::set_size_internally(u32 size){
	if( size == 0 ){
		set_reference_internally(
					ReadOnlyBuffer(nullptr),
					ReadWriteBuffer(nullptr),
					Size(0)
					);
	} else {
		m_size = size;
	}
}

void Reference::swap_byte_order(size_t size){

	if( write_data() ){
		if( size == 4 ){
			u32 * p = to_u32();
			if( p ){
				u32 i;
				for(i=0; i < this->size()/4; i++){
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
				for(i=0; i < this->size()/2; i++){
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

Reference::Reference(
		ReadOnlyBuffer buffer,
		Size size
		){
	refer_to(buffer, size);
}

Reference::Reference(
		ReadWriteBuffer buffer,
		Size size
		){
	refer_to(buffer, size);
}

Reference & Reference::refer_to(
		ReadOnlyBuffer read_only_data,
		Size size
		){
	set_reference(
				read_only_data,
				ReadWriteBuffer(nullptr),
				size);
	return *this;
}

Reference & Reference::refer_to(
		ReadWriteBuffer data,
		Size size
		){
	set_reference(
				ReadOnlyBuffer(data.argument()),
				data,
				size);
	return *this;

}


var::String Reference::to_string() const {

	var::String result;
	for(u32 i = 0; i < size(); i++){
		result << String().format("%02X", to_const_char()[i]);
	}

	return result;
}





