//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include <errno.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include "sys/Trace.hpp"

#if !defined __link
#include <reent.h>
#include <mcu/arch.h>
#endif

#include "var/Data.hpp"
using namespace var;

//This is here so that by default that data can point to a null value rather than be a null value
const int DataReference::m_zero_value MCU_ALIGN(4) = 0;

//this value corresponds to the malloc chunk size used in Stratify OS
//this may be something that could be determined through a system call
#if defined __link
#define MIN_CHUNK_SIZE 1024
#define MALLOC_CHUNK_SIZE 1024
#else
#define MIN_CHUNK_SIZE 52
#define MALLOC_CHUNK_SIZE 64
#endif

DataReference::DataReference(){
	set_size_internally(0);
}

void DataReference::set_reference(
		const arg::ReadOnlyBuffer read_data,
		arg::ReadWriteBuffer write_data,
		arg::Size size
		){
	set_reference_internally(
				read_data,
				write_data,
				size);
}

void DataReference::set_reference_internally(
		const arg::ReadOnlyBuffer read_data,
		arg::ReadWriteBuffer write_data,
		arg::Size size
		){
	m_data_read = read_data.argument();
	if( m_data_read == 0 ){
		m_data_read = &DataReference::m_zero_value;
	}

	m_data_write = write_data.argument();
	m_size = size.argument();
}

void DataReference::set_size_internally(u32 size){
	if( size == 0 ){
		set_reference_internally(
					arg::ReadOnlyBuffer(0),
					arg::ReadWriteBuffer(0),
					arg::Size(0)
					);
	} else {
		m_size = size;
	}
}

void DataReference::swap_byte_order(const arg::ImplicitSize size){

	if( write_data() ){
		if( size.argument() == 4 ){
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

DataReference::DataReference(arg::ReadOnlyBuffer buffer,
		arg::Size size
		){
	refer_to(buffer, size);
}

DataReference::DataReference(
		arg::ReadWriteBuffer buffer,
		arg::Size size
		){
	refer_to(buffer, size);
}

void DataReference::refer_to(
		const arg::ReadOnlyBuffer read_only_data,
		arg::Size size
		){
	set_reference(
				read_only_data,
				arg::ReadWriteBuffer(0),
				size);
}

void DataReference::refer_to(
		arg::ReadWriteBuffer data,
		arg::Size size
		){
	set_reference(
				arg::ReadOnlyBuffer(data.argument()),
				data,
				size);
}

u32 Data::minimum_capacity(){
	return MIN_CHUNK_SIZE;
}

u32 Data::block_size(){
	return MALLOC_CHUNK_SIZE;
}

Data::Data(){
	zero();
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
	if( is_free_needed() ){
		::free(write_data());
	}
	zero();
	return 0;
}

Data::~Data(){
	free();
}

bool Data::is_reference() const {
	return (is_free_needed() == false) &&
			(size() > 0);
}

void Data::set_reference(
		const arg::ReadOnlyBuffer read_data,
		arg::ReadWriteBuffer write_data,
		arg::Size size
		){

	//free if not already free
	free();
	set_reference_internally(read_data, write_data, size);
}

void Data::zero(){
	set_size_internally(0);
	set_free_needed(false);
	m_capacity = 0;
}


int Data::allocate(
		const arg::Size s,
		const arg::IsResize is_resize
		){

	void * new_data;
	u32 size_value = s.argument();

	if( is_reference() ){
		//this data object can't be resized -- it was created using a data reference
		return -1;
	}

	if( s.argument() == 0 ){
		free();
		return 0;
	}

	if( s.argument() <= capacity() ){
		//this will just change the effective size since capacity is already present
		set_size_internally(size_value);
		return 0;
	}


	if( size_value <= minimum_capacity() ){
		size_value = minimum_capacity();
	} else {
		//change s to allocate an integer multiple of minimum_size()
		u32 blocks = (size_value - minimum_capacity() + block_size() - 1) / block_size();
		size_value = minimum_capacity() + blocks * block_size();
	}

	new_data = ::malloc(size_value);

	if( set_error_number_if_null(new_data) == 0 ){
		//created new data failed but current object is intact
		return -1;
	}

	if( is_resize.argument() && (m_capacity > 0) ){
		//when resizing, old buffer needs to be copied to the new buffer
		//m_capacity has the capacity of the pre-allocation buffer
		memory_copy(
					arg::SourceBuffer(read_data()),
					arg::DestinationBuffer(new_data),
					arg::Size( size_value > m_capacity ? m_capacity : size_value )
					);
	}

	free();

	set_reference_internally(
				arg::ReadOnlyBuffer(new_data),
				arg::ReadWriteBuffer(new_data),
				arg::Size(s.argument())
				);

	set_free_needed();
	m_capacity = size_value;

	return 0;
}

int Data::copy_data(
		const void * buffer,
		u32 size
		){
	if( resize(size) < 0 ){
		return -1;
	}

	memory_copy(
				arg::SourceBuffer(buffer),
				arg::DestinationBuffer(to_u8()),
				arg::Size(size)
				);

	return 0;
}

int Data::copy_cstring(const char * str){
	int length = strlen(str);
	if( resize(length) < 0 ){
		return -1;
	}
	memory_copy(
				arg::SourceBuffer(str),
				arg::DestinationBuffer(to_void()),
				arg::Size(length)
				);
	return 0;
}

int Data::copy_contents(const arg::SourceData a){
	return copy_contents(
				arg::SourceData(a.argument()),
				arg::Position(0),
				arg::Size(a.argument().size()));
}


int Data::copy_contents(const arg::SourceData a,
								const arg::Size size
								){
	return copy_contents(
				a,
				arg::Position(0),
				arg::Size(size));
}

int Data::copy_contents(
		const arg::SourceData a,
		const arg::Position destination,
		const arg::Size size
		){

	u32 bytes_to_copy = size.argument() < a.argument().size() ?
				size.argument() :
				a.argument().size();

	u32 bytes_needed = bytes_to_copy + destination.argument();

	if( resize(
			 arg::Size(bytes_needed)
			 ) < 0 ){
		return -1;
	}

	memory_copy(
				arg::SourceBuffer(a.argument().to_const_void()),
				arg::DestinationBuffer(to_u8() + destination.argument()),
				arg::Size(bytes_to_copy)
			);

	return 0;
}

void Data::move_object(Data & a){
	if( this != &a ){

		free();

		set_reference_internally(
					arg::ReadOnlyBuffer(a.read_data()),
					arg::ReadWriteBuffer(a.write_data()),
					arg::Size(a.size())
					);

		//setting needs free on this and clearing it on a will complete the transfer
		m_capacity = a.capacity();
		set_free_needed( a.is_free_needed() );
		a.set_free_needed(false);
	}
}

void Data::copy_object(const Data & a){
	if( this != &a ){
		//copy the contents of a to this object
		copy_contents(
					arg::SourceData(a),
					arg::Size(a.size())
					);
	}
}





