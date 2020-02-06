/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_FS_DIR_HPP_
#define SAPI_FS_DIR_HPP_

#include <functional>

#ifdef __link
#include <sos/link.h>

#if defined __win32
#include <winsock2.h>
#include <windows.h>
#if !defined FALSE
#define FALSE 0
#endif
#if !defined TRUE
#define TRUE 1
#endif
#include "dirent_windows.h"
#undef ERROR
#else
#include <dirent.h>
#endif


#else
#include <dirent.h>
#endif

#include "../api/FsObject.hpp"
#include "../var/Vector.hpp"
#include "../var/String.hpp"
#include "../var/ConstString.hpp"
#include "../arg/Argument.hpp"
#include "File.hpp"
#include "Stat.hpp"

namespace fs {

/*! \brief Dir class
 *
 * \todo Add some examples
 *
 *
 */
class Dir : public api::WorkObject {
public:

	using SourcePath = File::SourcePath;
	using DestinationPath = File::DestinationPath;
	using IsRecursive = arg::Argument<bool, struct IsRecursiveTag>;
	using IsOverwrite = File::IsOverwrite;
	using Location = File::Location;

#if defined __link
	using LinkDriver = File::LinkDriver;
	using ImplicitLinkDriver = File::ImplicitLinkDriver;
	using SourceLinkDriver = File::SourceLinkDriver;
	using DestinationLinkDriver = File::DestinationLinkDriver;
#endif

	/*! \details Constructs a Dir object. */
	Dir(
			SAPI_LINK_DRIVER_NULLPTR
			);
	static int create(
			const var::String & path,
			const Permissions & permissions = Permissions(0777)
			SAPI_LINK_DRIVER_NULLPTR_LAST
			);

	static int create(
			const var::String & path,
			const Permissions & permissions,
			IsRecursive is_recursive
			SAPI_LINK_DRIVER_NULLPTR_LAST
			);

	static bool exists(
			const var::String & path
			SAPI_LINK_DRIVER_NULLPTR_LAST
			);

	static const var::String filter_hidden(const var::String & entry){
		if( !entry.is_empty() && entry.front() == '.' ){
			return var::String();
		}
		return entry;
	}

	static var::Vector<var::String> read_list(
			const var::String & path,
			IsRecursive is_recursive = IsRecursive(false)
			SAPI_LINK_DRIVER_NULLPTR_LAST
			);

	static var::Vector<var::String> read_list(
			const var::String & path,
			std::function<const var::String(const var::String & entry)> filter,
			IsRecursive is_recursive = IsRecursive(false)
			SAPI_LINK_DRIVER_NULLPTR_LAST
			);

	static int copy(
			SourcePath source_path,
			DestinationPath destination_path,
			const sys::ProgressCallback * progress_callback
		#if defined __link
			, SourceLinkDriver source_driver = SourceLinkDriver(nullptr),
			DestinationLinkDriver destination_driver = DestinationLinkDriver(nullptr)
		#endif
			);

	/*! \details Destructs the object.
	 *
	 * If the object has a directory that is
	 * currently open, the directory will
	 * be closed upon destruction.
	 *
	 */
	~Dir();

	/*! \details Opens a directory. */
	int open(const var::String & path);
	/*! \details Closes the directory.
	 *
	 * If this method is not called explicitly before
	 * the object is destroyed, it will be called
	 * during destruction. See ~Dir().
	 *
	 */
	int close();

	/*! \details Returns true if the directory is open. */
	bool is_open() const { return m_dirp != 0; }

	/*! \details Returns a pointer to the next entry or 0 if no more entries exist.
	 */
	const char * read();

	/*! \details Removes a directory.
	 *
	 * @return Zero on success or -1 for an error
	 */
	static int remove(
			const var::String & path,
			IsRecursive recursive
			SAPI_LINK_DRIVER_NULLPTR_LAST
			);

	/*! \details Gets the next entry and writes the full path of the entry to the given string.
	 *
	 * @param path_dest The var::String that will hold the full path of the next entry.
	 * @return True if an entry was read or false for an error or no more entries
	 */
	bool get_entry(var::String & path_dest);

	var::String get_entry();

	/*! \details Returns a list of all
	 * the entries in the directory.
	 *
	 *
	 * ```
	 * #include <sapi/fs.hpp>
	 * #include <sapi/var.hpp>
	 *
	 *
	 * Dir d;
	 *
	 * d.open(arg::SourceDirectoryPath("/home");
	 * Vector<String> list = d.read_list();
	 * d.close();
	 *
	 * for(u32 i=0; i < list.count(); i++){
	 *   printf("Entry is %s\n", list.at(i).cstring());
	 * }
	 *
	 * ```
	 *
	 *
	 *
	 */
	var::Vector<var::String> read_list(
			IsRecursive is_recursive = IsRecursive(false)
			);

	var::Vector<var::String> read_list(
			std::function<const var::String(const var::String & entry)> filter,
			IsRecursive is_recursive = IsRecursive(false)
			);



	/*! \details Returns a pointer (const) to the name of the most recently read entry. */
	const char * name(){
		return m_entry.d_name;
	}

	/*! \details Returns a pointer (editable) to the name of the most recently read entry. */
	char * data(){
		return m_entry.d_name;
	}

	/*! \details Returns the serial number of the most recently read entry. */
	int ino(){
		return m_entry.d_ino;
	}

#ifndef __link
	/*! \details Returns the directory handle pointer. */
	DIR * dirp(){ return m_dirp; }
	/*! \details Counts the total number of entries in the directory. */
	int count();
	/*! \details Rewinds the directory pointer. */
	void rewind(){
		if( m_dirp ) {
			rewinddir(m_dirp);
		}
	}
	/*! \details Seeks to a location in the directory.
	 *
	 * Each entry in the directory occupies 1 location
	 * space. The first entry is at location 0.
	 *
	 *
	 */
	void seek(Location location){
		if( m_dirp ) {
			seekdir(m_dirp, location.argument());
		}
	}

	/*! \details Returns the current location in the directory.
	 *
	 *
	 */
	inline long tell(){
		if( m_dirp ){
			return telldir(m_dirp);
		}
		return 0;
	}
#else
	Dir & set_driver(ImplicitLinkDriver link_driver){
		m_driver = link_driver.argument();
		return *this;
	}
#endif

private:
#ifdef __link
	int m_dirp;
	struct link_dirent m_entry;
	link_transport_mdriver_t * m_driver;
	link_transport_mdriver_t * driver(){ return m_driver; }

	DIR * m_dirp_local;
	struct dirent m_entry_local;

#else
	DIR * m_dirp;
	struct dirent m_entry;
#endif

	var::String m_path;


};

}

#endif /* SAPI_FS_DIR_HPP_ */
