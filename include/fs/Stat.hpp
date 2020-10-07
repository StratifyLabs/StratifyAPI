/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.


#ifndef SAPI_SYS_FILEINFO_HPP_
#define SAPI_SYS_FILEINFO_HPP_

#include <sos/link/types.h>

#if !defined __link
#include <unistd.h>
#define MCU_INT_CAST(var) ((void*)(u32)var)
#include <fcntl.h>
#else
#undef fileno
#define MCU_INT_CAST(var) ((void*)(u64)var)
#endif

#include "../api/FsObject.hpp"
#include "../var/ConstString.hpp"

namespace fs {

class FileFlags {
public:
	enum o_permission_flags {
		permission_public_execute = 0001,
		permission_public_write = 0002,
		permission_public_read = 0004,
		permission_group_execute = 0010,
		permission_group_write = 0020,
		permission_group_read = 0040,
		permission_owner_execute = 0100,
		permission_owner_write = 0200,
		permission_owner_read = 0400,
	};

	enum o_open_flags {
		open_read_only /*! Open as read-only */ = LINK_O_RDONLY,
		open_write_only /*! Open as write-only */ = LINK_O_WRONLY,
		open_create /*! Create when opening (files) */ = LINK_O_CREAT,
		open_truncate /*! Truncate when opening (files) */ = LINK_O_TRUNC,
		open_append /*! Append when opening (files)*/ = LINK_O_APPEND,
		open_exclusive /*! Create exclusively (files) */ = LINK_O_EXCL,
		open_read_write /*! Open as read-write */ = LINK_O_RDWR,
		open_non_block /*! Open as non-blocking */ = LINK_O_NONBLOCK,
		open_no_delay /*! Open as non-blocking */ = LINK_O_NONBLOCK,
		open_access_mode /*! Access mode mask */ = LINK_O_ACCMODE
	};

	enum o_access_flags {
		access_read_ok /*! Check if read access is allowed */ = LINK_R_OK,
		access_write_ok /*! Check if write access is allowed */ = LINK_W_OK,
		access_execute_ok /*! Check if execute access is allowed */ = LINK_X_OK,
		access_file_ok /*! Check if file exists */ = LINK_F_OK,
	};

	enum mode {
		mode_format /*! Mode format mask */ = LINK_S_IFMT,
		mode_file_socket /*! Mode Socket mask */ = LINK_S_IFSOCK,
		mode_regular /*! Mode regular file value */ = LINK_S_IFREG,
		mode_block /*! Mode block device value */ = LINK_S_IFBLK,
		mode_character /*! Mode character device value */ = LINK_S_IFCHR,
		mode_directory /*! Mode directory value */ = LINK_S_IFDIR,
		mode_fifo /*! Mode FIFO value */ = LINK_S_IFDIR,
		mode_symbolic_link /*! Mode symbolic link value */ = LINK_S_IFLNK
	};

};

API_OR_NAMED_FLAGS_OPERATOR(FileFlags, o_permission_flags)
API_OR_NAMED_FLAGS_OPERATOR(FileFlags, o_open_flags)
API_OR_NAMED_FLAGS_OPERATOR(FileFlags, o_access_flags)

class Permissions : public FileFlags {
public:
	explicit Permissions(int mode = 0666){ m_permissions = mode; }

	static Permissions all_access(){ return Permissions(0777); }
	static Permissions read_only(){ return Permissions(0444); }
	static Permissions write_only(){ return Permissions(0222); }

	bool is_owner_read() const { return m_permissions & permission_owner_read; }
	bool is_owner_write() const { return m_permissions & permission_owner_write; }
	bool is_owner_execute() const { return m_permissions & permission_owner_execute; }
	bool is_public_read() const { return m_permissions & permission_public_read; }
	bool is_public_write() const { return m_permissions & permission_public_write; }
	bool is_public_execute() const { return m_permissions & permission_public_execute; }

	Permissions & set_owner_read(){ m_permissions |= permission_owner_read; return *this; }
	Permissions & set_owner_write(){ m_permissions |= permission_owner_write; return *this; }
	Permissions & set_owner_execute(){ m_permissions |= permission_owner_execute; return *this; }

	Permissions & set_public_read(){ m_permissions |= permission_public_read; return *this; }
	Permissions & set_public_write(){ m_permissions |= permission_public_write; return *this; }
	Permissions & set_public_execute(){ m_permissions |= permission_public_execute; return *this; }

	bool operator == (const Permissions & a) const { return a.m_permissions == m_permissions; }

	int permissions() const { return m_permissions; }

private:
	int m_permissions;
};

class OpenFlags : public FileFlags {
public:
	explicit OpenFlags(){ m_flags = 0; }

	static OpenFlags create() { return OpenFlags(open_create | open_read_write); }
	static OpenFlags create_append_write_only() { return OpenFlags(open_create | open_write_only | open_append); }
	static OpenFlags create_truncate() { return OpenFlags(open_create | open_read_write | open_truncate); }
	static OpenFlags append() { return OpenFlags(open_read_write | open_append); }
	static OpenFlags append_read_write() { return OpenFlags(open_read_write | open_append); }
	static OpenFlags read_write() { return OpenFlags(open_read_write); }
	static OpenFlags read_only() { return OpenFlags(open_read_only); }
	static OpenFlags write_only() { return OpenFlags(open_write_only); }
	static OpenFlags append_write_only() { return OpenFlags(open_write_only | open_append); }

