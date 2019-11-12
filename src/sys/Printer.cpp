
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
	m_key_count = 0;
	m_verbose_level = INFO;
	m_container.push_back(CONTAINER_ARRAY);
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
		print("\033[1;%dm", code);
	}
#endif
}

void Printer::clear_format_code(u32 code){
#if defined __link
	if( api::ApiInfo::is_macosx() || is_bash() ){
		print("\033[1;2%dm", code);
	}
#endif
}

void Printer::set_color_code(u32 code){

#if defined __link
	if( api::ApiInfo::is_macosx() || is_bash() ){
		print("\033[1;%dm", code);
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

void Printer::clear_color_code(){
#if defined __link
	if( api::ApiInfo::is_macosx() || is_bash() ){
		printf("\033[0m");
	} else {
		set_color_code(COLOR_CODE_DEFAULT);
	}
#endif
}

u32 Printer::color_code(const var::ConstString & color){
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


Printer::~Printer(){
	print("\n");
}

void Printer::print_indentation(){
	s32 indent_count = m_container.count() - 1;
	if( indent_count < 0 ){
		indent_count = 0;
	}
	for(u16 i = 0; i < indent_count; i++){
		print("   ");
	}
}

void Printer::vprint_indented(const var::ConstString & key, const char * fmt, va_list list){
	print("\n");
	print_indentation();

	u8 container = current_container();

	if( container == CONTAINER_ARRAY ){
		print("- ");
	}

	if( !key.is_empty() ){
		if( m_o_flags & PRINT_BOLD_KEYS ){ set_format_code(FORMAT_BOLD); }
		if( m_o_flags & PRINT_CYAN_KEYS ){ set_color_code(COLOR_CODE_CYAN); }
		if( m_o_flags & PRINT_YELLOW_KEYS ){ set_color_code(COLOR_CODE_YELLOW); }
		if( m_o_flags & PRINT_MAGENTA_KEYS ){ set_color_code(COLOR_CODE_MAGENTA); }
		print("%s: ", key.cstring());
		if( m_o_flags & PRINT_BOLD_KEYS ){ clear_format_code(FORMAT_BOLD); }
		if( m_o_flags & (PRINT_CYAN_KEYS | PRINT_YELLOW_KEYS | PRINT_MAGENTA_KEYS) ){ clear_color_code(); }
	}

	if( m_o_flags & PRINT_BOLD_VALUES ){ set_format_code(FORMAT_BOLD); }
	if( m_o_flags & PRINT_GREEN_VALUES){ set_color_code(COLOR_CODE_GREEN); }
	if( m_o_flags & PRINT_YELLOW_VALUES){ set_color_code(COLOR_CODE_YELLOW); }
	if( m_o_flags & PRINT_RED_VALUES){ set_color_code(COLOR_CODE_RED); }
	vprint(fmt, list);
	if( m_o_flags & (PRINT_GREEN_VALUES | PRINT_YELLOW_VALUES | PRINT_RED_VALUES) ){ clear_color_code(); }
	if( m_o_flags & PRINT_BOLD_VALUES ){ clear_format_code(FORMAT_BOLD); }

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
	open_object(a.name(), current_level());
	{
		key("publisher", "%s", a.publisher().cstring());
		open_object("arguments", current_level());
		{
			for(u32 i = 0; i < a.count(); i++){
				key(0, "%s", a.at(i).cstring());
			}
			close_object();
		}
		close_object();
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


Printer & Printer::operator << (const var::DataReference & a){
	u32 o_flags = m_o_flags;
	const s8 * ptrs8 = a.to_const_s8();
	const s16 * ptrs16 = a.to_const_s16();
	const s32 * ptrs32 = a.to_const_s32();
	const u8 * ptru8 = a.to_const_u8();
	const u16 * ptru16 = a.to_const_u16();
	const u32 * ptru32 = a.to_const_u32();

	if( verbose_level() < current_level() ){
		return *this;
	}

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
	for(i=0; i < s; i++){
		print("\n");
		print_indentation();
		print("[%04d]=", i);
		if( o_flags & PRINT_HEX ){
			if( o_flags & PRINT_32 ){
				print(F32X, ptru32[i]);
			} else if( o_flags & PRINT_16 ){
				print("%X", ptru16[i]);
			} else if( o_flags & PRINT_BLOB ){
				for(u32 j=0; j < 16; j++){
					print("%02X", ptru8[i*16+j]);
					if( j < 15 ){
						print(" ");
					}
					bytes_printed++;
					if( bytes_printed == a.size() ){
						break;
					}
				}
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
			} else if( o_flags & PRINT_BLOB ){
				for(u32 j=0; j < 16; j++){
					print("%u", ptru8[i*16+j]);
					if( j < 15 ){
						print(" ");
					}
				}
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
			} else if( o_flags & PRINT_BLOB ){
				for(u32 j=0; j < 16; j++){
					print("%d", ptru8[i*16+j]);
					if( j < 15 ){
						print(" ");
					}
				}
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
	}

	return *this;
}

Printer & Printer::operator << (const var::Datum & a){
	*this << a.to_json();
	return *this;
}

Printer & Printer::operator << (const var::String & a){
	return key(0, a);
}

Printer & Printer::operator << (const char * a){
	return key(0, a);
}

Printer & Printer::operator << (const var::Tokenizer & a){
	for(u32 i=0; i < a.count(); i++){
		print("\n");
		print_indentation();
		print("[%04d]=", i);
		print("%s", a.at(i).cstring());
	}
	return *this;
}

Printer & Printer::operator << (const var::JsonObject & a){

	var::Vector<var::String> keys = a.keys();
	for(u32 i=0; i < keys.count(); i++){
		var::JsonValue entry = a.at(arg::JsonKey(keys.at(i)));
		if( entry.is_object() ){
			open_object(keys.at(i), current_level());
			*this << entry.to_object();
			close_object();
		} else if( entry.is_array() ){
			open_object(keys.at(i), current_level());
			*this << entry.to_array();
			close_object();
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
		key.format("[%d]", i);
		if( entry.is_object() ){
			open_object(key, current_level());
			*this << entry.to_object();
			close_object();
		} else if( entry.is_array() ){
			open_array(key, current_level());
			*this << entry.to_array();
			close_array();
		} else {
			this->key(key, entry.to_string().cstring());
		}
	}
	return *this;
}

Printer & Printer::key(const var::ConstString & key, const var::JsonValue & a){

	if(a.is_object()){
		open_object(key, current_level() );
		*this << a.to_object();
		close_object();
	} else if( a.is_array() ){
		open_object(key, current_level() );
		*this << a.to_array();
		close_object();
	} else {
		this->key(key, a.to_string());
	}
	return *this;
}

int Printer::set_verbose_level(const var::ConstString & level){
	if( level == "debug" ){ set_verbose_level(DEBUG); return 0; }
	if( level == "info" ){ set_verbose_level(Printer::INFO); return 0; }
	if( level == "message" ){ set_verbose_level(Printer::MESSAGE); return 0; }
	if( level == "warning" ){ set_verbose_level(Printer::WARNING); return 0; }
	if( level == "error" ){ set_verbose_level(Printer::ERROR); return 0; }
	if( level == "fatal" ){ set_verbose_level(Printer::FATAL); return 0; }
	return -1;
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
	key("id", "%ld", a.id());
	key("pid", "%ld", a.pid());
	key("memorySize", "%ld", a.memory_size());
	key("stack", "0x%lX", a.stack());
	key("stackSize", "%ld", a.stack_size());
	key("priority", "%ld", a.priority());
	key("priorityCeiling", "%ld", a.priority_ceiling());
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

	if( verbose_level() < current_level() ){
		return *this;
	}


	sgfx::Bitmap::api()->cursor_set(&y_cursor, a.bmap(), sg_point(0,0));

	key(var::String().format("lines    "), "");
	print(" ");
	for(j=0; j < a.bmap()->area.width; j++){
		if( j % 10 ){
			print("%d", j % 10);
		} else {
			print(" ");
		}
	}

	key(var::String().format("start    "), "");
	for(j=0; j < a.bmap()->area.width; j++){
		print("-");
	}
	print("--");
	for(i=0; i < a.bmap()->area.height; i++){
		sg_cursor_copy(&x_cursor, &y_cursor);

		key(var::String().format("line-%04d", i), "");
		print("|");
		for(j=0; j < a.bmap()->area.width; j++){
			color = sgfx::Bitmap::api()->cursor_get_pixel(&x_cursor);
			print_bitmap_pixel(color, a.bmap()->bits_per_pixel);
			if( (j < a.bmap()->area.width - 1) && (a.bmap()->bits_per_pixel > 4)){
				print(" ");
			}
		}
		print("|");
		sgfx::Bitmap::api()->cursor_inc_y(&y_cursor);
	}
	key(var::String().format("lines end", i), "");
	for(j=0; j < a.bmap()->area.width; j++){
		print("-");
	}
	print("--");

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

void Printer::print_bitmap_pixel(u32 color, u8 bits_per_pixel){
	print("%c", get_bitmap_pixel_character(color, bits_per_pixel));
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
	return *this;
}

Printer & Printer::operator << (const sgfx::Vector & a){
	return *this;
}

Printer & Printer::operator << (const sgfx::VectorPath & a){
	for(u32 i=0; i < a.icon_count(); i++){
		open_array(var::String().format("[%d]", i), current_level());
		{
			*this << sgfx::VectorPathDescription(a.icon_list()[i]);
			close_object();
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
			open_object("point", current_level());
			{
				*this << a.to_move().point;
				close_object();
			}
			break;
		case sgfx::VectorPathDescription::LINE:
			key("type", "line");
			open_object("point", current_level());
			{
				*this << a.to_line().point;
				close_object();
			}
			break;
		case sgfx::VectorPathDescription::QUADRATIC_BEZIER:
			key("type", "quadratic bezier");
			open_object("point", current_level());
			{
				*this << a.to_quadratic_bezier().point;
				close_object();
			}

			open_object("control", current_level());
			{
				*this << a.to_quadratic_bezier().control;
				close_object();
			}
			break;
		case sgfx::VectorPathDescription::CUBIC_BEZIER:
			key("type", "cubic bezier");
			open_object("point", current_level());
			{
				*this << a.to_cubic_bezier().point;
				close_object();
			}

			open_object("control0", current_level());
			{
				*this << sgfx::Point(a.to_cubic_bezier().control[0]);
				close_object();
			}
			open_object("control1", current_level());
			{
				*this << sgfx::Point(a.to_cubic_bezier().control[1]);
				close_object();
			}
			break;
		case sgfx::VectorPathDescription::CLOSE:
			key("type", "close");
			break;
		case sgfx::VectorPathDescription::POUR:
			key("type", "pour");
			open_object("point", current_level());
			{
				*this << a.to_pour().point;
				close_object();
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
			key(m_progress_key, "");
			if( total != -1 ){
				if( (m_o_flags & PRINT_SIMPLE_PROGRESS) == 0 ){
					for(u32 i=0; i < width; i++){
						print(".");
					}
					for(u32 i = 0; i < width; i++){
						print("\b"); //backspace
					}
				}
			}
			m_progress_state++;
			fflush(stdout);
		}

		if( m_progress_state	> 0 ){

			if( total == ProgressCallback::indeterminate_progress_total() ){
				var::String output;
				if( (m_o_flags & PRINT_SIMPLE_PROGRESS) == 0 ){
					output.format("?" F32U, progress);
					print(output.cstring());
					for(u32 i = 0; i < output.length(); i++){
						print("\b"); //backspace
					}
				} else {
					print("?");
				}

			} else {

				while( (total != 0) && (m_progress_state <= (progress*width+total/2)/total) ){
					print("#");
					m_progress_state++;
					fflush(stdout);
				}

				if( (progress >= total) || (total == 0) ){
					m_progress_state = 0;
				}
			}
		}
	}

	return false;
}

Printer & Printer::open_object(const var::ConstString & key, enum verbose_level level){
	if( verbose_level() >= level ){
		if( m_o_flags & PRINT_BOLD_OBJECTS ){ set_format_code(FORMAT_BOLD); }
		print_indented(key, "");
		if( m_o_flags & PRINT_BOLD_OBJECTS ){ clear_format_code(FORMAT_BOLD); }
	}
	m_container.push_back(CONTAINER_OBJECT | (level << 8));
	m_indent++;
	return *this;
}

Printer & Printer::open_array(const var::ConstString & key, enum verbose_level level){
	if( verbose_level() >= level ){
		if( m_o_flags & PRINT_BOLD_ARRAYS ){ set_format_code(FORMAT_BOLD); }
		print_indented(key, "");
		if( m_o_flags & PRINT_BOLD_ARRAYS ){ clear_format_code(FORMAT_BOLD); }
	}
	m_container.push_back(CONTAINER_ARRAY | (level << 8));
	m_indent++;
	return *this;
}

Printer & Printer::key(const var::ConstString & key, const var::String & a){
	this->key(key, "%s", a.cstring());
	return *this;
}

Printer & Printer::key(const var::ConstString & key, const char * fmt, ...){
	if( verbose_level() >= current_level() ){
		va_list list;
		va_start(list, fmt);
		vprint_indented(key, fmt, list);
		va_end(list);
	}
	return *this;
}

Printer & Printer::debug(const char * fmt, ...){
	if( verbose_level() == DEBUG ){
		va_list list;
		va_start(list, fmt);
		vprint_indented("debug", fmt, list);
		va_end(list);
	}
	return *this;

}

Printer & Printer::info(const char * fmt, ...){
	if( verbose_level() >= INFO ){
		va_list list;
		va_start(list, fmt);
		vprint_indented("info", fmt, list);
		va_end(list);
	}
	return *this;
}


Printer & Printer::message(const char * fmt, ...){
	if( verbose_level() >= MESSAGE ){
		va_list list;
		va_start(list, fmt);
		vprint_indented("message", fmt, list);
		va_end(list);
	}
	return *this;
}

Printer & Printer::warning(const char * fmt, ...){
	if( verbose_level() >= WARNING ){
		va_list list;
		if( flags() & PRINT_YELLOW_WARNINGS ){ set_color_code(COLOR_CODE_YELLOW); }
		print_indented("warning", "");
		if( flags() & PRINT_YELLOW_WARNINGS ){ clear_color_code(); }
		va_start(list, fmt);
		vprint(fmt, list);
		va_end(list);
	}
	return *this;
}

Printer & Printer::error(const char * fmt, ...){
	if( verbose_level() >= ERROR ){
		va_list list;
		if( flags() & PRINT_RED_ERRORS ){ set_color_code(COLOR_CODE_RED); }
		print_indented("error", "");
		if( flags() & PRINT_RED_ERRORS ){ clear_color_code(); }
		va_start(list, fmt);
		vprint(fmt, list);
		va_end(list);
	}
	return *this;
}

Printer & Printer::error(
		const api::Result result,
		u32 line_number
		){
	error(
				"returned %d with error number %d at line %d",
				result.return_value(),
				result.error_number(),
				line_number
			);
	return *this;
}

Printer & Printer::fatal(const char * fmt, ...){
	va_list list;
	va_start(list, fmt);
	vprint_indented("fatal", fmt, list);
	va_end(list);
	return *this;
}
