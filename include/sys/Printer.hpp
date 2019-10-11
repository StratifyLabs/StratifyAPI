#ifndef SAPI_SYS_PRINTER_HPP_
#define SAPI_SYS_PRINTER_HPP_

#include <cstdarg>
#include "../api/SysObject.hpp"
#include "../var/ConstString.hpp"
#include "../sys/Appfs.hpp"
#include "../sys/Trace.hpp"
#include "../sys/ProgressCallback.hpp"
#include "../var/Vector.hpp"

#if defined __win32
#undef ERROR
#endif

namespace api {
class Result;
}

namespace var {
class DataInfo;
class Data;
class Datum;
class String;
class Tokenizer;
class JsonObject;
class JsonArray;
class JsonValue;
template<typename T> class Vector;
template<typename T> class Ring;
}

namespace chrono{
class ClockTime;
class Microseconds;
class Time;
}

namespace sgfx{
class Point;
class Area;
class Region;
class Bitmap;
class Pen;
class Vector;
class Cursor;
class VectorPath;
class VectorPathDescription;
}

namespace draw {
class DrawingPoint;
class DrawingArea;
class DrawingRegion;
class DrawingAttributes;
}


namespace hal{
class I2CAttributes;
class UartAttributes;
class DriveInfo;
}

namespace fs {
class Stat;
}

namespace sys {

class TaskInfo;
class SysInfo;
class Cli;
class Info;
class AppfsInfo;


#define PRINTER_TRACE(printer) (printer.print("\n%s():%d", __FUNCTION__, __LINE__))
#define PRINTER_TRACE_ERROR(printer, x) int printer_result = x; if( printer_result < 0 ) printer.print("\nError: %s():%d (%d)", __FUNCTION__, __LINE__, x, y)

class PrinterTermination {
public:

private:
};


/*! \brief Printer Class
 * \details The Printer class is used for printing
 * YAML formatted messages to the standard output.
 * The printing mechanisms are virtual and can
 * be re-implemented to print to any output
 * (or multiple outputs).
 *
 * The Printer class supports the `<<` operator
 * for a growing list of StratifyAPI types.
 *
 * For example, the following code will
 * print the system information as a
 * YAML object.
 *
 * ```
 * //md2code:include
 * #include <sapi/sys.hpp>
 * ```
 *
 * ```
 * //md2code:main
 * Printer p;
 * p.open_object("System Information") << SysInfo::get() << p.close();
 * ```
 *
 */
class Printer : public api::WorkObject {
public:
	Printer();
	~Printer();

	enum {
		COLOR_CODE_DEFAULT = 39,
		COLOR_CODE_BLACK = 30,
		COLOR_CODE_RED = 31, //RED
		COLOR_CODE_GREEN = 32, //GREEN
		COLOR_CODE_YELLOW = 33,
		COLOR_CODE_BLUE = 34, //BLUE
		COLOR_CODE_MAGENTA = 35,
		COLOR_CODE_CYAN = 36, //BLUE | GREEN
		COLOR_CODE_LIGHT_GRAY = 37,
		COLOR_CODE_DARK_GRAY = 90,
		COLOR_CODE_LIGHT_RED = 91,
		COLOR_CODE_LIGHT_GREEN = 92,
		COLOR_CODE_LIGHT_YELLOW = 93,
		COLOR_CODE_LIGHT_BLUE = 94
	};

	enum {
		FORMAT_NORMAL = 0,
		FORMAT_BOLD = 1,
		FORMAT_DIM = 2,
		FORMAT_UNDERLINE = 4,
		FORMAT_BLINK = 5,
		FORMAT_INVERTED = 7,
		FORMAT_HIDDEN = 8
	};

	static u32 color_code(const var::ConstString & color);

	void set_format_code(u32 code);
	void clear_format_code(u32 code);

	void set_color_code(u32 code);
	void clear_color_code();
#if defined __win32
	static unsigned int m_default_color;
#endif

	Printer & operator << (const api::Result & a);

