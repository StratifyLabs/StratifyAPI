/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc


#include <errno.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include "fs/File.hpp"
#include "sys/Trace.hpp"
#include "var/Data.hpp"
#include "sys/Printer.hpp"

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
		result << String::number(to_const_u8()[i], "%02X");
	}
	return result;
}


int Reference::save(
		const var::String file_path,
		IsOverwrite is_overwrite
		) const {
	fs::File f;
	if( f.create(
				file_path,
				is_overwrite
				) < 0 ){
		return -1;
	}

	if( f.write(*this) != (int)size() ){
		return -1;
	}
	return 0;
}

sys::Printer& sys::operator << (sys::Printer& printer, const var::Reference & a){
	u32 o_flags = printer.o_flags();
	const s8 * ptrs8 = a.to_const_s8();
	const s16 * ptrs16 = a.to_const_s16();
	const s32 * ptrs32 = a.to_const_s32();
	const u8 * ptru8 = a.to_const_u8();
	const u16 * ptru16 = a.to_const_u16();
	const u32 * ptru32 = a.to_const_u32();

	int s;
	if( o_flags & sys::Printer::print_32 ){
		s = a.size() / 4;
	} else if( o_flags & sys::Printer::print_16 ){
		s = a.size() / 2;
	} else if( o_flags & sys::Printer::print_blob ){
		s = (a.size()+15) / 16;
	} else {
		s = a.size();
	}

	int i;
	u32 bytes_printed = 0;
	var::String data_string;



	for(i=0; i < s; i++){
		if( o_flags & sys::Printer::print_hex ){
			if( o_flags & sys::Printer::print_32 ){
				data_string << var::String().format(F32X, ptru32[i]);
			} else if( o_flags & sys::Printer::print_16 ){
				data_string << var::String().format("%X", ptru16[i]);
			} else if( o_flags & sys::Printer::print_blob ){
				for(u32 j=0; j < 16; j++){
					data_string << var::String().format("%02X ", ptru8[i*16+j]);
					if( j < 15 ){
						data_string << " ";
					}
					bytes_printed++;
					if( bytes_printed == a.size() ){
						break;
					}
				}
			} else {
				data_string << var::String().format("%X", ptru8[i]);
			}
			data_string << " ";
		}
		if( o_flags & sys::Printer::print_unsigned ){
			if( o_flags & sys::Printer::print_32 ){
				data_string << var::String().format(F32U, ptru32[i]);
			} else if( o_flags & sys::Printer::print_16 ){
				data_string << var::String().format("%u", ptru16[i]);
			} else if( o_flags & sys::Printer::print_blob ){
				for(u32 j=0; j < 16; j++){
					data_string << var::String().format("%u", ptru8[i*16+j]);
					if( j < 15 ){
						data_string << " ";
					}
				}
			} else {
				data_string << var::String().format("%u", ptru8[i]);
			}
			data_string << " ";
		}
		if( o_flags & sys::Printer::print_signed ){
			if( o_flags & sys::Printer::print_32 ){
				data_string << var::String().format(F32D, ptrs32[i]);
			} else if( o_flags & sys::Printer::print_16 ){
				data_string << var::String().format("%d", ptrs16[i]);
			} else if( o_flags & sys::Printer::print_blob ){
				for(u32 j=0; j < 16; j++){
					data_string << var::String().format("%d", ptru8[i*16+j]);
					if( j < 15 ){
						data_string << " ";
					}
				}
			} else {
				data_string << var::String().format("%d", ptrs8[i]);
			}
			data_string << " ";
		}
		if( o_flags & sys::Printer::print_char ){
			if( ptru8[i] == '\n' ){
				data_string << (" \\n");
			} else if( ptru8[i] == '\r' ){
				data_string << (" \\r");
			} else if( ptru8[i] == 0 ){
				data_string << (" null");
			} else if( ptru8[i] < 128){
				data_string << var::String().format(" %c", ptru8[i]);
			}
		}

		printer.print(
					printer.verbose_level(),
					var::String().format("[%04d]", i).cstring(),
					data_string.cstring()
					);
		data_string.clear();
	}

	return printer;
}



