/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef APPFS_HPP_
#define APPFS_HPP_

#include <sos/link.h>
#include "../api/SObject.hpp"
#include "../var/String.hpp"

namespace sys {

/*! \brief Application File System Class
 * \details This class provides an interface for creating data files in flash
 * memory.
 *
 * The following is a basic example of creating a data file in flash and then reading from it.
 *
 * \code
 * #include <sapi/sys.hpp>
 *
 * int main(int argc, char * argv[]){
 * 	char my_user_data[16];
 * 	//populate my_user_data as you please here
 * 	sprintf(my_user_data, "Hello World!");
 * 	Appfs::create("my_data", my_user_data, 16); //creates /app/flash/my_data as read only data
 * }
 * \endcode
 *
 * Now the file can be read just like any other file would be.  Of
 * course, reading files from /app/flash is very fast compared
 * to filesystems built on external chips.
 *
 * \code
 * #include <sapi/sys.hpp>
 *
 * int main(int argc, char * argv[]){
 * 	char my_user_data[16];
 * 	File f;
 * 	f.open("/app/flash/my_data", File::RDONLY); //only the file read only
 * 	f.read(my_user_data, 16); //read 16 bytes of user data
 * 	f.close();  //free resources
 * 	return 0;
 * }
 * \endcode
 *
 */
class Appfs {
public:

	/*! \details Creates a file in flash memory consisting
	 * of the data specified.
	 *
	 * @param name The name of the data file (no path info)
	 * @param buf A pointer to the data to be saved
	 * @param nbyte The number of bytes to save
	 * @param mount The mount path (default is /app)
	 * @param update A callback that is executed after each page write
	 * @param context The first argument passed to the \a update callback
	 * @return Zero on success or -1 with errno set accordingly
	 *
	 */
#if !defined __link
	static int create(const char * name,
			const void * buf,
			int nbyte,
			const char * mount = "/app",
			bool (*update)(void *, int, int) = 0,
			void * context = 0);
#else
	static int create(const char * name,
			const void * buf,
			int nbyte,
			const char * mount = "/app",
			bool (*update)(void *, int, int) = 0,
			void * context = 0,
			link_transport_mdriver_t * driver = 0);
#endif


	/*! \details Returns the page size for writing data. */
	static int page_size(){ return APPFS_PAGE_SIZE; }

	/*! \details Gets the info associated with the file.
	 *
     * @param path The path to the file
	 * @param info A reference to the destination info
	 * @return Zero on success
     *
     * This method will work if the file is installed in the
     * application filesystem or on a normal data filesystem.
     *
	 */
#if !defined __link
	static int get_info(const char * path, appfs_info_t & info);
#else
	static int get_info(const char * path, appfs_info_t & info, link_transport_mdriver_t * driver);
#endif

	/*! \details Gets the application version.
	 *
	 * @param path The path to the file (must be in the /app folder)
	 * @return The BCD version of the file
	 *
	 * For example, the BCD representation of version "1.1" is 0x0101.
	 *
	 */
#if !defined __link
	static u16 get_version(const char * path);
#else
	static u16 get_version(const char * path, link_transport_mdriver_t * driver);
#endif

	/*! \details Gets the application ID value.
	 *
	 * @param path The path to the file (must be in the /app folder)
	 * @param id A pointer to the destination data for the ID
	 * @param capacity The number of bytes available in \a id
	 * @return Zero on success
	 *
	 *
	 */
#if !defined __link
	static int get_id(const char * path, char * id, u32 capacity);
#else
	static int get_id(const char * path, char * id, u32 capacity, link_transport_mdriver_t * driver);
#endif

	/*! \details Gets the application ID value.
	 *
	 * @param path The path to the application
	 * @param id A var::String reference that will store the ID
	 * @return Zero on success
	 */
#if !defined __link
	static int get_id(const char * path, var::String & id){
		return get_id(path, id.cdata(), id.capacity());
	}
#endif

#if !defined __link
	static int cleanup(bool data = false);
#endif
};

};

#endif /* APPFS_HPP_ */
