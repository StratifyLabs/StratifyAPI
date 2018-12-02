#ifndef SAPI_SYS_PRINTER_HPP
#define SAPI_SYS_PRINTER_HPP

#include <cstdarg>
#include "../api/SysObject.hpp"
#include "../var/ConstString.hpp"
#include "../sys/Appfs.hpp"
#include "../sys/Trace.hpp"
#include "../sys/ProgressCallback.hpp"


namespace var {
class DataInfo;
class Data;
class String;
class Token;
class JsonObject;
class JsonArray;
class JsonValue;
template<typename T> class Vector;
}

namespace chrono{
class ClockTime;
class MicroTime;
class Time;
}

namespace sgfx{
class Point;
class Dim;
class Region;
class Bitmap;
class Pen;
class Vector;
class Cursor;
class VectorPath;
class VectorPathDescription;
}


namespace hal{
class I2CAttributes;
class UartAttributes;
}

namespace sys {

class TaskInfo;
class SysInfo;
class Cli;


#define PRINTER_TRACE(printer) (printer.print("\n%s():%d", __FUNCTION__, __LINE__))
#define PRINTER_TRACE_ERROR(printer, x) int printer_result = x; if( printer_result < 0 ) printer.print("\nError: %s():%d (%d)", __FUNCTION__, __LINE__, x, y)

class PrinterTermination {
public:

private:
};

class Printer : public api::SysWorkObject {
public:
	Printer();
	~Printer();

	enum {
		COLOR_CODE_DEFAULT = 39,
		COLOR_CODE_BLACK = 30,
		COLOR_CODE_RED = 31,
		COLOR_CODE_GREEN = 32,
		COLOR_CODE_YELLOW = 33,
		COLOR_CODE_BLUE = 34,
		COLOR_CODE_MAGENTA = 35,
		COLOR_CODE_CYAN = 36,
		COLOR_CODE_LIGHT_GRAY = 37,
		COLOR_CODE_DARK_GRAY = 90,
		COLOR_CODE_LIGHT_RED = 91,
		COLOR_CODE_LIGHT_GREEN = 92,
		COLOR_CODE_LIGHT_YELLOW = 93,
		COLOR_CODE_LIGHT_BLUE = 94
	};

	static u32 color_code(const var::ConstString & color);

	void set_color_code(u32 code);
	void clear_color_code();

	static PrinterTermination close(){
		return PrinterTermination();
	}

	Printer & operator << (const PrinterTermination & printer_termination){
		m_indent--;
		return *this;
	}

	Printer & operator << (const var::Data & a);
	Printer & operator << (const var::DataInfo & a);
	Printer & operator << (const var::String & a);
	Printer & operator << (const var::Token & a);
	Printer & operator << (const var::JsonObject & a);
	Printer & operator << (const var::JsonArray & a);
	Printer & operator << (const var::Vector<var::String> & a);
	Printer & operator << (const Cli & a);
	Printer & operator << (const appfs_file_t & a);
	Printer & operator << (const AppfsFileAttributes & a);
	Printer & operator << (const TraceEvent & a);

	Printer & operator << (const sgfx::Bitmap & a);
	Printer & operator << (const sgfx::Cursor & a);
	Printer & operator << (const sgfx::Point & a);
	Printer & operator << (const sgfx::Region & a);
	Printer & operator << (const sgfx::Dim & a);
	Printer & operator << (const sgfx::Pen & a);
	Printer & operator << (const sgfx::Vector & a);
	Printer & operator << (const sgfx::VectorPath & a);
	Printer & operator << (const sgfx::VectorPathDescription & a);


	Printer & operator << (const sys::SysInfo & a);
	Printer & operator << (const sys::TaskInfo & a);

	Printer & operator << (const chrono::ClockTime & a);
	Printer & operator << (const chrono::MicroTime & a);
	Printer & operator << (const chrono::Time & a);
	Printer & operator << (s32 a);
	Printer & operator << (u32 a);
	Printer & operator << (s16 a);
	Printer & operator << (u16 a);
	Printer & operator << (s8 a);
	Printer & operator << (u8 a);
	Printer & operator << (void * a);
	Printer & operator << (const char * a);
	Printer & operator << (float a);

	virtual Printer & message(const char * fmt, ...);
	virtual Printer & warning(const char * fmt, ...);
	virtual Printer & error(const char * fmt, ...);
	virtual Printer & fatal(const char * fmt, ...);
	virtual Printer & key(const var::ConstString & key, const char * fmt, ...);
	virtual Printer & key(const var::ConstString & key, const var::String & a);
	virtual Printer & key(const var::ConstString & key, const var::JsonValue & a);

	void print(const char * fmt, ...);


	enum {
		PRINT_HEX /*! Print hex data */ = (1<<0),
		PRINT_UNSIGNED /*! Print unsigned integers */ = (1<<1),
		PRINT_SIGNED /*! Printd signed integers */ = (1<<2),
		PRINT_CHAR /*! Print Characters */ = (1<<3),
		PRINT_8 /*! Print as 8 bit values (default) */ = 0,
		PRINT_16 /*! Print as 16 bit values */ = (1<<4),
		PRINT_32 /*! Print as 32 bit values */ = (1<<5),
	};

	void set_flags(u32 value){ m_o_flags = value; }

	Printer & open_object(const var::ConstString & key);
	void close_object(){ m_indent--; }


	const sys::ProgressCallback * progress_callback() const {
		return &m_progress_callback;
	}

	static bool update_progress_callback(void * context, int progress, int total){
		Printer * printer = (Printer*)context;
		return printer->update_progress(progress, total);
	}

	bool update_progress(int progress, int total);

protected:

	void print_indentation();

	virtual void print_indented(const var::ConstString & key, const char * fmt, ...);
	virtual void vprint_indented(const var::ConstString & key, const char * fmt, va_list list);

	void vprint(const char * fmt, va_list list);

private:

	ProgressCallback m_progress_callback;
	u16 m_progress_width;
	u16 m_indent;
	bool m_is_json;
	u32 m_o_flags;

};


}

#endif // SAPI_SYS_PRINTER_HPP
