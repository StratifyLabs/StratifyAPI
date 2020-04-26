/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc


#include <errno.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include "sys/Trace.hpp"
#include "var/Reference.hpp"
#include "var/Data.hpp"
#include "sys/Printer.hpp"

#if !defined __link
#include <reent.h>
#include <mcu/arch.h>

sys::Printer& operator << (sys::Printer& printer, const var::DataInfo & a){
	printer.key("arena", F32U, a.arena());
	printer.key("freeBlockCount", F32U, a.free_block_count());
	printer.key("freeSize", F32U, a.free_size());
	printer.key("usedSize", F32U, a.used_size());
	return printer;
}
#endif

using namespace var;

//this value corresponds to the malloc chunk size used in Stratify OS
//this may be something that could be determined through a system call


u32 Data::minimum_capacity(){
	return api::ApiInfo::malloc_start_chunk_size();
}

u32 Data::block_size(){
	return api::ApiInfo::malloc_chunk_size();;
}

int Data::free(){
	//swapping with an empty vector forces the vector to free the memory
	//other requests to free the memory are non-binding
	//such as resize(), shrink_to_fit(), clear() and erase()
	m_data = std::vector<u8>();
	update_reference();
	return 0;
}

Data::Data(){
	//var::Refernence sets size to zero
}

Data::Data(size_t size){
	allocate(size);
}

Data Data::from_string(const var::String& value){
	Data result(value.length()/2);
	for(u32 i=0; i < value.length()/2; i++){
		result.to_u8()[i] =
				value.create_sub_string(
					String::Position(i*2),
					String::Length(2))
				.to_unsigned_long(String::base_16);
	}
	return result;
}

void Data::update_reference(){
	set_reference(
				ReadOnlyBuffer(data()),
				ReadWriteBuffer(data()),
				Size(m_data.size())
				);
}

bool Data::is_reference() const {
	if( size() > 0 && m_data.empty() ){
		return true;
	}
	return false;
}

int Data::allocate(
		size_t s,
		IsResize is_resize
		){
	MCU_UNUSED_ARGUMENT(is_resize);

	m_data.resize(s);
	update_reference();
	if( m_data.size() < s ){
		return -1;
	}
	return 0;
}

int Data::copy_contents(
		const Reference & reference
		){
	return copy_contents(
				reference,
				Position(0),
				Size(reference.size())
				);
}


int Data::copy_contents(
		const Reference & a,
		Size size
		){
	return copy_contents(
				a,
				Position(0),
				size
				);
}

int Data::copy_contents(
		const Reference & a,
		Position destination_position,
		Size size
		){

	u32 bytes_to_copy = size.argument() < a.size() ?
				size.argument() :
				a.size();

	u32 bytes_needed = bytes_to_copy + destination_position.argument();

	if( resize(bytes_needed) < 0 ){
		return -1;
	}

	memory_copy(
				SourceBuffer(a.to_const_void()),
				DestinationBuffer(to_u8() + destination_position.argument()),
				Size(bytes_to_copy)
				);

	return 0;
}


Data & Data::append(const Reference & reference){
	m_data.reserve(
				size() + reference.size()
				);

	for(u32 i=0; i < reference.size(); i++){
		m_data.push_back(reference.to_const_u8()[i]);
	}
	update_reference();

	return *this;
}

Data & Data::operator << (const Reference & reference){
	return append(reference);
}

Data & Data::operator << (u8 a){
	return append(Reference(
						  Reference::ReadOnlyBuffer(&a),
						  Reference::Size(sizeof(a))
						  )
					  );
}
Data & Data::operator << (s8 a){
	return append(Reference(
						  Reference::ReadOnlyBuffer(&a),
						  Reference::Size(sizeof(a))
						  )
					  );
}
Data & Data::operator << (u16 a){
	return append(Reference(
						  Reference::ReadOnlyBuffer(&a),
						  Reference::Size(sizeof(a))
						  )
					  );
}

Data & Data::operator << (s16 a){
	return append(Reference(
						  Reference::ReadOnlyBuffer(&a),
						  Reference::Size(sizeof(a))
						  )
					  );
}

Data & Data::operator << (u32 a){
	return append(Reference(
						  Reference::ReadOnlyBuffer(&a),
						  Reference::Size(sizeof(a))
						  )
					  );
}
Data & Data::operator << (s32 a){
	return append(Reference(
						  Reference::ReadOnlyBuffer(&a),
						  Reference::Size(sizeof(a))
						  )
					  );
}
Data & Data::operator << (u64 a){
	return append(Reference(
						  Reference::ReadOnlyBuffer(&a),
						  Reference::Size(sizeof(a))
						  )
					  );
}

Data & Data::operator << (s64 a){
	return append(Reference(
						  Reference::ReadOnlyBuffer(&a),
						  Reference::Size(sizeof(a))
						  )
					  );
}





