/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.


//sl sdk.copyright:message="'Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.'",path=StratifyAPI,prefix='/*! \file */ //',filter=cmake_link?cmake_arm?rapidjson?rapidxml --verbose=message


#ifndef SAPI_API_APIOBJECT_HPP_
#define SAPI_API_APIOBJECT_HPP_

/*! \mainpage
 *
 * <small>This is the <b>Stratify Application Programming Interface (API)</b> documentation.  This documentation is for building applications
 * for Stratify OS. If you are creating board support packages, you will want to take a look at the
 * <a href="../../StratifyOS/html/">Stratify OS documentation</a>.</small>
 *
 * \section Introduction
 *
 * StratifyAPI is a library for Stratify OS applications that makes it easy to access hardware
 * and other system functions (you can also use standard \ref POSIX and \ref STDC functions if that
 * is your style).
 *
 * If you are new, it would be highly beneficial to quickly read through the naming conventions:
 * https://github.com/StratifyLabs/StratifyLib#naming-conventions. Once you are familiar with those,
 * you can check out these useful namespaces to get started.
 *
 * - \ref hal - Hardware Abstraction Layer
 * - \ref calc - Useful software algorithms
 * - \ref draw - Drawing on an LCD
 * - \ref fmt - Common file formats
 * - \ref sys - Access to timers and files and such
 * - \ref var - Variable storage in data and strings
 *
 * The following examples will give you an idea about how things work. These are the objects
 * used in the examples.
 *
 * - hal::Uart - access to UART hardware
 * - hal::Pin - read and write pin values
 * - var::String - string manipulation
 * - chrono::Timer - Timing and delays
 *
 * \code
 * #include <sapi/hal.hpp> //this will include all namespace classes and add the namespace i.e. "using namespace hal;"
 * #include <sapi/sys.hpp> //includes all sys classes plus "using namespace sys;"
 * #include <sapi/chrono.hpp> //includes all chrono classes plus "using namespace chrono;"
 *
 * int main(int argc, char * argv[]){
 *  Pin p(2, 10);
 *  Pin button(2,9);
 *  p.init(Pin::FLAG_SET_OUTPUT); //open the port and configure pin 10 as an output
 *  button.init(Pin::FLAG_SET_INPUT | Pin::FLAG_IS_PULLUP); //pin 9 is an input (pulled high)
 *
 *  //toggle p until the button is pressed
 *  while( button.get_value() != 0 ){
 *   p = false;
 *   Timer::wait_milliseconds(100);
 *   p = true;
 *   Timer::wait_milliseconds(100);
 *  }
 *
 * 	return 0;
 * }
 * \endcode
 *
 *
 * Here is an example of a UART echo routine.
 *
 * \code
 * #include <sapi/hal.hpp>
 * #include <sapi/var.hpp>
 *
 * int main(int argc, char * argv[]){
 *  Uart uart(0);
 *	int ret;
 *	String exit_command = "exit\n";
 *	bool done = false;
 *  char buffer[64];
 *  uart.init(); //initializes the UART with default settings
 *
 *  //echo UART until exit is entered
 *  while( done == false ){
 *  	if( (ret = uart.read(buffer,64)) > 0 ){
 *  		if( exit_command == buffer ){
 *  			done = true;
 *  		}
 *			uart.write(buffer, ret);
 *  	 }
 *  }
 *
 * 	return 0;
 * }
 * \endcode
 *
 * The best way to learn is to get started:  https://github.com/StratifyLabs/HelloWorld#helloworld.
 *
 */

#include <mcu/types.h>
#if defined __win32

/*
 * \tg
 *
 * There needs to be a list of POSIX error number
 *
 * For example,
 *
 * #define EIO 5
 *
 */

#endif


#define API_DEPRECATED(message) [[deprecated( message )]]
#define API_DEPRECATED_NO_REPLACEMENT [[deprecated]]

