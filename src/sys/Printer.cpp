/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#include <cstdarg>
#if defined __win32
#include <windows.h>
#endif

#include "sys/Printer.hpp"
#include "sys/Sys.hpp"
#include "hal/Core.hpp"
#include "hal/Drive.hpp"
#include "sys/TaskManager.hpp"
#include "sys/Cli.hpp"
#include "fs/Stat.hpp"
#include "sys/Appfs.hpp"
#include "var/Data.hpp"
#include "var/Datum.hpp"
#include "var/Vector.hpp"
#include "var/Ring.hpp"
#include "var/Json.hpp"
#include "var/String.hpp"
#include "var/Token.hpp"
#include "sgfx/Bitmap.hpp"
#include "sgfx/Vector.hpp"
#include "chrono/Time.hpp"
#include "chrono/ClockTime.hpp"
#include "draw/Drawing.hpp"

#if defined __win32
unsigned int sys::Printer::m_default_color = (unsigned int)-1;
#endif


using namespace sys;

Printer::Printer() : m_progress_callback(Printer::update_progress_callback, this){
	m_o_flags = PRINT_8 | PRINT_HEX;
	m_indent = 0;
	m_progress_width = 50;
	m_progress_state = 0;
	m_verbose_level = INFO;
	m_progress_key = "progress";
#if defined __win32
	if( m_default_color == (unsigned int)-1 ){
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
		m_default_color = info.wAttributes;
	}
#endif

#if defined __link
	m_is_bash = false;
#endif
}


void Printer::set_format_code(u32 code){
#if defined __link
	if( api::ApiInfo::is_macosx() || is_bash() ){
		print_final("\033[1;%dm", code);
	}
#endif
}

void Printer::clear_format_code(u32 code){
#if defined __link
	if( api::ApiInfo::is_macosx() || is_bash() ){
		print_final("\033[1;2%dm", code);
	}
#endif
}

