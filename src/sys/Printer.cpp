
#include <cstdarg>

#include "sys/Printer.hpp"
#include "sys/Sys.hpp"
#include "sys/Task.hpp"
#include "sys/Cli.hpp"
#include "var/Data.hpp"
#include "var/Vector.hpp"
#include "var/String.hpp"
#include "var/Token.hpp"


using namespace sys;

Printer::Printer(){
	m_o_flags = PRINT_8 | PRINT_HEX;
	m_indent = 0;
	m_progress_width = 50;
}

Printer::~Printer(){
	print("\n");
}

void Printer::print_indentation(){
	for(u16 i = 0; i < m_indent; i++){
		print("  ");
	}
}

void Printer::vprint_indented(const var::ConstString & key, const char * fmt, va_list list){
	print("\n");
	print_indentation();
	if( !key.is_empty() ){
		print("%s: ", key.str());
	} else {
		//generate a unique key for JSON
	}
	vprint(fmt, list);
}

void Printer::print_indented(const var::ConstString & key, const char * fmt, ...){
	va_list list;
	va_start(list, fmt);
	vprint_indented(key, fmt, list);
	va_end(list);
}

void Printer::vprint(const char * fmt, va_list list){
	vprintf(fmt, list);
	fflush(stdout);
}


void Printer::print(const char * fmt, ...){
	va_list list;
	va_start(list, fmt);
	vprint(fmt, list);
	va_end(list);
}

Printer & Printer::operator << (const Cli & a){
	open_object(a.name());
	print_indented("publisher", "%s", a.publisher());
	open_object("arguments");
	for(u32 i = 0; i < a.count(); i++){
		print_indented(0, "%s", a.at(i).str());
	}
	close_object();
	close_object();

	return *this;
}

Printer & Printer::operator << (const var::Data & a){

	u32 o_flags = m_o_flags;

	const s8 * ptrs8 = (const s8*)a.data_const();
	const s16 * ptrs16 = (const s16*)a.data_const();
	const s32 * ptrs32 = (const s32*)a.data_const();
	const u8 * ptru8 = (const u8*)a.data_const();
	const u16 * ptru16 = (const u16*)a.data_const();
	const u32 * ptru32 = (const u32*)a.data_const();

	int s;
	if( o_flags & PRINT_32 ){
		s = a.size() / 4;
	} else if( o_flags & PRINT_16 ){
		s = a.size() / 2;
	} else {
		s = a.size();
	}

	int i;
	for(i=0; i < s; i++){
		print("[%d]=", i);
		if( o_flags & PRINT_HEX ){
			if( o_flags & PRINT_32 ){
				print(F32X, ptru32[i]);
			} else if( o_flags & PRINT_16 ){
				print("%X", ptru16[i]);
			} else {
				print("%X", ptru8[i]);
			}
			print(" ");
		}
		if( o_flags & PRINT_UNSIGNED ){
			if( o_flags & PRINT_32 ){
				print(F32U, ptru32[i]);
			} else if( o_flags & PRINT_16 ){
				print("%u", ptru16[i]);
			} else {
				print("%u", ptru8[i]);
			}
			print(" ");
		}
		if( o_flags & PRINT_SIGNED ){
			if( o_flags & PRINT_32 ){
				print(F32D, ptrs32[i]);
			} else if( o_flags & PRINT_16 ){
				print("%d", ptrs16[i]);
			} else {
				print("%d", ptrs8[i]);
			}
			print(" ");
		}
		if( o_flags & PRINT_CHAR ){
			if( ptru8[i] == '\n' ){
				print(" \\n");
			} else if( ptru8[i] == '\r' ){
				print(" \\r");
			} else if( ptru8[i] == 0 ){
				print(" null");
			} else if( ptru8[i] < 128){
				print(" %c", ptru8[i]);
			}
		}
		print("\n");
	}

	return *this;
}

Printer & Printer::operator << (const var::String & a){
	return key(0, a);
}

Printer & Printer::operator << (const char * a){
	return key(0, a);
}

Printer & Printer::operator << (const var::Token & a){
	m_indent++;
	for(u32 i=0; i < a.count(); i++){
		print_indented("%s", a.at(i).str());
	}
	m_indent--;
	return *this;
}

Printer & Printer::operator << (const var::Vector<var::String> & a){
	for(u32 i=0; i < a.count(); i++){
		print_indented(0, "%s", a.at(i).str());
	}
	return *this;
}

Printer & Printer::operator << (const sys::TaskInfo & a){
	print_indented("name", "%s", a.name());
	print_indented("id", "%ld", a.id());
	print_indented("pid", "%ld", a.pid());
	print_indented("memory size", "%ld", a.memory_size());
	print_indented("stack size", "%ld", a.stack_size());
	print_indented("heap size", "%ld", a.heap_size());
	return *this;
}

Printer & Printer::operator << (const sys::SysInfo & a ){
	print_indented("Name", "%s", a.name().str());
	print_indented("Serial Number", F3208X F3208X F3208X F3208X,
						a.serial_number().sn[3],
			a.serial_number().sn[2],
			a.serial_number().sn[1],
			a.serial_number().sn[0]);
	print_indented("Hardware ID",  F3208X, a.hardware_id());
	if( a.name() != "bootloader" ){
		print_indented("BSP Version",  "%s", a.bsp_version().str());
		print_indented("SOS Version",  "%s", a.sos_version().str());
		print_indented("CPU Architecture",  "%s", a.cpu_architecture().str());
		print_indented("CPU Frequency", F32D, a.cpu_frequency());
		print_indented("Application Signature", F32X, a.application_signature());

		print_indented("BSP Git Hash",  "%s", a.bsp_git_hash().str());
		print_indented("SOS Git Hash",  "%s", a.sos_git_hash().str());
		print_indented("MCU Git Hash",  "%s", a.mcu_git_hash().str());
	}
	return *this;
}

bool Printer::update_progress(int progress, int total){
	const u32 width = m_progress_width;
	printf("\r[");
	for(u32 i = 0; i < width; i++){
		if( (total != 0) && (i < (progress*width+total/2)/total) ){
			printf("#");
		} else {
			printf(" ");
		}
	}
	printf("]");
	fflush(stdout);

	if( (progress >= total) || (total == 0) ){
		for(u32 i = 0; i < width+2; i++){
			printf("\b \b");
		}
		fflush(stdout);
		print_indentation();
		print("progress: %s", "complete");
	}


	return false;
}

Printer & Printer::open_object(const var::ConstString & key){
	print_indented(key, "");
	m_indent++;
	return *this;
}

Printer & Printer::key(const var::ConstString & key, const var::String & a){
	print_indented(key, "%s", a.str());
	return *this;
}

Printer & Printer::key(const var::ConstString & key, const char * fmt, ...){
	va_list list;
	va_start(list, fmt);
	vprint_indented(key, fmt, list);
	va_end(list);
	return *this;
}

Printer & Printer::message(const char * fmt, ...){
	va_list list;
	va_start(list, fmt);
	vprint_indented("message", fmt, list);
	va_end(list);
	return *this;
}

Printer & Printer::warning(const char * fmt, ...){
	va_list list;
	va_start(list, fmt);
	vprint_indented("warning!", fmt, list);
	va_end(list);
	return *this;
}

Printer & Printer::error(const char * fmt, ...){
	va_list list;
	va_start(list, fmt);
	vprint_indented("!error!", fmt, list);
	va_end(list);
	return *this;
}

Printer & Printer::fatal(const char * fmt, ...){
	va_list list;
	va_start(list, fmt);
	vprint_indented("~fatal~", fmt, list);
	va_end(list);
	return *this;
}
