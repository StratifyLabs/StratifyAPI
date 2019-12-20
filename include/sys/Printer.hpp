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

struct PrinterFlags {
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
		PRINT_BLOB /*! Print Data as a blob */ = (1<<19),
		print_hex /*! Print hex data */ = (1<<0),
		print_unsigned /*! Print unsigned integers */ = (1<<1),
		print_signed /*! Printd signed integers */ = (1<<2),
		print_char /*! Print Characters */ = (1<<3),
		print_8 /*! Print as 8 bit values (default) */ = 0,
		print_16 /*! Print as 16 bit values */ = (1<<4),
		print_32 /*! Print as 32 bit values */ = (1<<5),
		print_simple_progress /*! Just print # for progress */ = (1<<6),
		print_bold_keys /*! Print keys in bold if supported */ = (1<<7),
		print_bold_values /*! Print keys in bold if supported */ = (1<<8),
		print_bold_objects /*! Print keys in bold if supported */ = (1<<9),
		print_bold_arrays /*! Print keys in bold if supported */ = (1<<10),
		print_green_values = (1<<11),
		print_red_values = (1<<12),
		print_yellow_values = (1<<13),
		print_red_errors = (1<<14),
		print_yellow_warnings = (1<<15),
		print_cyan_keys = (1<<16),
		print_yellow_keys = (1<<17),
		print_magenta_keys = (1<<18),
		print_blob /*! Print Data as a blob */ = (1<<19),
		print_cyan_values = (1<<20),
		print_red_keys = (1<<21)
	};

	enum format_type {
		/*! \cond */
		FORMAT_NORMAL = 0,
		FORMAT_BOLD = 1,
		FORMAT_DIM = 2,
		FORMAT_UNDERLINE = 4,
		FORMAT_BLINK = 5,
		FORMAT_INVERTED = 7,
		FORMAT_HIDDEN = 8,
		/*! \endcond */
		format_normal = 0,
		format_bold = 1,
		format_dim = 2,
		format_underline = 4,
		format_blink = 5,
		format_inverted = 7,
		format_hidden = 8,
	};

	enum color_code {
		/*! \cond */
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
		COLOR_CODE_LIGHT_BLUE = 94,
		/*! \endcond */
		color_code_default = 39,
		color_code_black = 30,
		color_code_red = 31, //RED
		color_code_green = 32, //GREEN
		color_code_yellow = 33,
		color_code_blue = 34, //BLUE
		color_code_magenta = 35,
		color_code_cyan = 36, //BLUE | GREEN
		color_code_light_gray = 37,
		color_code_dark_gray = 90,
		color_code_light_red = 91,
		color_code_light_green = 92,
		color_code_light_yellow = 93,
		color_code_light_blue = 94
	};

	/*! \details Filtering levels. */
	enum verbose_level {
		/*! \cond */
		FATAL /*! Prints fatal errors. */,
		ERROR /*! Prints fatal and error messages. */,
		WARNING /*! Printers warnings and worse. */,
		INFO /*! Prints basic info (default setting) plus warning and errors. */,
		MESSAGE /*! Prints additional messages. */,
		DEBUG /*! Prints everything. */,
		/*! \endcond */
		level_fatal /*! Prints fatal errors. */ = FATAL,
		level_error /*! Prints fatal and error messages. */,
		level_warning /*! Printers warnings and worse. */,
		level_info /*! Prints basic info (default setting) plus warning and errors. */,
		level_message /*! Prints additional messages. */,
		level_debug /*! Prints debugging information. */,
		level_trace /*! Prints line-by-line tracing (useful only for development). */,
	};

};


#define PRINTER_TRACE(printer, msg) (printer.trace(__PRETTY_FUNCTION__, __LINE__, msg))
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
class Printer : public api::WorkObject, public PrinterFlags {
public:
	Printer();
	~Printer();


	static u32 color_code(const var::String & color);

	void set_format_code(u32 code);
	void clear_format_code(u32 code);

	void set_color_code(u32 code);
	void clear_color_code();
#if defined __win32
	static unsigned int m_default_color;
#endif

#if !defined __link
	Printer & operator << (const var::DataInfo & a);
#endif
	Printer & operator << (const api::Result & a);
	Printer & operator << (const var::Reference & a);
	Printer & operator << (const var::Datum & a);
	Printer & operator << (const var::String & a);
	Printer & operator << (const var::Tokenizer & a);
	Printer & operator << (const var::JsonObject & a);
	Printer & operator << (const var::JsonArray & a);
	Printer & operator << (const var::Vector<var::String> & a);
	Printer & operator << (const var::Ring<u32> & a);
	Printer & operator << (const var::Ring<s32> & a);
	Printer & operator << (const var::Ring<u16> & a);
	Printer & operator << (const var::Ring<s16> & a);
	Printer & operator << (const var::Ring<u8> & a);
	Printer & operator << (const var::Ring<s8> & a);
	Printer & operator << (const Cli & a);
	Printer & operator << (const appfs_file_t & a);
	Printer & operator << (const AppfsFileAttributes & a);
	Printer & operator << (const TraceEvent & a);
	Printer & operator << (const sys::SysInfo & a);
	Printer & operator << (const sys::AppfsInfo & a);
	Printer & operator << (const sys::TaskInfo & a);
	Printer & operator << (const fs::Stat & a);
	Printer & operator << (const sgfx::Bitmap & a);
	Printer & operator << (const sgfx::Cursor & a);
	Printer & operator << (const sgfx::Point & a);
	Printer & operator << (const sgfx::Region & a);
	Printer & operator << (const sgfx::Area & a);
	Printer & operator << (const sgfx::Pen & a);
	Printer & operator << (const sgfx::Vector & a);
	Printer & operator << (const sgfx::VectorPath & a);
	Printer & operator << (const sgfx::VectorPathDescription & a);
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
	Printer & operator << (const hal::DriveInfo & a);
	Printer & operator << (const draw::DrawingPoint & a);
	Printer & operator << (const draw::DrawingArea & a);
	Printer & operator << (const draw::DrawingRegion & a);