/*!
 * \brief Application Programming Interface
 *
 * \details
 *
 * **Introduction**
 *
 * The Stratify API library contains classes that allow easy access
 * to Stratify OS hardware and POSIX constructs such as threads (sys::Thread),
 * mutexes (sys::Mutex), files (fs::File) and many more. It also contains
 * classes used to manage data (see the var namespace) in an embedded
 * friendly way.
 *
 * If you are just getting started with Stratify OS and the Stratify API,
 * try reading through some of the guides:
 *
 * - [Stratify OS](https://docs.stratifylabs.co/guides/Guide-Stratify-OS/)
 * - [Filesystems](https://docs.stratifylabs.co/guides/Guide-Filesystems/)
 * - [Threads](https://docs.stratifylabs.co/guides/Guide-Threads/)
 * - [CMake Projects](https://docs.stratifylabs.co/guides/Guide-CMake/)
 *
 * If you are ready to start tinkering, hal::Pin is a nice place to
 * start and allows you to read and write GPIO values. hal::Uart
 * can be used to access UART peripherals and fs::File is for
 * reading and writing files on any mounted filesystem.
 *
 * **Code Hierarchy**
 *
 * The api namespace contains all top level objects. All objects inherit
 * from api::ApiObject. Below api::ApiObject this is api::ApiWorkObject
 * and api::ApiInfoObject.
 *
 * **None of the classes that are part of the api
 * namespace should be directly declared.**
 *
 * Work objects include an error number and are the base for objects that do work and make system calls.
 *
 * Info objects are used for storing and managing static data structures. They don't make
 * system calls and can't store errors. Info objects also include classes with only static methods.
 *
 *
 */
