/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef KERNEL_HPP_
#define KERNEL_HPP_

#include <sos/sos.h>
#include <sos/dev/sys.h>
#include <sos/link.h>
#include "../hal/Dev.hpp"


namespace sys {

/*! \brief Sys Class
 * \details This class allows access to system attributes and functions.
 */
class Sys : public File {
public:
	Sys();

	enum {
		LAUNCH_OPTIONS_FLASH /*! Install in flash memory */ = APPFS_FLAG_IS_FLASH,
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
	static int launch(const char * path,
			char * exec_dest = 0,
			const char * args = 0,
			int options = 0, //run in RAM, discard on exit
			int ram_size = LAUNCH_RAM_SIZE_DEFAULT,
			int (*update_progress)(int, int) = 0,
			char *const envp[] = 0
	);

	/*! \details Frees the RAM associated with the app without deleting the code from flash
	 * (should not be called when the app is currently running).
	 *
	 * @param path The path to the app (use \a exec_dest from launch())
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
	 * @return Zero on success
	 *
	 * \sa free_ram()
	 */
	static int reclaim_ram(const char * path, link_transport_mdriver_t * driver = 0);


#if !defined __link

	/*! \details Gets the version (system/board version).
	 *
	 * @param version The destination string for the version
	 * @return Zero on success
	 */
	static int get_version(var::String & version);

	/*! \details Gets the version (kernel version).
	 *
	 * @param version The destination string for the version
	 * @return Zero on success
	 */
	static int get_kernel_version(var::String & version);

	/*! \details Puts the kernel in powerdown mode.
	 *
	 * @param timeout_ms The number of milliseconds before the
	 * device will power on (reset).  If this isn't supported,
	 * the device will power off until reset by an external signal
	 */
	static void powerdown(int timeout_ms = 0);

	/*! \details Puts the kernel in hibernate mode.
	 *
	 * @param timeout_ms The number of milliseconds before the
	 * device will wake up from hibernation.  If this isn't supported,
	 * the device will stay in hibernation until woken up externally
	 */
	static int hibernate(int timeout_ms = 0);

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
	 * @return Zero on success
	 *
	 */
	int open(){
		return File::open("/dev/sys", RDWR);
	}

	using File::open;

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
	int get_23_info(sys_23_info_t & attr);
	int get_26_info(sys_26_info_t & attr);


	/*! \details Loads the kernel's task attributes.
	 *
	 * @param attr A reference to the destination object
	 * @param task Which task to load (-1 to auto-increment)
	 * @return 1 if task is active, 0 if task is not being used, -1 is task is invalid
	 *
	 * The object must be opened before calling this method.
	 *
	 */
	int get_taskattr(sys_taskattr_t & attr, int task = -1);

	inline int get_taskattr(sys_taskattr_t * attr, int task = -1){
		return get_taskattr(*attr, task);
	}

	/*! \details Returns the current task that is accessed by get_taskattr().
	 */
	int current_task() const { return m_current_task; }

	/*! \details Sets the current task to read using get_taskattr(). */
	void set_current_task(int v){ m_current_task = v; }

	/*! \details Loads the cloud kernel ID.
	 *
	 * @param A reference to the destination data
	 * @return Less than zero if the operation failed
	 *
	 * The object must be opened before calling this method.
	 *
	 */
	int get_id(sys_id_t & id);

private:
	int m_current_task;

};
};

#endif /* KERNEL_HPP_ */
