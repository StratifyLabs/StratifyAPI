/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


#ifndef SAPI_SYS_FILEINFO_HPP_
#define SAPI_SYS_FILEINFO_HPP_


#include "../api/SysObject.hpp"
#include "../var/ConstString.hpp"

namespace sys {


/*! \brief FileInfo Class
 * \details The FileInfo class is basically
 * a wrapper of `struct stat` and provides
 * information on the type and size
 * of the file.
 *
 *
 * ```
 * #include <sapi/sys.hpp>
 *
 * FileInfo info;
 *
 * info.get_info("/home/test.txt");
 *
 * if( info.is_file() ){
 *   printf("Is a file\n");
 * } else if( info.is_directory() ){
 *   printf("Is a directory\n");
 * }
 *
 * File file;
 *
 * file.open("/home/test.txt", File::RDONLY);
 *
 * //grab the info from a file that is already open
 * info.get_info(file.fileno());
 * file.close();
 *
 * ```
 *
 */
class FileInfo : public api::SysInfoObject {
public:

#if defined __link
	FileInfo(bool is_local = false);
	FileInfo(const struct link_stat & st, bool is_local = false){
		m_is_local = is_local;
		m_stat = st;
	}
#else
	FileInfo();
	FileInfo(const struct stat & st){
		m_stat = st;
	}
#endif

	bool is_valid() const {
		return m_stat.st_mode != 0;
	}


	/*! \details Returns true if the file is a directory. */
	bool is_directory() const;

	/*! \details Returns true if the file is a regular file. */
	bool is_file() const;

	/*! \details Returns true if the file is a device (block or character). */
	bool is_device() const;

	/*! \details Returns true if the file is a block device. */
	bool is_block_device() const;

	/*! \details Returns true if the file is a character device. */
	bool is_character_device() const;

	/*! \details Returns true if the file is a socket. */
	bool is_socket() const;

	/*! \details Returns true if the file is a FIFO. */
	bool is_fifo() const;

	/*! \details Returns the size of the file in bytes.
	 *
	 * This method is only valid for regular files (ie is_file() must return true).
	 */
	u32 size() const;

	/*! \details Returns true if the file is executable. */
	bool is_executable() const;

	/*! \details Returns the file mode value. */
	u32 mode() const { return m_stat.st_mode; }

private:

#ifdef __link
	struct link_stat m_stat;
	bool m_is_local;
#else
	struct stat m_stat;
#endif


};

} /* namespace sys */

#endif /* SAPI_SYS_FILEINFO_HPP_ */