	/*! \details Assign an effective verbose level to this object. */
	Printer& set_verbose_level(enum verbose_level level){
		m_verbose_level = level;
		return *this;
	}

	/*! \details Assign a verbose level.
	 *
	 * @param level The level as 'fatal', 'error', 'warning', 'info', 'message', or 'debug'
	 * @return Zero on success
	 */
	Printer& set_verbose_level(const var::String & level);

	/*! \details Returns the current verbose level. */
	enum verbose_level verbose_level() const { return m_verbose_level; }

	/*! \cond */
	virtual Printer & debug(const char * fmt, ...);
	virtual Printer & message(const char * fmt, ...);
	virtual Printer & info(const char * fmt, ...);
	virtual Printer & warning(const char * fmt, ...);
	virtual Printer & error(const char * fmt, ...);
	virtual Printer & fatal(const char * fmt, ...);
	/*! \endcond */

	Printer & trace(const char * function, int line, const var::String & message);
	/*! \details Prints a debug message if the verbose level is set to debug. */
	Printer & debug(const var::String & a){ return debug(a.cstring()); }
	/*! \details Prints a message (filtered according to verbose_level()). */
	Printer & message(const var::String & a){ return message(a.cstring()); }
	/*! \details Prints information (filtered according to verbose_level()). */
	Printer & info(const var::String & a){ return info(a.cstring()); }
	/*! \details Prints a warning (filtered according to verbose_level()). */
	Printer & warning(const var::String & a){ return warning(a.cstring()); }
	/*! \details Prints an error (filtered according to verbose_level()). */
	Printer & error(const var::String & a){ return error(a.cstring()); }
	/*! \details Prints a fatal error message. */
	Printer & fatal(const var::String & a){ return fatal(a.cstring()); }

	Printer & error(const api::Result result, u32 line_number);

	/*! \details Sets the flags that modify how numbers and some messages are printed. */
	Printer& set_flags(u32 value){ m_o_flags = value; return *this; }

	Printer& enable_flags(u32 value){ m_o_flags |= value; return *this; }
	Printer& disable_flags(u32 value){ m_o_flags &= ~value; return *this; }

	u32 flags() const { return m_o_flags; }



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
	 * p.progress_key() = var::String("downloading");
	 * Socket socket;
	 * HttpClient http_client(socket);
	 * DataFile data_file(OpenFlags::append_read_write());
	 *
	 * //download file to data_file and print the progress using #'s
	 * http_client.get(
	 *   arg::const var::String &("http://some.url/file"),
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
	static u32 get_bitmap_pixel_color(char c, u8 bits_per_pixel);

#if defined __link
	bool is_bash() const { return m_is_bash; }
	void set_bash(bool value = true){ m_is_bash = value; }
#endif

	u32 o_flags() const {
		return m_o_flags;
	}

	Printer & key(const var::String & key, const char * fmt, ...);
	Printer & key(const var::String & key, const var::String & a);
	Printer & key(const var::String & key, const var::JsonValue & a);


	const var::String terminal_color_code(enum color_code code);

protected:

	virtual void print_open_object(
			enum verbose_level verbose_level,
			const char * key
			);
	virtual void print_close_object();
	virtual void print(
			enum verbose_level level,
			const char * key,
			const char * value,
			bool is_newline = true
			);
	void print_final_color(enum color_code code, const char * snippet);
	virtual void print_final(const char * fmt, ...);

private:

	ProgressCallback m_progress_callback;
	u16 m_progress_width;
	u16 m_progress_state;

	u16 m_indent;
	u32 m_o_flags;

	var::String m_progress_key;

	enum verbose_level m_verbose_level;

#if defined __link
	bool m_is_bash;
#endif

};

class NullPrinter : public Printer {
protected:
	void print(const char * key, const char * value){
		MCU_UNUSED_ARGUMENT(key);
		MCU_UNUSED_ARGUMENT(value);
	}
};

template <typename T> class PrinterContainer: public PrinterFlags {
public:
	PrinterContainer(
			enum verbose_level verbose_level,
			T type
			){
		m_verbose_level = verbose_level;
		m_type = type;
		m_count = 1;
	}

	enum verbose_level verbose_level() const {
		return m_verbose_level;
	}

	void set_verbose_level(enum verbose_level level){
		m_verbose_level = level;
	}

	const u32 & count() const { return m_count; }
	u32 & count(){ return m_count; }

	T type() const {
		return m_type;
	}

private:
	enum verbose_level m_verbose_level;
	u32 m_count;
	T m_type;
};


}

#endif // SAPI_SYS_PRINTER_HPP_
