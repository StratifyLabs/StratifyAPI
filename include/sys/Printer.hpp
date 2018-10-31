#ifndef SAPI_SYS_PRINTER_HPP
#define SAPI_SYS_PRINTER_HPP

#include <cstdarg>
#include "../api/SysObject.hpp"
#include "../var/ConstString.hpp"
#include "../sys/Appfs.hpp"
#include "../sys/Trace.hpp"


namespace var {
class DataInfo;
class Data;
class String;
class Token;
template<typename T> class Vector;
}

namespace chrono{
class ClockTime;
class MicroTime;
class Time;
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

class Printer : public api::SysWorkObject {
public:
	Printer();
	~Printer();

	void set_color_code(u32 code);
	void clear_color_code();

	Printer & operator << (const var::Data & a);
	Printer & operator << (const var::DataInfo & a);
	Printer & operator << (const var::String & a);
	Printer & operator << (const var::Token & a);
	Printer & operator << (const var::Vector<var::String> & a);
	Printer & operator << (const Cli & a);
	Printer & operator << (const appfs_file_t & a);
	Printer & operator << (const AppfsFileAttributes & a);
	Printer & operator << (const TraceEvent & a);

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

	u16 m_progress_width;
	u16 m_indent;
	bool m_is_json;
	u32 m_o_flags;

};


}

#endif // SAPI_SYS_PRINTER_HPP