namespace api {



#define API_ACCESS_BOOL(c, v, iv) \
	public: \
	bool is_##v() const { return m_is_##v; } \
	c& set_##v(bool value = true){ m_is_##v = value; return *this; } \
	private: \
	bool m_is_##v = iv

#define API_AB(c, v, iv) API_ACCESS_BOOL(c, v, iv)

#define API_READ_ACCESS_BOOL(c, v, iv) \
	public: \
	bool is_##v() const { return m_is_##v; } \
	private: \
	bool m_is_##v = iv

#define API_RAB(c, v, iv) API_READ_ACCESS_BOOL(c, v, iv)

#define API_ACCESS_FUNDAMENTAL(c, t, v, iv) \
	public: \
	t v() const { return m_##v; } \
	c& set_##v(t value){ m_##v = value; return *this; } \
	private: \
	t m_##v = iv

#define API_AF(c, t, v, iv) API_ACCESS_FUNDAMENTAL(c, t, v, iv)

#define API_ACCESS_MEMBER_FUNDAMENTAL(c, t, p, v) \
	public: \
	t v() const { return m_##p.v; } \
	c& set_##v(t value){ m_##p.v = value; return *this; }

#define API_AMF(c, t, p, v) API_ACCESS_MEMBER_FUNDAMENTAL(c, t, p, v)

#define API_READ_ACCESS_MEMBER_FUNDAMENTAL(c, t, p, v) \
	public: \
	t v() const { return m_##p.v; }

#define API_RAMF(c, t, p, v) API_READ_ACCESS_MEMBER_FUNDAMENTAL(c, t, p, v)

#define API_ACCESS_MEMBER_FUNDAMENTAL_WITH_ALIAS(c, t, p, a, v) \
	public: \
	t a() const { return m_##p.v; } \
	c& set_##a(t value){ m_##p.v = value; return *this; } \

#define API_AMFWA(c, t, p, a, v) API_ACCESS_MEMBER_FUNDAMENTAL_WITH_ALIAS(c, t, p, a, v)

#define API_ACCESS_MEMBER_COMPOUND(c, t, p, v) \
	public: \
	const t& v() const { return m_##p.v; } \
	c& set_##v(const t& value){ m_##p.v = value; return *this; } \

#define API_AMC(c, t, p, v) API_ACCESS_MEMBER_COMPOUND(c, t, p, v)

#define API_READ_ACCESS_FUNDAMENTAL(c, t, v, iv) \
	public: \
	t v() const { return m_##v; } \
	private: \
	t m_##v = iv

#define API_RAF(c, t, v, iv) API_READ_ACCESS_FUNDAMENTAL(c, t, v, iv)

#define API_ACCESS_COMPOUND(c, t, v) \
	public: \
	const t& v() const { return m_##v; } \
	t& v(){ return m_##v; } \
	c& set_##v(const t& value){ m_##v = value; return *this; } \
	private: \
	t m_##v

#define API_AC(c, t, v) API_ACCESS_COMPOUND(c, t, v)

#define API_READ_ACCESS_COMPOUND(c, t, v) \
	public: \
	const t& v() const { return m_##v; } \
	private: \
	t m_##v

#define API_RAC(c, t, v) API_READ_ACCESS_COMPOUND(c, t, v)

#define API_ACCESS_DERIVED_COMPOUND(c,d,t,v) \
	d& set_##v(const t& value){ \
	c::set_##v(value); \
	return static_cast<d&>(*this); \
	}

#define API_ADC(c,d,t,v) API_ACCESS_DERIVED_COMPOUND(c,d,t,v)

#define API_ACCESS_DERIVED_FUNDAMETAL(c,d,t,v) \
	d& set_##v(t value){ \
	c::set_##v(value); \
	return static_cast<d&>(*this); \
	}

#define API_ADF(c,d,t,v) API_ACCESS_DERIVED_FUNDAMETAL(c,d,t,v)

#define API_ACCESS_DERIVED_BOOL(c,d,v) \
	d& set_##v(bool value = true){ \
	c::set_##v(value); \
	return static_cast<d&>(*this); \
	}

#define API_ADB(c,d,v) API_ACCESS_DERIVED_BOOL(c,d,v)

#define API_OR_NAMED_FLAGS_OPERATOR(TYPE, FLAG_NAME) \
	inline enum TYPE::FLAG_NAME operator |( \
	const enum TYPE::FLAG_NAME a, \
	const enum TYPE::FLAG_NAME b){ \
	return static_cast<enum TYPE::FLAG_NAME>( \
	static_cast<u32>(a) | \
	static_cast<u32>(b) \
	); \
} \
	inline enum TYPE::FLAG_NAME operator &( \
	const enum TYPE::FLAG_NAME a, \
	const enum TYPE::FLAG_NAME b){ \
	return static_cast<enum TYPE::FLAG_NAME>( \
	static_cast<u32>(a) & \
	static_cast<u32>(b) \
	); \
} \
	inline enum TYPE::FLAG_NAME operator ~( \
	const enum TYPE::FLAG_NAME a){ \
	return static_cast<enum TYPE::FLAG_NAME>( \
	~(static_cast<u32>(a)) \
	); \
} \
	inline enum TYPE::FLAG_NAME & operator |=( \
	enum TYPE::FLAG_NAME & a, \
	const enum TYPE::FLAG_NAME b){ \
	return a = a | b;\
} \
	inline enum TYPE::FLAG_NAME & operator &=( \
	enum TYPE::FLAG_NAME & a, \
	const enum TYPE::FLAG_NAME b){ \
	return a = a & b;\
}

#define API_OR_FLAGS_OPERATOR(TYPE) API_OR_NAMED_FLAGS_OPERATOR(TYPE, flags)

#define API_ENUM_LOOP_OPERATOR(TYPE) \
	TYPE& operator ++ (TYPE& a){ \
	a =	TYPE(static_cast<std::underlying_type<TYPE>::type>(a) + 1); \
	return a; }

/*! \brief Application Programming Interface Object
 * \details The API Object class is the parent of all
 * other classes. The API namespace contains
 * two other classes that inherit this object
 *
 * - api::InfoObject
 * - api::WorkObject
 *
 * InfoObject is for classes that are used to access static
 * data and don't do any work or have the potential to have errors.
 * InfoObjects also include classes that consist purely of static
 * methods and members.
 *
 * WorkObject has an interface to setting the error and issuing
 * a fatal exit if a fatal error occurs.
 *
 * All object in the API inherit either InfoObject or WorkObject.
 *
 */
class ApiObject {
public:

};


/*! \brief ApiInfo Class
 * \details Provides inforamation abou the API library.
 *
 * This object is available in the api
 * namespace when including any object
 * or namespace within the StratifyAPI. For
 * this example, we will include `sapi/sys.hpp`
 * but any `sapi` header file will do.
 *
 * ```
 * //md2code:include
 * #include <sapi/sys.hpp>
 * ```
 */
class ApiInfo : public ApiObject {
public:
	/*! \details Returns a pointer to a string
		* that shows the API version.
		*
	 * ```
	 * //md2code:main
	 *	printf("The Stratify API version is %s\n",
	 *	  api::ApiInfo::version()
	 * );
	 * ```
		*
		*/
	static const char * version(){ return "3.20.3"; }

	/*! \details Returns a c-style string pointer
	 * to the git hash used to build the Stratify API.
	 *
	 * ```
	 * //md2code:main
	 *	printf("The Stratify API git hash is %s\n",
	 *	  api::ApiInfo::git_hash()
	 * );
	 * ```
	 *
	 *
	 */
	static const char * git_hash();

	/*! \details Returns a c-style string pointer
	 * of the name of the operating system
	 * the application is running on.
	 *
	 * ```
	 * //md2code:main
	 *	printf("The Stratify API application is running on %s\n",
	 *	  api::ApiInfo::operating_system_name()
	 * );
	 * ```
	 *
	 * The names are:
	 *
	 * - `macosx`
	 * - `windows`
	 * - `linux`
	 * - `stratifyos`
	 *
	 *
	 */
	static const char * operating_system_name();

	static const char * system_processor();

	static bool is_processor_i386();
	static bool is_processor_x86_64();
	static bool is_processor_arm32();
	static bool is_processor_arm64();

	/*! \details Returns true if the application
	 * is running on a windows system.
	 *
	 * ```
	 * //md2code:main
	 * if( api::ApiInfo::is_windows() ){
	 *   printf("This is windows baby!\n");
	 * }
	 * ```
	 *
	 */
	static bool is_windows();

	/*! \details Returns true if the application
	 * is running on a mac os x system.
	 *
	 * ```
	 * //md2code:main
	 * if( api::ApiInfo::is_macosx() ){
	 *   printf("Running on macosx\n");
	 * }
	 * ```
	 *
	 */
	static bool is_macosx();

	/*! \details Returns true if the application
	 * is running on a mac os x system.
	 *
	 * ```
	 * //md2code:main
	 * if( api::ApiInfo::is_linux() ){
	 *   printf("Running on linux\n");
	 * }
	 * ```
	 *
	 */
	static bool is_linux();


	/*! \details Returns true if the application
	 * is running on a Stratify OS system.
	 *
	 * ```
	 * //md2code:main
	 * if( api::ApiInfo::is_stratify_os() ){
	 *   printf("Yep!\n");
	 * }
	 * ```
	 *
	 */
	static bool is_stratify_os();

	/*! \details Returns a directory path
	 * that can be used to store application
	 * data.
	 *
	 * For Stratify OS, this is `/home`.
	 *
	 */
	static const char * user_data_path();

	static u32 malloc_start_chunk_size();
	static u32 malloc_chunk_size();
};

/** \cond */
#if defined __link
template<typename A, const A * initial_value> class Api : public ApiObject {
#else
extern "C" const void * kernel_request_api(u32 request);
template<typename A, u32 request> class Api : public ApiObject {
#endif
public:

	Api(){ initialize(); }

	bool is_valid(){
		initialize();
		return m_api != nullptr;
	}

	Api & operator = (const A * value){
		m_api = value;
		return *this;
	}

	const A * operator ->() const { return m_api; }
	const A * api() const { return m_api; }

private:
	void initialize(){
		if( m_api == nullptr ){
#if defined __link
			m_api = initial_value;
#else
			m_api = (const A*)kernel_request_api(request);
#endif
		}
	}
	const A * m_api = nullptr;
};

/** \endcond */

template<class T> class Argument : public ApiObject {
public:
	explicit Argument(T argument) : m_argument(argument){}
	T argument() const { return m_argument; }
private:
	T m_argument;
};

enum error_codes : s32 {
	error_code_flag_calc = 0x00001000,
	error_code_flag_chrono = 0x00002000,
	error_code_flag_crypto = 0x00004000,
	error_code_flag_fmt = 0x00008000,
	error_code_flag_fs = 0x00010000,
	error_code_flag_hal = 0x00020000,
	error_code_flag_inet = 0x00040000,
	error_code_flag_sgfx = 0x00080000,
	error_code_flag_sys = 0x00100000,
	error_code_flag_test = 0x00200000,
	error_code_flag_ux = 0x00400000,
	error_code_flag_var = 0x00800000,

	error_code_crypto_size_mismatch = -(error_code_flag_crypto|1),
	error_code_crypto_bad_block_size = -(error_code_flag_crypto|2),
	error_code_crypto_operation_failed = -(error_code_flag_crypto|3),
	error_code_crypto_missing_api = -(error_code_flag_crypto|4),
	error_code_crypto_unsupported_operation = -(error_code_flag_crypto|5),
	error_code_crypto_bad_iv_size = -(error_code_flag_crypto|6),

	error_code_fs_failed_to_open = -(error_code_flag_fs|1),
	error_code_fs_failed_to_read = -(error_code_flag_fs|2),
	error_code_fs_failed_to_write = -(error_code_flag_fs|3),
	error_code_fs_failed_to_seek = -(error_code_flag_fs|4),
	error_code_fs_failed_to_create = -(error_code_flag_fs|5),
	error_code_fs_failed_to_stat = -(error_code_flag_fs|6),
	error_code_fs_unsupported_operation = -(error_code_flag_fs|7),
	error_code_fs_bad_descriptor = -(error_code_flag_fs|8),
	error_code_fs_cant_read = -(error_code_flag_fs|9),
	error_code_fs_cant_write = -(error_code_flag_fs|10),
	error_code_fs_cant_write_read_only = -(error_code_flag_fs|11),
	error_code_fs_cant_write_append_only = -(error_code_flag_fs|12),
	error_code_fs_not_open = -(error_code_flag_fs|13),
  error_code_fs_failed_to_close = -(error_code_flag_fs|14),
  error_code_fs_failed_to_unlink = -(error_code_flag_fs|15),
  error_code_fs_failed_to_rmdir = -(error_code_flag_fs|16),

	error_code_inet_failed_to_create_socket /*! Failed to create a socket (1) */ = -(error_code_flag_inet|1),
	error_code_inet_failed_to_connect_to_socket /*! Failed to connect to socket (2) */ = -(error_code_flag_inet|2),
	error_code_inet_failed_to_write_header /*! Failed to write request header (3) */ = -(error_code_flag_inet|3),
	error_code_inet_failed_to_write_data /*! Failed to write data (4) */ = -(error_code_flag_inet|4),
	error_code_inet_failed_to_write_incoming_data_to_file /*! Failed  write incoming data to file provided (5) */ = -(error_code_flag_inet|5),
	error_code_inet_failed_to_find_address /*! Failed to find IP address of URL (6) */ = -(error_code_flag_inet|6),
	error_code_inet_failed_to_get_status_code /*! Failed to get a status code in the HTTP response (7) */ = -(error_code_flag_inet|7),
	error_code_inet_failed_to_get_header /*! Failed to receive the header (8) */ = -(error_code_flag_inet|8),
	error_code_inet_failed_wrong_domain = -(error_code_flag_inet|9),
	error_code_inet_wifi_api_missing = -(error_code_flag_inet|10),

	error_code_var_json_unknown = -(error_code_flag_var|1),
	error_code_var_json_out_of_memory = -(error_code_flag_var|2),
	error_code_var_json_stack_overflow = -(error_code_flag_var|3),
	error_code_var_json_cannot_open_file = -(error_code_flag_var|4),
	error_code_var_json_invalid_argument = -(error_code_flag_var|5),
	error_code_var_json_invalid_utf8 = -(error_code_flag_var|6),
	error_code_var_json_premature_end_of_input = -(error_code_flag_var|7),
	error_code_var_json_end_of_input_expected = -(error_code_flag_var|8),
	error_code_var_json_invalid_syntax = -(error_code_flag_var|9),
	error_code_var_json_invalid_format = -(error_code_flag_var|10),
	error_code_var_json_wrong_type = -(error_code_flag_var|11),
	error_code_var_json_null_character = -(error_code_flag_var|12),
	error_code_var_json_null_value = -(error_code_flag_var|13),
	error_code_var_json_null_byte_in_key = -(error_code_flag_var|14),
	error_code_var_json_duplicate_key = -(error_code_flag_var|15),
	error_code_var_json_numeric_overflow = -(error_code_flag_var|16),
	error_code_var_json_item_not_found = -(error_code_flag_var|17),
	error_code_var_json_index_out_of_range = -(error_code_flag_var|18),

	error_code_flag_sys_appfs_create_failed = -(error_code_flag_sys|1),
	error_code_flag_sys_appfs_append_failed = -(error_code_flag_sys|2),

};

const char * get_error_code_description(s32 ec);

#define API_ASSERT(a) api::api_assert(a,__PRETTY_FUNCTION__,__LINE__);
void api_assert(bool value, const char * function, int line);

}

#endif // SAPI_API_APIOBJECT_HPP_
