/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef KERNEL_HPP_
#define KERNEL_HPP_

#include <Hal/Dev.hpp>
#include <iface/dev/sys.h>
#include <iface/link.h>


namespace Sys {

class Kernel : public Hal::Dev {
public:
	Kernel();

	enum {
		LAUNCH_OPTIONS_FLASH /*! \brief Install in flash memory */ = LINK_APPFS_EXEC_OPTIONS_FLASH,
		LAUNCH_OPTIONS_STARTUP /*! \brief Run at startup (must be in flash) */ = LINK_APPFS_EXEC_OPTIONS_STARTUP,
		LAUNCH_OPTIONS_ROOT /*! \brief Run as root (if applicable) */ = LINK_APPFS_EXEC_OPTIONS_ROOT,
		LAUNCH_OPTIONS_REPLACE /*! \brief Delete if application exists */ = LINK_APPFS_EXEC_OPTIONS_REPLACE,
		LAUNCH_OPTIONS_ORPHAN /*! \brief Allow app to become an orphan */ = LINK_APPFS_EXEC_OPTIONS_ORPHAN,
		LAUNCH_OPTIONS_UNIQUE_NAMES /*! \brief Create a unique name on install */ = LINK_APPFS_EXEC_OPTIONS_UNIQUE,
		LAUNCH_RAM_SIZE_DEFAULT = 0
	};

	/*! \details Launch a new application
	 *
	 * @param path The path to the application
	 * @param exec_dest A pointer to a buffer where the execution path will be written (null if not needed)
	 * @param args The arguments to pass to the applications
	 * @param options For example:  LAUNCH_OPTIONS_FLASH | LAUNCH_OPTIONS_STARTUP
	 * @param ram_size The amount of RAM that will be used by the app
	 * @param update_progress A callback to show the progress if the app needs to be installed (copied to flash/RAM)
	 * @param envp Not used (set to zero)
	 * @return The process ID of the new app if successful
	 */
	static int launch(const char * path,
			char * exec_dest = 0,
			const char * args = 0,
			int options = 0, //run in RAM, discard on exit
			int ram_size = LAUNCH_RAM_SIZE_DEFAULT,
			int (*update_progress)(int, int) = 0,
			char *const envp[] = 0
	);

	/*! \details This method will free the RAM associated with the app without deleting the code from flash.
	 * This should not be called when the app is currently running.
	 *
	 * @param path The path to the app (use \a exec_dest from launch())
	 * @return Zero on success
	 *
	 * \sa reclaim_ram()
	 */
	static int free_ram(const char * path, link_transport_mdriver_t * driver = 0);

	/*! \details Reclaim RAM that was freed using free_ram()
	 *
	 * @param path The path to the app
	 * @return Zero on success
	 *
	 * \sa free_ram()
	 */
	static int reclaim_ram(const char * path, link_transport_mdriver_t * driver = 0);

#if !defined __link
	/*! \details Put the kernel in powerdown mode
	 *
	 * @param count The number of milliseconds before the
	 * device will power on (reset).  If this isn't supported,
	 * the device will power off until reset by an external signal
	 */
	static void powerdown(int count = 0);

	/*! \details Put the kernel in hibernate mode
	 *
	 * @param count The number of milliseconds before the
	 * device will wake up from hibernation.  If this isn't supported,
	 * the device will stay in hibernation until woken up externally
	 */
	static int hibernate(int count = 0);
#endif

	/*! \details This will open /dev/sys
	 *
	 * @param mode Usually Kernel::READWRITE
	 * @return Zero on success
	 */
	inline int open(){
		return Dev::open("/dev/sys", RDWR);
	}

	using Dev::open;

	/*! \details Load the current kernel attributes
	 *
	 * @param attr  A reference to where the data should be stored
	 * @return Zero on success
	 */
	int get_attr(sys_attr_t & attr);
	inline int attr(sys_attr_t * v){ return get_attr(*v); }

	/*! \details Load the kernel's task attributes
	 *
	 * @param attr Pointer to the destination attributes
	 * @param task Which task to load
	 * @return
	 */
	int get_taskattr(sys_taskattr_t * attr, int task = -1);

	inline int get_taskattr(sys_taskattr_t & attr, int task = -1){
		return get_taskattr(&attr, task);
	}



private:
	int current_task;

};
};

#endif /* KERNEL_HPP_ */