	/*! \details Prints a var::DataReference object. */
	Printer & operator << (const var::DataReference & a);
	/*! \details Prints a var::Datum object. */
	Printer & operator << (const var::Datum & a);

#if !defined __link
	/*! \details Prints a var::DataInfo object. */
	Printer & operator << (const var::DataInfo & a);
#endif

	/*! \details Prints a var::String object. */
	Printer & operator << (const var::String & a);
	/*! \details Prints a var::Tokenizer object. */
	Printer & operator << (const var::Tokenizer & a);
	/*! \details Prints a var::JsonObject object. */
	Printer & operator << (const var::JsonObject & a);
	/*! \details Prints a var::JsonArray object. */
	Printer & operator << (const var::JsonArray & a);
	/*! \details Prints a string list object. */
	Printer & operator << (const var::Vector<var::String> & a);
	/*! \details Prints a var::Ring<u32> object. */
	Printer & operator << (const var::Ring<u32> & a);
	/*! \details Prints a var::Ring<s32> object. */
	Printer & operator << (const var::Ring<s32> & a);
	/*! \details Prints a var::Ring<u16> object. */
	Printer & operator << (const var::Ring<u16> & a);
	/*! \details Prints a var::Ring<s16> object. */
	Printer & operator << (const var::Ring<s16> & a);
	/*! \details Prints a var::Ring<u8> object. */
	Printer & operator << (const var::Ring<u8> & a);
	/*! \details Prints a var::Ring<s8> object. */
	Printer & operator << (const var::Ring<s8> & a);

	/*! \details Prints a sys::Cli object. */
	Printer & operator << (const Cli & a);
	/*! \details Prints a sys::appfs_file_t object. */
	Printer & operator << (const appfs_file_t & a);
	/*! \details Prints a sys::AppfsFileAttributes object. */
	Printer & operator << (const AppfsFileAttributes & a);
	/*! \details Prints a sys::TraceEvent object. */
	Printer & operator << (const TraceEvent & a);

	/*! \details Prints a sys::SysInfo object. */
	Printer & operator << (const sys::SysInfo & a);
	/*! \details Prints a sys::AppfsInfo object. */
	Printer & operator << (const sys::AppfsInfo & a);
	/*! \details Prints a sys::TaskInfo object. */
	Printer & operator << (const sys::TaskInfo & a);
	/*! \details Prints a file::Info object. */
	Printer & operator << (const fs::Stat & a);

	/*! \details Prints a sgfx::Bitmap object. */
	Printer & operator << (const sgfx::Bitmap & a);
	/*! \details Prints a sgfx::Cursor object. */
	Printer & operator << (const sgfx::Cursor & a);
	/*! \details Prints a sgfx::Point object. */
	Printer & operator << (const sgfx::Point & a);
	/*! \details Prints a sgfx::Region object. */
	Printer & operator << (const sgfx::Region & a);
	/*! \details Prints a sgfx::Area object. */
	Printer & operator << (const sgfx::Area & a);
	/*! \details Prints a sgfx::Pen object. */
	Printer & operator << (const sgfx::Pen & a);
	/*! \details Prints a sgfx::Vector object. */
	Printer & operator << (const sgfx::Vector & a);
	/*! \details Prints a sgfx::VectorPath object. */
	Printer & operator << (const sgfx::VectorPath & a);
	/*! \details Prints a sgfx::VectorPathDescription object. */
	Printer & operator << (const sgfx::VectorPathDescription & a);


	/*! \details Prints a chrono::ClockTime object. */
	Printer & operator << (const chrono::ClockTime & a);
	/*! \details Prints a chrono::MicroTime object. */
	Printer & operator << (const chrono::MicroTime & a);
	/*! \details Prints a chrono::Time object. */
	Printer & operator << (const chrono::Time & a);

	/*! \details Prints a basic s32 type (see set_flags() for details). */
	Printer & operator << (s32 a);
	/*! \details Prints a basic u32 type (see set_flags() for details). */
	Printer & operator << (u32 a);
	/*! \details Prints a basic s16 type (see set_flags() for details). */
	Printer & operator << (s16 a);
	/*! \details Prints a basic u16 type (see set_flags() for details). */
	Printer & operator << (u16 a);
	/*! \details Prints a basic s8 type (see set_flags() for details). */
	Printer & operator << (s8 a);
	/*! \details Prints a basic u8 type (see set_flags() for details). */
	Printer & operator << (u8 a);
	/*! \details Prints a pointer. */
	Printer & operator << (void * a);
	/*! \details Prints a c-style string. */
	Printer & operator << (const char * a);
	/*! \details Prints a float type. */
	Printer & operator << (float a);

