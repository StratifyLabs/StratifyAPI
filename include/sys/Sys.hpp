/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SYS_SYS_HPP_
#define SYS_SYS_HPP_

#if !defined __link
#include <sos/sos.h>
#endif

#include <sos/dev/sys.h>
#include <sos/link.h>
#include "File.hpp"
#include "../var/ConstString.hpp"

namespace sys {

class Sys;


/*! \brief Serial Number class
 * \details The SerialNumber class holds a value for an
 * MCU serial number.
 *
 * Stratify OS supports reading the serial number directly
 * from the chip. This class makes doing so as simply as possible.
 *
 */
class SerialNumber : public api::SysInfoObject {
public:

	/*! \details Constructs an empty serial number. */
	SerialNumber();

	/*! \details Constructs a serial number for an array of u32 values. */
	SerialNumber(const u32 serial_number[4]){ memcpy(m_serial_number.sn, serial_number, sizeof(u32)*4); }

	/*! \details Constructs a serial number from an mcu_sn_t. */
	SerialNumber(const mcu_sn_t serial_number){ m_serial_number = serial_number; }

	/*! \details Constructs this serial number from \a str. */
	SerialNumber(const var::ConstString & str);

	/*! \details Returns true if a valid serial number is held. */
	bool is_valid() const {
		return at(0) + at(1) + at(2) + at(3) != 0;
	}

	/*! \details Queries the system for the MCU serial number associated
	 * with the device.
	 *
	 * @return Zero on success.
	 *
	 */
	static SerialNumber get();

	/*! \details Returns a serial number object from a string type. */
	static SerialNumber from_string(const var::ConstString & str);

	/*! \details Returns the u32 section of the serial number specified by *idx*. */
	u32 at(u32 idx) const {
		if( idx >= 4 ){
			idx = 3;
		}
		return m_serial_number.sn[idx];
	}



	/*! \details Compares this strig to \a serial_number. */
	bool operator == (const SerialNumber & serial_number);

	/*! \details Converts the serial number to a string. */
	var::String to_string() const;

private:
	mcu_sn_t m_serial_number;
};

/*! \brief System Information Class
 * \details This class holds the system information.
 *
 *
 * \code
 * #include <sapi/sys.hpp>
 * SysInfo info = SysInfo::get(); //grab system information
 * Printer p;
 * p << info; //print system information using the printer
 * \endcode
 *
 */
class SysInfo : public api::SysInfoObject {
	friend class Sys;
public:

	/*! \details Constructs an empty SysInfo object. */
	SysInfo(){ clear(); }

	/*! \details Constructs an object from *info*. */
	SysInfo(const sys_info_t & info ){
		m_info = info;
	}

	operator const sys_info_t & () const { return m_info; }

	/*! \details Returns true if the object is valid. */
	bool is_valid() const { return cpu_frequency() != 0; }

	/*! \details Gets the system information from the OS and returns a SysInfo object. */
	static SysInfo get();

	/*! \details Returns the OS package project ID. */
	var::ConstString id() const { return m_info.id; }
	/*! \details Returns the name of the system. */
	var::ConstString name() const { return m_info.name; }
	/*! \details Returns the system version. */
	var::ConstString system_version() const { return m_info.sys_version; }
	/*! \details Returns the board support package version (same as system_version()). */
	var::ConstString bsp_version() const { return m_info.sys_version; }
	/*! \details Returns the Stratify OS version version. */
	var::ConstString sos_version() const { return m_info.kernel_version; }
	/*! \details Returns the kernel version (same as sos_version()). */
	var::ConstString kernel_version() const { return m_info.kernel_version; }

	/*! \details Returns the CPU architecture.
	 *
	 * Applications that are installed on the system must
	 * have a compatible architecture.
	 *
	 *
	 */
	var::ConstString cpu_architecture() const { return m_info.arch; }

	/*! \details Returns the CPU core clock frequency in Hertz. */
	u32 cpu_frequency() const { return m_info.cpu_freq; }

	/*! \details Returns the signature for the system call table.
	 *
	 * Applications that are installed must have a compatible signature.
	 *
	 */
	u32 application_signature() const { return m_info.signature; }

	/*! \details Returns the GIT Hash of the OS package as built. */
	var::ConstString bsp_git_hash() const { return m_info.bsp_git_hash; }
	/*! \details Returns the Stratify OS library used to link the OS package. */
	var::ConstString sos_git_hash() const { return m_info.sos_git_hash; }
	/*! \details Returns the Stratify OS MCU library used to link the OS package. */
	var::ConstString mcu_git_hash() const { return m_info.mcu_git_hash; }

	u32 o_flags() const { return m_info.o_flags; }

