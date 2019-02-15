/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


#ifndef SYS_FILEINFO_HPP_
#define SYS_FILEINFO_HPP_


#include "File.hpp"
#include "../api/SysObject.hpp"
#include "../var/ConstString.hpp"

namespace sys {


/*! \brief FileInfo Class
 * \details The FileInfo class is basically
 * a wrapper of `struct stat` and provides
 * information on the type and size
 * of the file.
 *
 */
class FileInfo : public api::SysWorkObject {
public:

#if defined __link
	FileInfo(link_transport_mdriver_t * driver = 0);
#else
	FileInfo();
#endif

	/*! Gets the file info for the specified path.
	 * @param path The file path (can be a file, directory or device)
	 * @return Zero on success or less than zero with errno set
	 */
	int get_info(const var::ConstString & path);


	int get_info(int file_no);


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

#ifdef __link
	void set_driver(link_transport_mdriver_t * driver){
		m_driver = driver;
	}
#endif

private:

#ifdef __link
	struct link_stat m_stat;
	link_transport_mdriver_t * m_driver;
	bool m_is_local;
#else
	struct stat m_stat;
#endif


};

} /* namespace sys */

#endif /* SYS_FILEINFO_HPP_ */