	Printer & operator << (const hal::DriveInfo & a);

	Printer & operator << (const draw::DrawingPoint & a);
	Printer & operator << (const draw::DrawingArea & a);
	Printer & operator << (const draw::DrawingRegion & a);

	/*! \details Filtering levels. */
	enum verbose_level {
		FATAL /*! Prints fatal errors. */,
		ERROR /*! Prints fatal and error messages. */,
		WARNING /*! Printers warnings and worse. */,
		INFO /*! Prints basic info (default setting) plus warning and errors. */,
		MESSAGE /*! Prints additional messages. */,
		DEBUG /*! Prints everything. */
	};

	/*! \details Assign an effective verbose level to this object. */
	void set_verbose_level(enum verbose_level level){
		m_verbose_level = level;
	}

	/*! \details Assign a verbose level.
	 *
	 * @param level The level as 'fatal', 'error', 'warning', 'info', 'message', or 'debug'
	 * @return Zero on success
	 */
	int set_verbose_level(const var::ConstString & level);

	/*! \details Returns the current verbose level. */
	enum verbose_level verbose_level() const { return m_verbose_level; }

	/*! \details Prints a debug message if the verbose level is set to debug. */
	virtual Printer & debug(const char * fmt, ...);
	/*! \details Prints a message (filtered according to verbose_level()). */
	virtual Printer & message(const char * fmt, ...);
	/*! \details Prints information (filtered according to verbose_level()). */
	virtual Printer & info(const char * fmt, ...);
	/*! \details Prints a warning (filtered according to verbose_level()). */
	virtual Printer & warning(const char * fmt, ...);
	/*! \details Prints an error (filtered according to verbose_level()). */
	virtual Printer & error(const char * fmt, ...);
	/*! \details Prints a fatal error message. */
	virtual Printer & fatal(const char * fmt, ...);

	/*! \details Prints a debug message if the verbose level is set to debug. */
	Printer & debug(const var::ConstString & a){ return debug(a.cstring()); }
	/*! \details Prints a message (filtered according to verbose_level()). */
	Printer & message(const var::ConstString & a){ return message(a.cstring()); }
	/*! \details Prints information (filtered according to verbose_level()). */
	Printer & info(const var::ConstString & a){ return info(a.cstring()); }
	/*! \details Prints a warning (filtered according to verbose_level()). */
	Printer & warning(const var::ConstString & a){ return warning(a.cstring()); }
	/*! \details Prints an error (filtered according to verbose_level()). */
	Printer & error(const var::ConstString & a){ return error(a.cstring()); }
	/*! \details Prints a fatal error message. */
	Printer & fatal(const var::ConstString & a){ return fatal(a.cstring()); }


	Printer & error(const api::Result result, u32 line_number);

	/*! \details Prints a key value pair. */
	virtual Printer & key(const var::ConstString & key, const char * fmt, ...);
	/*! \details Prints a key value pair. */
	virtual Printer & key(const var::ConstString & key, const var::String & a);
	/*! \details Prints a key value pair. */
	virtual Printer & key(const var::ConstString & key, const var::JsonValue & a);

	void print(const char * fmt, ...);


