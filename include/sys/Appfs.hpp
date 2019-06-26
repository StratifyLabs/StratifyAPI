/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_SYS_APPFS_HPP_
#define SAPI_SYS_APPFS_HPP_

#include <sos/link.h>
#include "../api/WorkObject.hpp"
#include "../var/String.hpp"
#include "../api/SysObject.hpp"
#include "../var/ConstString.hpp"
#include "ProgressCallback.hpp"
#include "File.hpp"

namespace sys {


/*! \brief AppfsInfo Class
 * \details The AppfsInfo class is for
 * getting information associated with
 * executable files that are either installed
 * in or built for the application filesystem.
 *
 */
class AppfsInfo : public api::SysInfoObject {
public:

	/*! \details Constructs an empty object. */
	AppfsInfo(){ memset(&m_info, 0, sizeof(m_info)); }

	/*! \details Constructs an object from a *appfs_info_t* object. */
	AppfsInfo(const appfs_info_t & info){
		memcpy(&m_info, &info, sizeof(appfs_info_t));
	}

	/*! \details Returns true if the object is valid. */
	bool is_valid() const {
		return m_info.signature != 0;
	}

	/*! \details Returns the application ID (cloud id). */
	const var::ConstString id() const { return (const char*)m_info.id; }

	/*! \details Returns the name of the application. */
	const var::ConstString name() const { return (const char*)m_info.name; }

	/*! \details Returns the file mode. */
	u16 mode() const { return m_info.mode; }
	/*! \details Returns the version. */
	u16 version() const { return m_info.version; }

	/*! \details Returns the data RAM size used by the application. */
	u32 ram_size() const { return m_info.ram_size; }

	/*! \details Returns the flags.
	 *
	 * See also: is_executable(), is_startup(), is_flash(), is_orphan(), is_root(),
	 * is_unique().
	 *
	 */
	u32 o_flags() const { return m_info.o_flags; }

	/*! \details Returns the application signature.
	 *
	 * This value specifies the version of the table that
	 * connects the application to the operating system. Every
	 * OS package has a signature that is associated with
	 * the calls (such as printf(), pthread_create()) that are available to
	 * application installed on the system.
	 *
	 */
	u32 signature() const { return m_info.signature; }

	/*! \details Returns true if the application is executable. */
	bool is_executable() const { return m_info.mode & 0111; }

	/*! \details Returns true if the application runs at startup. */
	bool is_startup() const { return (m_info.o_flags & APPFS_FLAG_IS_STARTUP) != 0; }
	/*! \details Returns true if the application is to be installed in flash. */
	bool is_flash() const { return (m_info.o_flags & APPFS_FLAG_IS_FLASH) != 0; }
	/*! \details Returns true if the application code is to be installed in external memory. */
	bool is_code_external() const { return (m_info.o_flags & APPFS_FLAG_IS_CODE_EXTERNAL) != 0; }
	/*! \details Returns true if the application data is to be installed in external memory. */
	bool is_data_external() const { return (m_info.o_flags & APPFS_FLAG_IS_DATA_EXTERNAL) != 0; }
	/*! \details Returns true if the application code is to be installed in tightly coupled memory. */
	bool is_code_tightly_coupled() const { return (m_info.o_flags & APPFS_FLAG_IS_CODE_TIGHTLY_COUPLED) != 0; }
	/*! \details Returns true if the application data is to be installed in tightly coupled memory. */
	bool is_data_tightly_coupled() const { return (m_info.o_flags & APPFS_FLAG_IS_DATA_TIGHTLY_COUPLED) != 0; }
	/*! \details Returns true if the application should run as an orphan. */
	bool is_orphan() const { return (m_info.o_flags & APPFS_FLAG_IS_ORPHAN) != 0; }
	/*! \details Returns true if the application should run as root. */
	bool is_root() const { return (m_info.o_flags & APPFS_FLAG_IS_ROOT) != 0; }
	/*! \details Returns true if the application should create a unique instance.
	 *
	 * If unique is false, the system will not allow a second copy of
	 * the application to be installed on the system.
	 *
	 * If unique is true, the application will be assigned a unique
	 * name when it is installed in RAM or flash.
	 *
	 */
	bool is_unique() const { return (m_info.o_flags & APPFS_FLAG_IS_UNIQUE) != 0; }