	bool is_read_only() const { return access() == open_read_only; }
	bool is_write_only() const { return access() == open_write_only; }
	bool is_read_write() const { return access() == open_read_write; }
	int access() const { return m_flags & open_access_mode; }

	bool is_create() const { return m_flags & open_create; }
	bool is_exclusive() const { return m_flags & open_exclusive; }
	bool is_truncate() const { return m_flags & open_truncate; }
	bool is_append() const { return m_flags & open_append; }
	bool is_non_blocking() const { return m_flags & open_non_block; }


	OpenFlags & set_read_only(){ clear_access(); m_flags |= open_read_only; return *this; }
	OpenFlags & set_write_only(){ clear_access(); m_flags |= open_write_only; return *this; }
	OpenFlags & set_read_write(){ clear_access(); m_flags |= open_read_write; return *this; }
	OpenFlags & set_create(){ m_flags |= open_create; return *this; }
	OpenFlags & set_truncate(){ m_flags |= open_truncate; return *this; }
	OpenFlags & set_append(){ m_flags |= open_append; return *this; }
	OpenFlags & set_exclusive(){ m_flags |= open_exclusive; return *this; }
	OpenFlags & set_non_blocking(){ m_flags |= open_non_block; return *this; }

	int o_flags() const { return m_flags; }

private:
	explicit OpenFlags(int flags){ m_flags = flags; }

	void clear_access(){ m_flags &= ~LINK_O_ACCMODE; }

	int m_flags;
};

class Access : public FileFlags {
public:
	explicit Access(enum o_access_flags access = access_read_ok | access_write_ok | access_file_ok | access_execute_ok ){ m_access = access; }

	bool is_read_ok() const { return m_access & access_read_ok; }
	bool is_write_ok() const { return m_access & access_write_ok; }
	bool is_file_ok() const { return m_access & access_file_ok; }
	bool is_execute_ok() const { return m_access & access_execute_ok; }

	Access & set_read_ok(){ m_access |= access_read_ok; return *this; }
	Access & set_write_ok(){ m_access |= access_write_ok; return *this; }
	Access & set_file_ok(){ m_access |= access_file_ok; return *this; }
	Access & set_execute_ok(){ m_access |= access_execute_ok; return *this; }

	enum o_access_flags o_access() const { return m_access; }

private:
	enum o_access_flags m_access;
};




/*! \brief Stat Class
 * \details The Stat class is basically
 * a wrapper of `struct stat` and provides
 * information on the type and size
 * of the file.
 *
 * The file can be any filesystem object including
 * a file, directory, or device.
 *
 *
 * ```
 * #include <sapi/sys.hpp>
 *
 * Info info = File::get_info("/home/test.txt");
 *
 * if( info.is_file() ){
 *   printf("Is a file\n");
 * } else if( info.is_directory() ){
 *   printf("Is a directory\n");
 * }
 *
 * //grabbing the info of an already open file
 * File file;
 * file.open("/home/test.txt", File::RDONLY);
 * //grab the info from a file that is already open
 * info = file.get_info();
 * file.close();
 * ```
 *
 */
class Stat : public FileFlags {
public:

#if defined __link
	Stat(bool is_local = false);
	Stat(const struct FSAPI_LINK_STAT & st, bool is_local = false) //cppcheck-suppress[noExplicitConstructor]
		: m_stat(st), m_is_local(is_local) {}
#else
	/*! \details Constructs a new object.
	 *
	 * For the newly constructed object,
	 * is_valid() returns false.
	 *
	 */
	Stat();

	/*! \details Constructs a new object
	 * from the struct stat data provided.
	 *
	 */
	Stat(const struct stat & st) //cppcheck-suppress[noExplicitConstructor]
		: m_stat(st){}
#endif

	/*! \details Returns true if the object is valid. */
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
	Permissions permissions() const { return Permissions(m_stat.st_mode); }

	int owner() const { return m_stat.st_uid; }
	int group() const { return m_stat.st_gid; }


	/*! \details Returns the file suffix ('txt' for '/home/test.txt'). */
	static const var::String suffix(const var::String & path);
	/*! \details Returns the file name ('test.txt' for '/home/test.txt'). */
	static const var::String name(const var::String & path);
	/*! \details Returns the path to a file ('/home' for '/home/test.txt'). */
	static const var::String parent_directory(const var::String & path);
	/*! \details Returns the base name of a file path('test' for '/home/test.txt'). */
	static const var::String base_name(const var::String & path);
	/*! \details Returns the path without the suffix ('/home/test' for '/home/test.txt). */
	static const var::String no_suffix(const var::String & path);
	/*! \details Returns true if the first character in any directory or file name
	 * starts with `.`
	 *
	 * Examples:
	 * - `/home/.DS_Store` returns true
	 * - `/home/.library/data.txt` returns true
	 * - `.DS_Store` returns true
	 *
	 *
	 */
	static bool is_hidden(const var::String & path);

private:

#ifdef __link
	struct FSAPI_LINK_STAT m_stat;
	bool m_is_local;
#else
	struct stat m_stat;
#endif

};

typedef Stat FileInfo;

} /* namespace fs */

namespace sys {
class Printer;
sys::Printer& operator << (sys::Printer& printer, const fs::Stat & a);
}

#endif /* SAPI_SYS_FILEINFO_HPP_ */