void Printer::set_color_code(u32 code){

#if defined __link
	if( api::ApiInfo::is_macosx() || is_bash() ){
		print_final("\033[1;%dm", code);
	}
#endif

#if defined __link && defined __win32
	WORD color;
	switch(code){
		case COLOR_CODE_DEFAULT: color = m_default_color; break;
		case COLOR_CODE_BLACK: color = 0; break;
		case COLOR_CODE_RED: color = FOREGROUND_RED; break;
		case COLOR_CODE_GREEN: color = FOREGROUND_GREEN; break;
		case COLOR_CODE_YELLOW: color = FOREGROUND_RED | FOREGROUND_GREEN; break;
		case COLOR_CODE_BLUE: color = FOREGROUND_BLUE; break;
		case COLOR_CODE_MAGENTA: color = FOREGROUND_BLUE | FOREGROUND_RED; break;
		case COLOR_CODE_CYAN: color = FOREGROUND_BLUE | FOREGROUND_GREEN; break;
		case COLOR_CODE_LIGHT_GRAY: color = FOREGROUND_INTENSITY; break;
		case COLOR_CODE_DARK_GRAY: color = 0; break;
		case COLOR_CODE_LIGHT_RED: color = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
		case COLOR_CODE_LIGHT_GREEN: color = FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
		case COLOR_CODE_LIGHT_YELLOW: color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
		case COLOR_CODE_LIGHT_BLUE: color = FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
#endif

}

void Printer::print_final_color(enum color_code code, const char * snippet){
	set_color_code(code);
	print_final(snippet);
	clear_color_code();
}

void Printer::print(
		enum verbose_level verbose_level,
		const char * key,
		const char * value,
		bool is_newline
		){
	//default flat printer behavior
	if( verbose_level > this->verbose_level() ){
		return;
	}

	for(u32 indent=0; indent < m_indent; indent++){
		print_final("  ");
	}

	if( key != nullptr ){
		if( m_o_flags & print_bold_keys ){ set_format_code(format_bold); }
		if( m_o_flags & print_cyan_keys ){ set_color_code(color_code_cyan); }
		if( m_o_flags & print_yellow_keys ){ set_color_code(color_code_yellow); }
		if( m_o_flags & print_magenta_keys ){ set_color_code(color_code_magenta); }
		if( m_o_flags & print_red_keys ){ set_color_code(color_code_red); }
		if( m_o_flags & print_key_quotes ){
			print_final("\"%s\": ", key);
		} else {
			print_final("%s: ", key);
		}
		if( m_o_flags & print_bold_keys ){ clear_format_code(format_bold); }
		if( m_o_flags & (print_cyan_keys | print_yellow_keys | print_magenta_keys | print_red_keys) ){ clear_color_code(); }
	}

	if( value != nullptr ){
		if( m_o_flags & print_bold_values ){ set_format_code(format_bold); }
		if( m_o_flags & print_green_values){ set_color_code(color_code_green); }
		if( m_o_flags & print_yellow_values){ set_color_code(color_code_yellow); }
		if( m_o_flags & print_red_values){ set_color_code(color_code_red); }
		if( m_o_flags & print_cyan_values){ set_color_code(color_code_cyan); }
		if( m_o_flags & print_value_quotes ){
			print_final("\"%s\"", value);
		} else {
			print_final("%s", value);
		}
		if( m_o_flags & (print_green_values | print_yellow_values | print_red_values | print_cyan_values) ){ clear_color_code(); }
		if( m_o_flags & print_bold_values ){ clear_format_code(format_bold); }
	}

	if( is_newline ){
		print_final("\n");
	}


}

void Printer::print_final(const char * fmt, ...){
	va_list list;
	va_start(list, fmt);
	vprintf(fmt, list);
	va_end(list);
	fflush(stdout);
}

Printer & Printer::open_object(
		const var::String & key,
		enum verbose_level level){
	print_open_object(level, key.cstring());
	return *this;
}

Printer & Printer::close_object(){
	print_close_object();
	return *this;
}

Printer & Printer::open_array(
		const var::String & key,
		enum verbose_level level){
	print_open_array(level, key.cstring());
	return *this;
}

Printer & Printer::close_array(){
	print_close_array();
	return *this;
}

void Printer::print_open_object(
		enum verbose_level verbose_level,
		const char * key
		){
	print(verbose_level, key, "");
	m_indent++;
}

void Printer::print_close_object(){
	if( m_indent ){
		m_indent--;
	}
}

void Printer::print_open_array(
		enum verbose_level verbose_level,
		const char * key
		){
	print(verbose_level, key, "");
	m_indent++;
}

void Printer::print_close_array(){
	if( m_indent ){
		m_indent--;
	}
}

void Printer::clear_color_code(){
#if defined __link
	if( api::ApiInfo::is_macosx() || is_bash() ){
		print_final("\033[0m");
	} else {
		set_color_code(COLOR_CODE_DEFAULT);
	}
#endif
}

u32 Printer::color_code(const var::String & color){
	var::String color_upper(color);
	color_upper.to_upper();
	if( color_upper == "BLACK" ){ return COLOR_CODE_BLACK; }
	if( color_upper == "RED" ){ return COLOR_CODE_RED; }
	if( color_upper == "GREEN" ){ return COLOR_CODE_GREEN; }
	if( color_upper == "YELLOW" ){ return COLOR_CODE_YELLOW; }
	if( color_upper == "BLUE" ){ return COLOR_CODE_BLUE; }
	if( color_upper == "MAGENTA" ){ return COLOR_CODE_MAGENTA; }
	if( color_upper == "CYAN" ){ return COLOR_CODE_CYAN; }
	if( color_upper == "LIGHT GRAY" ){ return COLOR_CODE_LIGHT_GRAY; }
	if( color_upper == "DARK GRAY" ){ return COLOR_CODE_DARK_GRAY; }
	if( color_upper == "LIGHT RED" ){ return COLOR_CODE_LIGHT_RED; }
	if( color_upper == "LIGHT GREEN" ){ return COLOR_CODE_LIGHT_GREEN; }
	if( color_upper == "LIGHT YELLOW" ){ return COLOR_CODE_LIGHT_YELLOW; }
	if( color_upper == "LIGHT BLUE" ){ return COLOR_CODE_LIGHT_BLUE; }

	return COLOR_CODE_DEFAULT;
}


Printer::~Printer(){}

#if 0
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
#endif

Printer & Printer::operator << (const Cli & a){
	print_open_object(
				verbose_level(),
				a.name().cstring()
				);
	{
		key("publisher", "%s", a.publisher().cstring());
		print_open_object(verbose_level(), "arguments");
		{
			for(u32 i = 0; i < a.count(); i++){
				key(0, "%s", a.at(i).cstring());
			}
			print_close_object();
		}
		print_close_object();
	}

	return *this;
}

Printer & Printer::operator << (const api::Result & a){
	key("returnValue", "%d", a.return_value());
	key("errorNumber", "%d", a.error_number());
	return *this;
}

#if !defined __link
Printer & Printer::operator << (const var::DataInfo & a){
	key("arena", F32U, a.arena());
	key("freeBlockCount", F32U, a.free_block_count());
	key("freeSize", F32U, a.free_size());
	key("usedSize", F32U, a.used_size());
	return *this;
}
#endif


Printer & Printer::operator << (const var::Reference & a){
	u32 o_flags = m_o_flags;
	const s8 * ptrs8 = a.to_const_s8();
	const s16 * ptrs16 = a.to_const_s16();
	const s32 * ptrs32 = a.to_const_s32();
	const u8 * ptru8 = a.to_const_u8();
	const u16 * ptru16 = a.to_const_u16();
	const u32 * ptru32 = a.to_const_u32();


	int s;
	if( o_flags & PRINT_32 ){
		s = a.size() / 4;
	} else if( o_flags & PRINT_16 ){
		s = a.size() / 2;
	} else if( o_flags & PRINT_BLOB ){
		s = (a.size()+15) / 16;
	} else {
		s = a.size();
	}

	int i;
	u32 bytes_printed = 0;
	var::String data_string;

	for(i=0; i < s; i++){
		if( o_flags & PRINT_HEX ){
			if( o_flags & PRINT_32 ){
				data_string << var::String().format(F32X, ptru32[i]);
			} else if( o_flags & PRINT_16 ){
				data_string << var::String().format("%X", ptru16[i]);
			} else if( o_flags & PRINT_BLOB ){
				for(u32 j=0; j < 16; j++){
					data_string << var::String().format("%02X", ptru8[i*16+j]);
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
		if( o_flags & PRINT_UNSIGNED ){
			if( o_flags & PRINT_32 ){
				data_string << var::String().format(F32U, ptru32[i]);
			} else if( o_flags & PRINT_16 ){
				data_string << var::String().format("%u", ptru16[i]);
			} else if( o_flags & PRINT_BLOB ){
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
		if( o_flags & PRINT_SIGNED ){
			if( o_flags & PRINT_32 ){
				data_string << var::String().format(F32D, ptrs32[i]);
			} else if( o_flags & PRINT_16 ){
				data_string << var::String().format("%d", ptrs16[i]);
			} else if( o_flags & PRINT_BLOB ){
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
		if( o_flags & PRINT_CHAR ){
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

		print(verbose_level(),
				var::String().format("[%04d]", i).cstring(),
				data_string.cstring()
				);
		data_string.clear();
	}

	return *this;
}

Printer & Printer::operator << (const var::Datum & a){
	*this << a.to_json();
	return *this;
}

Printer & Printer::operator << (const var::String & a){
	return key(nullptr, a);
}

Printer & Printer::operator << (const char * a){
	return key(nullptr, a);
}

Printer & Printer::operator << (const var::Tokenizer & a){
	print_open_object(verbose_level(), "tokens");
	for(u32 i=0; i < a.count(); i++){
		print(verbose_level(),
				var::String().format("[%04d]", i).cstring(),
				a.at(i).cstring()
				);
	}
	print_close_object();
	return *this;
}

Printer & Printer::operator << (const var::JsonObject & a){
	var::Vector<var::String> keys = a.keys();
	for(u32 i=0; i < keys.count(); i++){
		var::JsonValue entry = a.at(keys.at(i));
		if( entry.is_object() ){
			print_open_object(verbose_level(), keys.at(i).cstring());
			*this << entry.to_object();
			print_close_object();
		} else if( entry.is_array() ){
			print_open_object(verbose_level(), keys.at(i).cstring());
			*this << entry.to_array();
			print_close_object();
		} else {
			key(keys.at(i), entry.to_string().cstring());
		}
	}

	return *this;
}

Printer & Printer::operator << (const var::JsonArray & a){

	for(u32 i=0; i < a.count(); i++){
		var::JsonValue entry = a.at(i);
		var::String key;
		key.format("[%04d]", i);
		if( entry.is_object() ){
			print_open_object(verbose_level(), key.cstring());
			*this << entry.to_object();
			print_close_object();
		} else if( entry.is_array() ){
			print_open_object(verbose_level(), key.cstring());
			*this << entry.to_array();
			print_close_object();
		} else {
			this->key(key, entry.to_string().cstring());
		}
	}
	return *this;
}

Printer & Printer::key(
		const var::String & key,
		const var::JsonValue & a
		){

	if(a.is_object()){
		print_open_object(verbose_level(), key.cstring());
		*this << a.to_object();
		print_close_object();
	} else if( a.is_array() ){
		print_open_object(verbose_level(), key.cstring());
		*this << a.to_array();
		print_close_object();
	} else {
		this->key(key, a.to_string());
	}
	return *this;
}

Printer& Printer::set_verbose_level(
		const var::String & level
		){
	if( level == "debug" ){ set_verbose_level(level_debug); }
	else if( level == "info" ){ set_verbose_level(level_info); }
	else if( level == "message" ){ set_verbose_level(level_message); }
	else if( level == "warning" ){ set_verbose_level(level_warning); }
	else if( level == "error" ){ set_verbose_level(level_error); }
	else if( level == "fatal" ){ set_verbose_level(level_fatal); }
	else if( level == "trace" ){ set_verbose_level(level_trace); }
	return *this;
}



Printer & Printer::operator << (const var::Vector<var::String> & a){
	for(u32 i=0; i < a.count(); i++){
		key(0, "%s", a.at(i).cstring());
	}
	return *this;
}

Printer & Printer::operator << (const var::Ring<u32> & a){
	for(u32 i=0; i < a.count_ready(); i++){
		key(0, F32U, a.at(i));
	}
	return *this;
}

Printer & Printer::operator << (const var::Ring<s32> & a){
	for(u32 i=0; i < a.count_ready(); i++){
		key(0, F32D, a.at(i));
	}
	return *this;
}

Printer & Printer::operator << (const var::Ring<u16> & a){
	for(u32 i=0; i < a.count_ready(); i++){
		key(0, "%u", a.at(i));
	}
	return *this;
}

Printer & Printer::operator << (const var::Ring<s16> & a){
	for(u32 i=0; i < a.count_ready(); i++){
		key(0, "%d", a.at(i));
	}
	return *this;
}

Printer & Printer::operator << (const var::Ring<u8> & a){
	for(u32 i=0; i < a.count_ready(); i++){
		key(0, "%u", a.at(i));
	}
	return *this;
}

Printer & Printer::operator << (const var::Ring<s8> & a){
	for(u32 i=0; i < a.count_ready(); i++){
		key(0, "%d", a.at(i));
	}
	return *this;
}


Printer & Printer::operator << (const sys::TaskInfo & a){
	key("name", "%s", a.name().cstring());
	key("id", F32U, a.id());
	key("pid", F32U, a.pid());
	key("memorySize", F32U, a.memory_size());
	key("stack", "0x%lX", a.stack());
	key("stackSize", F32U, a.stack_size());
	key("priority", F32U, a.priority());
	key("priorityCeiling", F32U, a.priority_ceiling());
	key("isThread", a.is_thread() ? "true" : "false");
	if( a.is_thread() == false ){
		key("heap", "0x%lX", a.heap());
		key("heapSize", "%ld", a.heap_size());
	}
	return *this;
}

Printer & Printer::operator << (const fs::Stat & a){
	var::String type;
	if( a.is_directory() ){ type = "directory"; }
	if( a.is_file() ){ type = "file"; }
	if( a.is_device() ){ type = "device"; }
	if( a.is_block_device() ){ type = "block device"; }
	if( a.is_character_device() ){ type = "character device"; }
	if( a.is_socket() ){ type = "socket"; }
	key("type", type.cstring());
	if( a.is_file() ){
		key("size", "%ld", a.size());
	}
	key("mode", "0%o", a.permissions().permissions() & 0777);

	return *this;
}

Printer & Printer::operator << (
		const sys::SysInfo & a
		){
	key("name", "%s", a.name().cstring());
	key("serialNumber", a.serial_number().to_string().cstring());
	key("hardwareId",  F3208X, a.hardware_id());
	if( a.name() != "bootloader" ){
		key("projectId", a.id().cstring());
		if( a.team_id().is_empty() == false ){
			key("teamId", a.team_id().cstring());
		}
		key("bspVersion",	a.bsp_version().cstring());
		key("sosVersion",	a.sos_version().cstring());
		key("cpuArchitecture", a.cpu_architecture().cstring());
		key("cpuFrequency", F32D, a.cpu_frequency());
		key("applicationSignature", F32X, a.application_signature());

		key("bspGitHash",	a.bsp_git_hash().cstring());
		key("sosGitHash",	a.sos_git_hash().cstring());
		key("mcuGitHash",	a.mcu_git_hash().cstring());
	}
	return *this;
}

Printer & Printer::operator << (const sys::AppfsInfo & a){
	key("name", "%s", a.name().cstring());
	key("mode", "0%o", a.mode());
	if( a.is_executable() ){
		key("id", "%s", a.id().cstring());
		key("version", "%d.%d", a.version() >> 8, a.version() & 0xff);
		key("signature", F3208X, a.signature());
		key("ram", F32U, a.ram_size());
		key("orphan", a.is_orphan() ? "true" : "false");
		key("flash", a.is_flash() ? "true" : "false");
		key("startup", a.is_startup() ? "true" : "false");
		key("unique", a.is_unique() ? "true" : "false");
	}
	return *this;
}

Printer & Printer::operator << (const hal::DriveInfo & a){
	key("addressableSize", "%ld", a.addressable_size());
	key("eraseBlockSize", "%ld", a.erase_block_size());
	key("eraseBlockTime", "%ldus", a.erase_block_time().microseconds());
	key("eraseDeviceTime", "%ldms", a.erase_device_time().milliseconds());
	key("frequency", "%ld", a.frequency());
	key("events", "0x%lX", a.o_events());
	key("flags", "0x%lX", a.o_flags());
	key("pageProgramSize", "%ld", a.page_program_size());
	key("writeBlockCount", "%ld", a.write_block_count());
	key("writeBlockSize", "%ld", a.write_block_size());
	return *this;
}

Printer & Printer::operator << (const sgfx::Bitmap & a){
	sg_size_t i,j;

	sg_color_t color;
	sg_cursor_t y_cursor;
	sg_cursor_t x_cursor;

	if( verbose_level() < verbose_level() ){
		return *this;
	}

	sgfx::Bitmap::api()->cursor_set(&y_cursor, a.bmap(), sg_point(0,0));

	var::String line;
	line << " ";
	for(j=0; j < a.bmap()->area.width; j++){
		if( j % 10 ){
			line << var::String().format("%d", j % 10);
		} else {
			line << (" ");
		}
	}

	key("lines    ", line);

	line.clear();
	for(j=0; j < a.bmap()->area.width; j++){
		line.append("-");
	}
	line.append("--");
	key("start    ", line);

	for(i=0; i < a.bmap()->area.height; i++){
		sg_cursor_copy(&x_cursor, &y_cursor);

		line.clear();
		line.append("|");
		for(j=0; j < a.bmap()->area.width; j++){
			color = sgfx::Bitmap::api()->cursor_get_pixel(&x_cursor);
			line << get_bitmap_pixel_character(color, a.bmap()->bits_per_pixel);
			if( (j < a.bmap()->area.width - 1) && (a.bmap()->bits_per_pixel > 4)){
				line.append(" ");
			}
		}
		line.append("|");
		key(var::String().format("line-%04d", i), line);
		sgfx::Bitmap::api()->cursor_inc_y(&y_cursor);
	}
	line.clear();
	for(j=0; j < a.bmap()->area.width; j++){
		line.append("-");
	}
	line.append("--");
	key("lines end", line);

	return *this;
}

char Printer::get_bitmap_pixel_character(u32 color, u8 bits_per_pixel){
	if( color == 0 ){
		return ' ';
	} else {
		if( bits_per_pixel == 2 ){
			switch(color){
				case 1: return '.';
				case 2: return '+';
				case 3: return '@';
			}
		} else if( bits_per_pixel < 8 ){
			switch(color){
				case 1: return '.';
				case 2: return ',';
				case 3: return '-';
				case 4: return '+';
				case 5: return '=';
				case 6: return '^';
				case 7: return 'x';
				case 8: return 'o';
				case 9: return '*';
				case 10: return '#';
				case 11: return '%';
				case 12: return '$';
				case 13: return 'O';
				case 14: return 'X';
				case 15: return '@';
			}
		} else {
			return '#';
		}
	}
	return '?';
}

u32 Printer::get_bitmap_pixel_color(char c, u8 bits_per_pixel){

	if( bits_per_pixel == 1 ){
		if( c == ' ' ){ return 0; }
		return 1;
	}

	if( bits_per_pixel == 2 ){
		switch(c){
			case ' ': return 0;
			case '.': return 1;
			case ',': return 2;
			case '-':
			default: return 3;
		}
	}

	if( bits_per_pixel == 4 ){
		switch(c){
			case ' ': return 0;
			case '.': return 1;
			case ',': return 2;
			case '-': return 3;
			case '+': return 4;
			case '=': return 5;
			case '^': return 6;
			case 'x': return 7;
			case 'o': return 8;
			case '*': return 9;
			case '#': return 10;
			case '%': return 11;
			case '$': return 12;
			case 'O': return 13;
			case 'X': return 14;
			case '@':
			default: return 15;
		}
	}

	return 255;
}

Printer & Printer::operator << (const sgfx::Cursor & a){
	return *this;
}

Printer & Printer::operator << (const draw::DrawingPoint & a){
	key("x", "%d", a.x());
	key("y", "%d", a.y());
	return *this;
}

Printer & Printer::operator << (const draw::DrawingArea & a){
	key("width", "%d", a.width());
	key("height", "%d", a.height());
	return *this;
}

Printer & Printer::operator << (const draw::DrawingRegion & a){
	key("x", "%d", a.point().x());
	key("y", "%d", a.point().y());
	key("width", "%d", a.area().width());
	key("height", "%d", a.area().height());
	return *this;
}

Printer & Printer::operator << (const sgfx::Point & a){
	key("x", "%d", a.x());
	key("y", "%d", a.y());
	return *this;
}

Printer & Printer::operator << (const sgfx::Region & a){
	key("x", "%d", a.point().x());
	key("y", "%d", a.point().y());
	key("width", "%d", a.area().width());
	key("height", "%d", a.area().height());
	return *this;
}

Printer & Printer::operator << (const sgfx::Area & a){
	key("width", "%d", a.width());
	key("height", "%d", a.height());
	return *this;
}

Printer & Printer::operator << (const sgfx::Pen & a){
	key("color", "%d", a.color());
	key("thickness", "%d", a.color());
	key("o_flags", "0x%x", a.o_flags());
	key("solid", "%d", a.is_solid());
	key("invert", "%d", a.is_invert());
	key("erase", "%d", a.is_erase());
	key("blend", "%d", a.is_blend());
	key("fill", "%d", a.is_fill());
	return *this;
}

Printer & Printer::operator << (const sgfx::Vector & a){
	return *this;
}

Printer & Printer::operator << (const sgfx::VectorPath & a){
	for(u32 i=0; i < a.icon_count(); i++){
		print_open_object(verbose_level(), var::String().format("[%d]", i).cstring());
		{
			*this << sgfx::VectorPathDescription(a.icon_list()[i]);
			print_close_object();
		}
	}
	return *this;
}

Printer & Printer::operator << (const sgfx::VectorPathDescription & a){

	switch( a.type() ){
		case sgfx::VectorPathDescription::NONE:
			key("type", "none");
			break;
		case sgfx::VectorPathDescription::MOVE:
			key("type", "move");
			print_open_object(verbose_level(), "point");
			{
				*this << a.to_move().point;
				print_close_object();
			}
			break;
		case sgfx::VectorPathDescription::LINE:
			key("type", "line");
			print_open_object(verbose_level(), "point");
			{
				*this << a.to_line().point;
				print_close_object();
			}
			break;
		case sgfx::VectorPathDescription::QUADRATIC_BEZIER:
			key("type", "quadratic bezier");
			print_open_object(verbose_level(), "point");
			{
				*this << a.to_quadratic_bezier().point;
				print_close_object();
			}

			print_open_object(verbose_level(), "control");
			{
				*this << a.to_quadratic_bezier().control;
				print_close_object();
			}
			break;
		case sgfx::VectorPathDescription::CUBIC_BEZIER:
			key("type", "cubic bezier");
			print_open_object(verbose_level(), "point");
			{
				*this << a.to_cubic_bezier().point;
				print_close_object();
			}

			print_open_object( verbose_level(), "control0");
			{
				*this << sgfx::Point(a.to_cubic_bezier().control[0]);
				print_close_object();
			}
			print_open_object(verbose_level(), "control1");
			{
				*this << sgfx::Point(a.to_cubic_bezier().control[1]);
				print_close_object();
			}
			break;
		case sgfx::VectorPathDescription::CLOSE:
			key("type", "close");
			break;
		case sgfx::VectorPathDescription::POUR:
			key("type", "pour");
			print_open_object(verbose_level(), "point");
			{
				*this << a.to_pour().point;
				print_close_object();
			}
			break;
	}

	return *this;
}

Printer & Printer::operator << (const TraceEvent & a){
	var::String id;
	chrono::ClockTime clock_time;
	clock_time = a.timestamp();
	switch(a.id()){
		case LINK_POSIX_TRACE_FATAL: id = "fatal"; break;
		case LINK_POSIX_TRACE_CRITICAL: id = "critical"; break;
		case LINK_POSIX_TRACE_WARNING: id = "warning"; break;
		case LINK_POSIX_TRACE_MESSAGE: id = "message"; break;
		case LINK_POSIX_TRACE_ERROR: id = "error"; break;
		default: id = "other"; break;
	}
	key("timestamp", F32U ".%06ld", clock_time.seconds(), clock_time.nanoseconds()/1000UL);
	key("id", "%s", id.cstring());
	key("thread", "%d", a.thread_id());
	key("pid", "%d", a.pid());
	key("programAddress", "0x%lX", a.program_address());
	key("message", a.message().cstring());
	return *this;
}

Printer & Printer::operator << (const appfs_file_t & a){
	key("name", "%s", a.hdr.name);
	key("id", "%s", a.hdr.id);
	key("mode", "0%o", a.hdr.mode);
	key("version", "%d.%d", a.hdr.version >> 8, a.hdr.version & 0xff);
	key("startup", "%p", a.exec.startup);
	key("codeStart", "%p", a.exec.code_start);
	key("codeSize", "%p", a.exec.code_size);
	key("ramStart", "%p", a.exec.ram_start);
	key("ramSize", "%ld", a.exec.ram_size);
	key("dataSize", "%ld", a.exec.data_size);
	key("oFlags", "0x%lX", a.exec.o_flags);
	key("signature", "0x%lX", a.exec.signature);
	return *this;
}

Printer & Printer::operator << (const AppfsFileAttributes & a){
	key("name", a.name().cstring());
	key("id", a.id().cstring());
	key("version", "%d.%d", a.version() >> 8, a.version() & 0xff);
	key("flash", a.is_flash() ? "true" : "false");
	key("codeExternal", a.is_code_external() ? "true" : "false");
	key("dataExternal", a.is_data_external() ? "true" : "false");
	key("codeTightlyCoupled", a.is_code_tightly_coupled() ? "true" : "false");
	key("dataTightlyCoupled", a.is_data_tightly_coupled() ? "true" : "false");
	key("startup", a.is_startup() ? "true" : "false");
	key("unique", a.is_unique() ? "true" : "false");
	key("ramSize", "%ld", a.ram_size());
	return *this;
}

bool Printer::update_progress(int progress, int total){
	const u32 width = m_progress_width;

	if( verbose_level() >= Printer::INFO ){

		if( (m_progress_state == 0) && total ){

			//only print the key once with total == -1
			print(Printer::INFO, m_progress_key.cstring(), nullptr);
			if( total != -1 ){
				if( m_o_flags & print_value_quotes ){
					print_final("\"");
				}
				if( (m_o_flags & print_simple_progress) == 0 ){
					for(u32 i=0; i < width; i++){
						print_final(".");
					}
					for(u32 i = 0; i < width; i++){
						print_final("\b"); //backspace
					}
				}
			}
			m_progress_state++;
			fflush(stdout);
		}

		if( m_progress_state	> 0 ){

			if( total == ProgressCallback::indeterminate_progress_total() ){
				var::String output;
				var::String animation = "-\\|/";
				if( (m_o_flags & print_value_quotes) && (m_progress_state == 1) ){
					print_final("\"");
				}
				m_progress_state++;

				if( (m_o_flags & print_simple_progress) == 0 ){
					output.format(
								"%c" F32U,
								animation.at(m_progress_state % animation.length()),
								progress
								);
					print_final(output.cstring());
					for(u32 i = 0; i < output.length(); i++){
						print_final("\b"); //backspace
					}
				} else {
					print_final(F32U, m_progress_state-1);
				}

			} else {

				while( (total != 0) &&
						 (m_progress_state <= (progress*width+total/2)/total)
						 ){
					print_final("#");
					m_progress_state++;
					fflush(stdout);
				}

				if( (progress >= total) || (total == 0) ){
					m_progress_state = 0;
				}
			}
		}
		if( total == 0 ){
			if( (m_o_flags & print_no_progress_newline) == 0 ){
				print_final("\n");
			}
			if( m_o_flags & print_value_quotes ){
				print_final("\"");
			}
		}
	}

	return false;
}

Printer & Printer::key(const var::String & key, const var::String & a){
	print(level_info,
			key.is_empty() ? nullptr : key.cstring(),
			a.is_empty() ? nullptr : a.cstring()
			);
	return *this;
}

Printer & Printer::key(const var::String & key, const char * fmt, ...){
	va_list list;
	va_start(list, fmt);
	print(level_info,
			key.is_empty() ? nullptr : key.cstring(),
			var::String().vformat(fmt, list).cstring()
			);
	va_end(list);
	return *this;
}

Printer & Printer::debug(const char * fmt, ...){
	va_list list;
	va_start(list, fmt);
	print(level_debug, "debug", var::String().vformat(fmt, list).cstring());
	va_end(list);
	return *this;
}

Printer & Printer::info(const char * fmt, ...){
	va_list list;
	va_start(list, fmt);
	print(level_info, "info", var::String().vformat(fmt, list).cstring());
	va_end(list);
	return *this;
}


Printer & Printer::message(const char * fmt, ...){
	va_list list;
	va_start(list, fmt);
	print(level_message, "message", var::String().vformat(fmt, list).cstring());
	va_end(list);
	return *this;
}

Printer & Printer::warning(const char * fmt, ...){
	va_list list;
	if( flags() & print_yellow_warnings ){ set_color_code(color_code_yellow); }
	va_start(list, fmt);
	print(level_warning, "warning", var::String().vformat(fmt, list).cstring());
	va_end(list);
	if( flags() & print_yellow_warnings ){ clear_color_code(); }
	return *this;
}

Printer & Printer::error(const char * fmt, ...){
	va_list list;
	va_start(list, fmt);
	print(level_error, "error", var::String().vformat(fmt, list).cstring());
	va_end(list);
	return *this;
}

Printer & Printer::error(
		const api::Result result,
		u32 line_number
		){
	error("returned %d with error number %d at line %d",
			result.return_value(),
			result.error_number(),
			line_number);
	return *this;
}

Printer & Printer::fatal(const char * fmt, ...){
	va_list list;
	va_start(list, fmt);
	print(level_fatal, "fatal", var::String().vformat(fmt, list).cstring());
	va_end(list);
	return *this;
}

Printer & Printer::trace(const char * function, int line, const var::String & message){

	if( verbose_level() == level_trace ){
		print_final(">> trace %s:%d %s\n",
						function,
						line,
						message.cstring()
						);
	}
	return *this;
}




