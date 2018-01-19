/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


#ifndef SYS_FILEINFO_HPP_
#define SYS_FILEINFO_HPP_

namespace sys {

class FileInfo {
public:
#if defined __link
	FileInfo(link_transport_mdriver_t * d);
#else
	FileInfo();
#endif
	/*! Gets the file info for the specified path.
	 *
	 * @param path The file path (can be a file, directory or device)
	 * @return Zero on success or less than zero with errno set
	 */
	int get_info(const char * path);


	/*! \details Returns true if the file is a directory. */
	bool is_dir() const;

	/*! \details Returns true if the file is a regular file. */
	bool is_file() const;

	/*! \details Returns true if the file is a device (block or character). */
	bool is_device() const;

	/*! \details Returns true if the file is a block device. */
	bool is_block_device() const;

	/*! \details Returns true if the file is a character device. */
	bool is_char_device() const;

	/*! \details Returns the size of the file in bytes.
	 *
	 * This method is only valid for regular files (ie is_file() must return true).
	 */
	u32 size() const;

	/*! \details Returns true if the file is executable. */
	bool is_executable() const;


	u32 mode() const { return m_stat.st_mode; }

private:

#ifdef __link
	struct link_stat m_stat;
	link_transport_mdriver_t * m_driver;
#else
	struct stat m_stat;
#endif


};

} /* namespace sys */

#endif /* SYS_FILEINFO_HPP_ */