	/*! \details Number printing flags. */
	enum {
		PRINT_HEX /*! Print hex data */ = (1<<0),
		PRINT_UNSIGNED /*! Print unsigned integers */ = (1<<1),
		PRINT_SIGNED /*! Printd signed integers */ = (1<<2),
		PRINT_CHAR /*! Print Characters */ = (1<<3),
		PRINT_8 /*! Print as 8 bit values (default) */ = 0,
		PRINT_16 /*! Print as 16 bit values */ = (1<<4),
		PRINT_32 /*! Print as 32 bit values */ = (1<<5),
		PRINT_SIMPLE_PROGRESS /*! Just print # for progress */ = (1<<6),
		PRINT_BOLD_KEYS /*! Print keys in bold if supported */ = (1<<7),
		PRINT_BOLD_VALUES /*! Print keys in bold if supported */ = (1<<8),
		PRINT_BOLD_OBJECTS /*! Print keys in bold if supported */ = (1<<9),
		PRINT_BOLD_ARRAYS /*! Print keys in bold if supported */ = (1<<10),
		PRINT_GREEN_VALUES = (1<<11),
		PRINT_RED_VALUES = (1<<12),
		PRINT_YELLOW_VALUES = (1<<13),
		PRINT_RED_ERRORS = (1<<14),
		PRINT_YELLOW_WARNINGS = (1<<15),
		PRINT_CYAN_KEYS = (1<<16),
		PRINT_YELLOW_KEYS = (1<<17),
		PRINT_MAGENTA_KEYS = (1<<18),
		PRINT_BLOB /*! Print Data as a blob */ = (1<<19)
	};

	/*! \details Sets the flags that modify how numbers and some messages are printed. */
	void set_flags(u32 value){ m_o_flags = value; }

	void enable_flags(u32 value){ m_o_flags |= value; }
	void disable_flags(u32 value){ m_o_flags &= ~value; }

	u32 flags() const { return m_o_flags; }

	/*! \details Opens a YAML object.
	 *
	 * @param key The key to be used for the object
	 * @param level The filter level for child keys
	 *
	 * YAML object values are not preceded by '-' while arrays are.
	 * Objects should have unique keys in order to have properly
	 * formatted YAML. The debug(), info(), warning(), error(),
	 * and fatal() methods can be used at most once inside of an object
	 * without having a duplicate key. The key() method can be used to assign
	 * unique keys to each object.
	 *
	 * ```
	 * //md2code:main
	 * Printer p;
	 * //whole object only prints with debug verbose level
	 * p.open_object("keys", Printer::DEBUG);
	 * p.key("key0", "value0");
	 * p.key("key1", "value1");
	 * p.key("key2", "value2");
	 * p.close_object();
	 * ```
	 *
	 */
	Printer & open_object(
			const var::ConstString & key,
			enum verbose_level level = FATAL);
	void close_object(){
		m_container.pop_back();
		if( m_indent ){
			m_indent--;
		}
	}

	/*! \details Opens a YAML array.
	 *
	 * @param key The key to be used for the object
	 * @param level The filter level for child keys
	 *
	 * YAML object values are not preceded by '-' while arrays are.
	 * Objects should have unique keys in order to have properly
	 * formatted YAML. The debug(), info(), warning(), error(),
	 * and fatal() methods can be used multple times inside of
	 * an array and are indexed when parsed.
	 *
	 *
	 *
	 * ```
	 * //md2code:main
	 * Printer p;
	 * //whole object only prints with debug verbose level
	 * p.open_array("keys", Printer::MESSAGE);
	 * p.info("Hello");
	 * p.info("Hello");
	 * p.info("Hello");
	 * p.warning("Hello");
	 * p.message("Hello");
	 * p.message("Hello");
	 * p.warning("Hello");
	 * p.close_array(); //close_array() and close_object() are interchangeable
	 * ```
	 *
	 *
	 * By default the Printer starts with an open array.  The following
	 * code can change the top level type to an object.
	 *
	 * ```
	 * //md2code:main
	 * Printer p; //printer is now an array
	 * p.close_array();
	 * p.open_object(""); //top level type is an object
	 * ```
	 *
	 */
	Printer & open_array(const var::ConstString & key, enum verbose_level level = FATAL);
	void close_array(){
		close_object();
	}

	/*! \details Returns a printer termination object for closing
	 * an array or object with the `<<` operator.
	 *
	 * ```
	 * //md2code:main
	 * Printer p;
	 * p.open_object("empty object") << p.close();
	 * p.open_array("messages", Printer::MESSAGE) << "hello" << p.close();
	 * ```
	 *
	 */
	static PrinterTermination close(){
		return PrinterTermination();
	}