	var::ConstString arch() const { return m_info.arch; }
	var::ConstString stdin_name() const { return m_info.stdin_name; }
	var::ConstString stdout_name() const { return m_info.stdout_name; }
	var::ConstString trace_name() const { return m_info.trace_name; }
	u32 hardware_id() const { return m_info.hardware_id; }


	SerialNumber serial_number() const { return SerialNumber(m_info.serial); }

	void clear(){
		memset(&m_info, 0, sizeof(m_info));
	}

private:
	sys_info_t m_info;


};

/*! \brief Sys Class
 * \details This class allows access to system attributes and functions.
 */
class Sys : public File {
public:

#if defined __link
	Sys(link_transport_mdriver_t * driver);
#else
	Sys();
#endif

	/*! \details Returns a c style string pointer
	  * to the API version.
	  *
	  * This version is 2.4.0
	  *
	  */
	static const char * version(){ return "2.4.0"; }

	/*! \details Options for launching applications. */
	enum {
		LAUNCH_OPTIONS_FLASH /*! Install in flash memory */ = APPFS_FLAG_IS_FLASH,
		LAUNCH_OPTIONS_RAM /*! Install in ram memory */ = 0,
		LAUNCH_OPTIONS_STARTUP /*! Run at startup (must be in flash) */ = APPFS_FLAG_IS_STARTUP,
		LAUNCH_OPTIONS_ROOT /*! Run as root (if applicable) */ = APPFS_FLAG_IS_ROOT,
		LAUNCH_OPTIONS_REPLACE /*! Delete if application exists */ = APPFS_FLAG_IS_REPLACE,
		LAUNCH_OPTIONS_ORPHAN /*! Allow app to become an orphan */ = APPFS_FLAG_IS_ORPHAN,
		LAUNCH_OPTIONS_UNIQUE_NAMES /*! Create a unique name on install */ = APPFS_FLAG_IS_UNIQUE,
		LAUNCH_RAM_SIZE_DEFAULT = 0
	};

	/*! \details Launches a new application.
	 *
	 * @param path The path to the application
	 * @param exec_dest A pointer to a buffer where the execution path will be written (null if not needed)
	 * @param args The arguments to pass to the applications
	 * @param options For example:  LAUNCH_OPTIONS_FLASH | LAUNCH_OPTIONS_STARTUP
	 * @param ram_size The amount of RAM that will be used by the app
	 * @param update_progress A callback to show the progress if the app needs to be installed (copied to flash/RAM)
	 * @param envp Not used (set to zero)
	 * @return The process ID of the new app if successful
	 *
	 * This method must be called locally in an app. It can't be executed over the link protocol.
	 */
	static int launch(const var::ConstString & path,
							const var::ConstString & args,
							int options = 0, //run in RAM, discard on exit
							int ram_size = LAUNCH_RAM_SIZE_DEFAULT
			);

	static int launch(const var::ConstString & path,
							const var::ConstString & args,
							var::String & exec_destination,
							int options, //run in RAM, discard on exit
							int ram_size,
							const sys::ProgressCallback * progress_callback,
							const var::ConstString & envp = ""
			);

	static var::String install(const var::ConstString & path,
							int options = 0, //run in RAM, discard on exit
							int ram_size = LAUNCH_RAM_SIZE_DEFAULT
			);

	static var::String install(const var::ConstString & path,
							int options, //run in RAM, discard on exit
							int ram_size,
							const sys::ProgressCallback * progress_callback
			);



	/*! \details Frees the RAM associated with the app without deleting the code from flash
	 * (should not be called when the app is currently running).
	 *
	 * @param path The path to the app (use \a exec_dest from launch())
	 * @param driver Used with link protocol only
	 * @return Zero on success
	 *
	 * This method can causes problems if not used correctly. The RAM associated with
	 * the app will be free and available for other applications. Any applications
	 * that are using the RAM must quit before the RAM can be reclaimed using reclaim_ram().
	 *
	 * \sa reclaim_ram()
	 */
	static int free_ram(const char * path, link_transport_mdriver_t * driver = 0);

	/*! \details Reclaims RAM that was freed using free_ram().
	 *
	 * @param path The path to the app
	 * @param driver Used with link protocol only
	 * @return Zero on success
	 *
	 * \sa free_ram()
	 */
	static int reclaim_ram(const char * path, link_transport_mdriver_t * driver = 0);


	static void assign_zero_sum32(void * data, int size);
	static int verify_zero_sum32(void * data, int size);


#if !defined __link

	/*! \details Gets the version (system/board version).
	 *
	 * @param version The destination string for the version
	 * @return Zero on success
	 */
	static int get_version(var::String & version);
	static var::String get_version();

