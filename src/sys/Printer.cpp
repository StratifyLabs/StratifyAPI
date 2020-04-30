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
unsigned int sys::Printer::m_default_color = static_cast<unsigned int>(-1);
#endif


using namespace sys;

Printer::Printer() : m_progress_callback(Printer::update_progress_callback, this){
	m_o_flags = print_8 | print_hex;
	m_indent = 0;
	m_progress_width = 50;
	m_progress_state = 0;
	m_verbose_level = level_info;
	m_progress_key = "progress";
#if defined __win32
	if( m_default_color == static_cast<unsigned int>(-1) ){
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
	WORD color = static_cast<WORD>(m_default_color);
	switch(code){
		case color_code_default: color = static_cast<WORD>(m_default_color); break;
		case color_code_black: color = 0; break;
		case color_code_red: color = FOREGROUND_RED; break;
		case color_code_green: color = FOREGROUND_GREEN; break;
		case color_code_yellow: color = FOREGROUND_RED | FOREGROUND_GREEN; break;
		case color_code_blue: color = FOREGROUND_BLUE; break;
		case color_code_magenta: color = FOREGROUND_BLUE | FOREGROUND_RED; break;
		case color_code_cyan: color = FOREGROUND_BLUE | FOREGROUND_GREEN; break;
		case color_code_light_gray: color = FOREGROUND_INTENSITY; break;
		case color_code_dark_gray: color = 0; break;
		case color_code_light_red: color = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
		case color_code_light_green: color = FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
		case color_code_light_yellow: color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
		case color_code_light_blue: color = FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
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
		set_color_code(color_code_default);
	}
#endif
}

u32 Printer::color_code(const var::String & color){
	var::String color_upper(color);
	color_upper.to_upper();
	if( color_upper == "BLACK" ){ return color_code_black; }
	if( color_upper == "RED" ){ return color_code_red; }
	if( color_upper == "GREEN" ){ return color_code_green; }
	if( color_upper == "YELLOW" ){ return color_code_yellow; }
	if( color_upper == "BLUE" ){ return color_code_blue; }
	if( color_upper == "MAGENTA" ){ return color_code_magenta; }
	if( color_upper == "CYAN" ){ return color_code_cyan; }
	if( color_upper == "LIGHT GRAY" ){ return color_code_light_gray; }
	if( color_upper == "DARK GRAY" ){ return color_code_dark_gray; }
	if( color_upper == "LIGHT RED" ){ return color_code_light_red; }
	if( color_upper == "LIGHT GREEN" ){ return color_code_light_green; }
	if( color_upper == "LIGHT YELLOW" ){ return color_code_light_yellow; }
	if( color_upper == "LIGHT BLUE" ){ return color_code_light_blue; }

	return color_code_default;
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

Printer & Printer::operator << (const api::Result & a){
	key("returnValue", "%d", a.return_value());
	key("errorNumber", "%d", a.error_number());
	return *this;
}


Printer & Printer::operator << (const char * a){
	return key(nullptr, a);
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


bool Printer::update_progress(int progress, int total){
	const u32 width = m_progress_width;

	if( verbose_level() >= Printer::level_info ){

		if( (m_progress_state == 0) && total ){

			//only print the key once with total == -1
			print(Printer::level_info, m_progress_key.cstring(), nullptr);
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

Printer & Printer::key(const var::String & key, bool value){
	print(level_info, key.cstring(), value ? "true" : "false");
	return *this;
}

Printer & Printer::key(const var::String & key, const var::String & a){
	print(level_info, key.cstring(), a.cstring());
	return *this;
}

Printer & Printer::key(const var::String & key, const char * fmt, ...){
	va_list list;
	va_start(list, fmt);
	var::String formatted;
	formatted.vformat(fmt, list);
	print(level_info,
			key.is_empty() ? nullptr : key.cstring(),
			formatted.is_empty() ? nullptr : formatted.cstring()
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