	Printer & operator << (const PrinterTermination & printer_termination){
		close_object();
		return *this;
	}

	/*! \details Returns a pointer to the sys::ProgressCallback member.
	 *
	 * This method can be used for operations that use sys::ProgressCallback
	 * class to update the progress. The progess is printed to the screen
	 * with the key set to progress_key().
	 *
	 * ```
	 * //md2code:include
	 * #include <sapi/sys.hpp>
	 * #include <sapi/inet.hpp>
	 * #include <sapi/fs.hpp>
	 * ```
	 *
	 * ```
	 * //md2code:main
	 * Printer p;
	 *
	 * p.progress_key() = var::ConstString("downloading");
	 * Socket socket;
	 * HttpClient http_client(socket);
	 * DataFile data_file(OpenFlags::append_read_write());
	 *
	 * //download file to data_file and print the progress using #'s
	 * http_client.get(
	 *   arg::UrlEncodedString("http://some.url/file"),
	 *   arg::DestinationFile(data_file),
	 *   p.progress_callback()
	 *   );
	 * ```
	 *
	 */
	const sys::ProgressCallback * progress_callback() const {
		return &m_progress_callback;
	}

	/*! \details Used by progres_callback(). */
	static bool update_progress_callback(void * context, int progress, int total){
		Printer * printer = (Printer*)context;
		return printer->update_progress(progress, total);
	}

	/*! \details Used by progres_callback(). */
	bool update_progress(int progress, int total);

	/*! \details Access to the key to print during progress updates. */
	var::String & progress_key(){ return m_progress_key; }
	/*! \details Access (read-only) to the key to print during progress updates. */
	const var::String & progress_key() const { return m_progress_key; }

	 /*! \details Returns a character that will be printed.
	 *
	 * @param color The color of the pixel
	 * @param bits_per_pixel The number of bits per pixel in the color
	 * @return A character (like '*') to be used for the color
	 *
	 * This method is used for printing 1, 2, and 4, bit per pixel
	 * colors.
	 *
	 */
	static char get_bitmap_pixel_character(u32 color, u8 bits_per_pixel);

#if defined __link
	bool is_bash() const { return m_is_bash; }
	void set_bash(bool value = true){ m_is_bash = value; }
#endif

protected:

	/*! \details Prints the YAML indentation. */
	void print_indentation();

	/*! \details Prints an indented key (can be re-implemented). */
	virtual void print_indented(const var::ConstString & key, const char * fmt, ...);
	/*! \details Prints an indented key (can be re-implemented). */
	virtual void vprint_indented(const var::ConstString & key, const char * fmt, va_list list);
	/*! \details Prints all characters.
	 *
	 * This is the only method that needs to be re-implemented
	 * to re-direct the Printer from the standard output to
	 * another file or output.
	 *
	 */
	virtual void vprint(const char * fmt, va_list list);

private:

	enum {
		CONTAINER_OBJECT,
		CONTAINER_ARRAY
	};

	u8 current_container(){
		if( m_container.count() ){
			return m_container.at( m_container.count() - 1 ) & 0xff;
		}
		return CONTAINER_OBJECT;
	}

	enum verbose_level current_level(){
		if( m_container.count() ){
			return (enum verbose_level)(m_container.at( m_container.count() - 1 ) >> 8);
		}
		return FATAL;
	}

	var::Vector<u16> m_container;
	ProgressCallback m_progress_callback;
	u16 m_progress_width;
	u16 m_progress_state;
	u16 m_indent;
	u32 m_key_count;
	bool m_is_json;
	u32 m_o_flags;
	void print_bitmap_pixel(u32 color, u8 bits_per_pixel);
	var::String m_progress_key;

	enum verbose_level m_verbose_level;

#if defined __link
	bool m_is_bash;
#endif

};

class NullPrinter : public Printer {
protected:
	void vprint(const char * fmt, va_list list){
		MCU_UNUSED_ARGUMENT(fmt);
		MCU_UNUSED_ARGUMENT(list);
	}

};


}

#endif // SAPI_SYS_PRINTER_HPP_
