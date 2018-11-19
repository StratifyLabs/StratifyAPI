/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef APPFS_HPP_
#define APPFS_HPP_

#include <sos/link.h>
#include "../api/WorkObject.hpp"
#include "../var/String.hpp"
#include "../api/SysObject.hpp"
#include "../var/ConstString.hpp"
#include "ProgressCallback.hpp"
#include "File.hpp"

namespace sys {

class AppfsFileAttributes : public api::SysInfoObject {
public:

	enum {
		IS_FLASH = APPFS_FLAG_IS_FLASH,
		IS_STARTUP = APPFS_FLAG_IS_STARTUP,
		IS_ROOT = APPFS_FLAG_IS_ROOT,
		IS_REPLACE = APPFS_FLAG_IS_REPLACE,
		IS_ORPHAN = APPFS_FLAG_IS_ORPHAN,
		IS_UNIQUE = APPFS_FLAG_IS_UNIQUE
	};

	AppfsFileAttributes(){
		m_version = 0;
		m_ram_size = 0;
		m_o_flags = APPFS_FLAG_IS_FLASH;
	}

	void apply(appfs_file_t * dest) const;

	void set_name(const var::ConstString & value){ m_name = value; }
	const var::String & name() const { return m_name; }
	void set_id(const var::ConstString & value){ m_id = value; }
	const var::String & id() const { return m_id; }
	void set_version(u16 value){ m_version = value; }
	u16 version() const { return m_version; }
	void set_flags(u32 o_value){ m_o_flags = o_value; }
	u16 o_flags() const { return m_o_flags; }
	void set_ram_size(u32 value){ m_ram_size = value; }
	u16 ram_size() const { return m_ram_size; }
	void set_startup(bool value = true){
		if( value ){
			m_o_flags |= IS_STARTUP;
		} else {
			m_o_flags &= ~IS_STARTUP;
		}
	}

	void set_flash(bool value = true){
		if( value ){
			m_o_flags |= IS_FLASH;
		} else {
			m_o_flags &= ~IS_FLASH;
		}
	}


private:
	var::String m_name;
	var::String m_id;
	u32 m_ram_size;
	u32 m_o_flags;
	u16 m_version;
};

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
class Appfs : public api::SysInfoObject {
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
	static int create(const var::ConstString & name,
							const sys::File & source_data,
							const var::ConstString & mount = "/app",
							const ProgressCallback * progress_callback = 0);

	static int create(const var::ConstString & name,
							const var::Data & data,
							const char * mount = "/app",
							const ProgressCallback * progress_callback = 0){
		return create(name, data.to_void(), data.size(), mount, progress_callback);
	}
#else
	static int create(const var::ConstString & name,
							const sys::File & source_data,
							const var::ConstString & mount = "/app",
							const ProgressCallback * progress_callback = 0,
							link_transport_mdriver_t * driver = 0);
#endif


	/*! \details Returns the page size for writing data. */
	static int page_size(){ return APPFS_PAGE_SIZE; }

	/*! \details Gets the info associated with an executable file.
	 *
	  * @param path The path to the file
	 * @param info A reference to the destination info
	 * @return Zero on success
	  *
	  * This method will work if the file is installed in the
	  * application filesystem or on a normal data filesystem.
	  *
	  * The method will return less than zero if the file is not
	  * a recognized executable file.
	  *
	  * The errno will be set to ENOENT or ENOEXEC if the file
	  * does not exist or is not a recognized executable, respectively.
	  *
	 */
#if !defined __link
	static int get_info(const var::ConstString & path, appfs_info_t & info);
#else
	static int get_info(const var::ConstString & path, appfs_info_t & info, link_transport_mdriver_t * driver);
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
	static u16 get_version(const var::ConstString & path);
#else
	static u16 get_version(const var::ConstString & path, link_transport_mdriver_t * driver);
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
	static var::String get_id(const var::ConstString & path);
#else
	static var::String get_id(const var::ConstString & path, link_transport_mdriver_t * driver);
#endif

#if !defined __link
	static int cleanup(bool data = false);
#endif
};

};

#endif /* APPFS_HPP_ */