	const appfs_info_t & info() const { return m_info; }
	appfs_info_t & info(){ return m_info; }


private:
	appfs_info_t m_info;
};


/*! \brief AppfsFileAttributes Class
 * \details The AppfsFileAttributes class holds the
 * information that is needed to modify an application
 * binary that has been built with the compiler.
 *
 * The compiler is unable to build some information
 * directly into the binary but it allocates space
 * for the information.
 *
 * This class is used for that information and includes
 * things like the application name, project id,
 * and execution flags.
 *
 *
 */
class AppfsFileAttributes : public api::SysInfoObject {
public:

	enum {
		IS_FLASH = APPFS_FLAG_IS_FLASH,
		IS_STARTUP = APPFS_FLAG_IS_STARTUP,
		IS_ROOT = APPFS_FLAG_IS_ROOT,
		IS_REPLACE = APPFS_FLAG_IS_REPLACE,
		IS_ORPHAN = APPFS_FLAG_IS_ORPHAN,
		IS_UNIQUE = APPFS_FLAG_IS_UNIQUE,
		IS_CODE_EXTERNAL = APPFS_FLAG_IS_CODE_EXTERNAL,
		IS_DATA_EXTERNAL = APPFS_FLAG_IS_DATA_EXTERNAL,
		IS_CODE_TIGHTLY_COUPLED = APPFS_FLAG_IS_CODE_TIGHTLY_COUPLED,
		IS_DATA_TIGHTLY_COUPLED = APPFS_FLAG_IS_DATA_TIGHTLY_COUPLED
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

	bool is_flash() const { return m_o_flags & IS_FLASH; }
	bool is_code_external() const { return m_o_flags & IS_CODE_EXTERNAL; }
	bool is_data_external() const { return m_o_flags & IS_DATA_EXTERNAL; }
	bool is_code_tightly_coupled() const { return m_o_flags & IS_CODE_TIGHTLY_COUPLED; }
	bool is_data_tightly_coupled() const { return m_o_flags & IS_DATA_TIGHTLY_COUPLED; }
	bool is_startup() const { return m_o_flags & IS_STARTUP; }
	bool is_unique() const { return m_o_flags & IS_UNIQUE; }

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

	void set_code_external(bool value = true){
		if( value ){
			m_o_flags |= IS_CODE_EXTERNAL;
		} else {
			m_o_flags &= ~IS_CODE_EXTERNAL;
		}
	}

	void set_data_external(bool value = true){
		if( value ){
			m_o_flags |= IS_DATA_EXTERNAL;
		} else {
			m_o_flags &= ~IS_DATA_EXTERNAL;
		}
	}

	void set_code_tightly_coupled(bool value = true){
		if( value ){
			m_o_flags |= IS_CODE_TIGHTLY_COUPLED;
		} else {
			m_o_flags &= ~IS_CODE_TIGHTLY_COUPLED;
		}
	}

	void set_data_tightly_coupled(bool value = true){
		if( value ){
			m_o_flags |= IS_DATA_TIGHTLY_COUPLED;
		} else {
			m_o_flags &= ~IS_DATA_TIGHTLY_COUPLED;
		}
	}

	void set_unique(bool value = true){
		if( value ){
			m_o_flags |= IS_UNIQUE;
		} else {
			m_o_flags &= ~IS_UNIQUE;
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
	static AppfsInfo get_info(const var::ConstString & path);
#else
	static AppfsInfo get_info(const var::ConstString & path, link_transport_mdriver_t * driver);
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

#endif /* SAPI_SYS_APPFS_HPP_ */