	/*! \details Gets the version (kernel version).
	 *
	 * @param version The destination string for the version
	 * @return Zero on success
	 */
	static int get_kernel_version(var::String & version);

	/*! \details Puts the kernel in powerdown mode.
	 *
	 * @param timeout_msec The number of milliseconds before the
	 * device will power on (reset).  If this isn't supported,
	 * the device will power off until reset by an external signal
	 */
	static void powerdown(int timeout_msec = 0);

	/*! \details Puts the kernel in hibernate mode.
	 *
	 * @param timeout_msec The number of milliseconds before the
	 * device will wake up from hibernation.  If this isn't supported,
	 * the device will stay in hibernation until woken up externally
	 */
	static int hibernate(int timeout_msec = 0);

	/*! \details Executes a kernel request.
	 *
	 * @param req The request number
	 * @param arg Argument pointer
	 * @return The result of the execution of the request. (-1 if request is not available)
	 *
	 * The kernel request must
	 * be defined and implemented by the board support package.
	 */
	static int request(int req, void * arg = 0);

	/*! \details Request a kernel install library's API.
	 *
	 * @param request The API request value (ie SAPI_API_REQUEST_ARM_DSP)
	 * @return A pointer to the api_t.
	 *
	 * This method will request a library API from the kernel. If
	 * the library is install in the kernel. This method will return
	 * a pointer to the api. Otherwise, zero will be retured. It is
	 * possible for the application to link directly to the library
	 * if the library is not provided by the kernel.
	 *
	 */
	template<typename T> static const T * request_api(int request){
		return (const T*)::kernel_request_api(request);
	}

	/*! \details Forces a reset of the device. */
	static void reset();

	/*! \details Loads the board configuration provided as
	 * part of the board support package.
	 *
	 * @param config A reference to the destination object
	 * @return Zero on success
	 *
	 * The object must be opened before calling this method.
	 *
	 * \sa open()
	 */
	int get_board_config(sos_board_config_t & config);
#endif

	/*! \details Opens /dev/sys.
	 *
	  * @return Less than zero for an error
	 *
	 */
	int open(){
		return File::open("/dev/sys", RDWR);
	}

	/*! \details Loads the current system info.
	 *
	 * @param attr A reference to where the data should be stored
	 * @return Zero on success
	 *
	 * The object must be opened before calling this method.
	 *
	 * \sa open()
	 *
	 */
	int get_info(sys_info_t & attr);

	/*! \cond */
	using File::open;
	int get_23_info(sys_23_info_t & attr);
	int get_26_info(sys_26_info_t & attr);


	//these are deprecated: use sys::Task instead
	int get_taskattr(sys_taskattr_t & attr, int task = -1);
	inline int get_taskattr(sys_taskattr_t * attr, int task = -1){
		return get_taskattr(*attr, task);
	}

	int current_task() const { return m_current_task; }
	void set_current_task(int v){ m_current_task = v; }
	/*! \endcond */

	/*! \details Loads the cloud kernel ID.
	 *
	 * @param id A reference to the destination data
	 * @return Less than zero if the operation failed
	 *
	 * The object must be opened before calling this method.
	 *
	 */
	int get_id(sys_id_t & id);

#if !defined __link
	/*! \details Redirects the standard output to the file specified.
	 *
	 * @param fd The file descriptor where the standard output should be directed.
	 *
	 * The file desriptor should be open and ready for writing. For example,
	 * to redirect the standard output to the UART:
	 *
	 * \code
	 * #include <sapi/sys.hpp>
	 * #include <sapi/hal.hpp>
	 *
	 * Uart uart(0);
	 * uart.init(); //initializes uart using default settings (if available)
	 * Sys::redirect_stdout( uart.fileno() );
	 * printf("This will be written to UART0\n");
	 * \endcode
	 *
	 *
	 */
	static void redirect_stdout(int fd){
		_impure_ptr->_stdout->_file = fd;
	}

	/*! \details Redirects the standard input from the specified file descriptor.
	 *
	 * @param fd The open and readable file descriptor to use for standard input
	 *
	 * See Sys::redirect_stdout() for an example.
	 *
	 */
	static void redirect_stdin(int fd){
		_impure_ptr->_stdin->_file = fd;
	}

	/*! \details Redirects the standard error from the specified file descriptor.
	 *
	 * @param fd The open and writable file descriptor to use for standard input
	 *
	 * See Sys::redirect_stdout() for an example.
	 *
	 */
	static void redirect_stderr(int fd){
		_impure_ptr->_stderr->_file = fd;
	}
#endif


private:
	/*! \cond */
	int m_current_task;
	/*! \endcond */

};

}

#endif /* SYS_SYS_HPP_ */
