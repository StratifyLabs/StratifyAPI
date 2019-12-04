//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include <errno.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include "sys/Trace.hpp"
#include "var/Reference.hpp"

#if !defined __link
#include <reent.h>
#include <mcu/arch.h>
#endif

#include "var/Data.hpp"
using namespace var;

//this value corresponds to the malloc chunk size used in Stratify OS
//this may be something that could be determined through a system call
#if defined __link
#define MIN_CHUNK_SIZE 1024
#define MALLOC_CHUNK_SIZE 1024
#else
#define MIN_CHUNK_SIZE 52
#define MALLOC_CHUNK_SIZE 64
#endif

u32 Data::minimum_capacity(){
	return MIN_CHUNK_SIZE;
}

u32 Data::block_size(){
	return MALLOC_CHUNK_SIZE;
}

int Data::free(){
	m_data.clear();
	update_reference();
	return 0;
}

Data::Data(){
	//var::Refernence sets size to zero
}

Data::Data(size_t size){
	allocate(size);
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





