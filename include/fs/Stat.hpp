/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


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
		/*! \cond */
		PUBLIC_EXECUTE = 0001,
		PUBLIC_WRITE = 0002,
		PUBLIC_READ = 0004,
		GROUP_EXECUTE = 0010,
		GROUP_WRITE = 0020,
		GROUP_READ = 0040,
		OWNER_EXECUTE = 0100,
		OWNER_WRITE = 0200,
		OWNER_READ = 0400,
		/*! \endcond */

		public_execute = 0001,
		public_write = 0002,
		public_read = 0004,
		group_execute = 0010,
		group_write = 0020,
		group_read = 0040,
		owner_execute = 0100,
		owner_write = 0200,
		owner_read = 0400,
	};

	enum o_open_flags {
		/*! \cond */
		RDONLY /*! Open as read-only */ = LINK_O_RDONLY,
		READONLY /*! Open as read-only */ = LINK_O_RDONLY,
		WRONLY /*! Open as write-only */ = LINK_O_WRONLY,
		WRITEONLY /*! Open as write-only */ = LINK_O_WRONLY,
		CREATE /*! Create when opening (files) */ = LINK_O_CREAT,
		CREAT /*! Create when opening (files) */ = LINK_O_CREAT,
		TRUNCATE /*! Truncate when opening (files) */ = LINK_O_TRUNC,
		TRUNC /*! Truncate when opening (files) */ = LINK_O_TRUNC,
		APPEND /*! Append when opening (files)*/ = LINK_O_APPEND,
		EXCLUSIVE /*! Create exclusively (files) */ = LINK_O_EXCL,
		EXCL /*! Create exclusively (files) */ = LINK_O_EXCL,
		READWRITE /*! Open as read-write */ = LINK_O_RDWR,
		RDWR /*! Open as read-write */ = LINK_O_RDWR,
		NONBLOCK /*! Open as non-blocking */ = LINK_O_NONBLOCK,
		NDELAY /*! Open as non-blocking */ = LINK_O_NONBLOCK,
		ACCMODE /*! Access mode mask */ = LINK_O_ACCMODE,
		ACCESS_MODE /*! Access mode mask */ = LINK_O_ACCMODE,
		/*! \endcond */

		rdonly /*! Open as read-only */ = LINK_O_RDONLY,
		readonly /*! Open as read-only */ = LINK_O_RDONLY,
		wronly /*! Open as write-only */ = LINK_O_WRONLY,
		writeonly /*! Open as write-only */ = LINK_O_WRONLY,
		create /*! Create when opening (files) */ = LINK_O_CREAT,
		creat /*! Create when opening (files) */ = LINK_O_CREAT,
		truncate /*! Truncate when opening (files) */ = LINK_O_TRUNC,
		trunc /*! Truncate when opening (files) */ = LINK_O_TRUNC,
		append /*! Append when opening (files)*/ = LINK_O_APPEND,
		exclusive /*! Create exclusively (files) */ = LINK_O_EXCL,
		excl /*! Create exclusively (files) */ = LINK_O_EXCL,
		readwrite /*! Open as read-write */ = LINK_O_RDWR,
		rdwr /*! Open as read-write */ = LINK_O_RDWR,
		nonblock /*! Open as non-blocking */ = LINK_O_NONBLOCK,
		ndelay /*! Open as non-blocking */ = LINK_O_NONBLOCK,
		accmode /*! Access mode mask */ = LINK_O_ACCMODE,
		access_mode /*! Access mode mask */ = LINK_O_ACCMODE
	};

	enum o_access_flags {
		/*! \cond */
		READ_OK /*! Check if read access is allowed */ = LINK_R_OK,
		WRITE_OK /*! Check if write access is allowed */ = LINK_W_OK,
		EXECUTE_OK /*! Check if execute access is allowed */ = LINK_X_OK,
		FILE_OK /*! Check if file exists */ = LINK_F_OK,
		/*! \endcond */

		read_ok /*! Check if read access is allowed */ = LINK_R_OK,
		write_ok /*! Check if write access is allowed */ = LINK_W_OK,
		execute_ok /*! Check if execute access is allowed */ = LINK_X_OK,
		file_ok /*! Check if file exists */ = LINK_F_OK,
	};

	enum mode {
		/*! \cond */
		FORMAT /*! Mode format mask */ = LINK_S_IFMT,
		FILE_SOCKET /*! Mode Socket mask */ = LINK_S_IFSOCK,
		REGULAR /*! Mode regular file value */ = LINK_S_IFREG,
		BLOCK /*! Mode block device value */ = LINK_S_IFBLK,
		CHARACTER /*! Mode character device value */ = LINK_S_IFCHR,
		DIRECTORY /*! Mode directory value */ = LINK_S_IFDIR,
		FIFO /*! Mode FIFO value */ = LINK_S_IFDIR,
		SYMBOLIC_LINK /*! Mode symbolic link value */ = LINK_S_IFLNK,
		/*! \endcond */

		format /*! Mode format mask */ = LINK_S_IFMT,
		file_socket /*! Mode Socket mask */ = LINK_S_IFSOCK,
		regular /*! Mode regular file value */ = LINK_S_IFREG,
		block /*! Mode block device value */ = LINK_S_IFBLK,
		character /*! Mode character device value */ = LINK_S_IFCHR,
		directory /*! Mode directory value */ = LINK_S_IFDIR,
		fifo /*! Mode FIFO value */ = LINK_S_IFDIR,
		symbolic_link /*! Mode symbolic link value */ = LINK_S_IFLNK
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

	bool is_owner_read() const { return m_permissions & OWNER_READ; }
	bool is_owner_write() const { return m_permissions & OWNER_WRITE; }
	bool is_owner_execute() const { return m_permissions & OWNER_EXECUTE; }
	bool is_public_read() const { return m_permissions & PUBLIC_READ; }
	bool is_public_write() const { return m_permissions & PUBLIC_WRITE; }
	bool is_public_execute() const { return m_permissions & PUBLIC_EXECUTE; }

	Permissions & set_owner_read(){ m_permissions |= OWNER_READ; return *this; }
	Permissions & set_owner_write(){ m_permissions |= OWNER_WRITE; return *this; }
	Permissions & set_owner_execute(){ m_permissions |= OWNER_EXECUTE; return *this; }

	Permissions & set_public_read(){ m_permissions |= PUBLIC_READ; return *this; }
	Permissions & set_public_write(){ m_permissions |= PUBLIC_WRITE; return *this; }
	Permissions & set_public_execute(){ m_permissions |= PUBLIC_EXECUTE; return *this; }

	bool operator == (const Permissions & a) const { return a.m_permissions == m_permissions; }

	int permissions() const { return m_permissions; }

private:
	int m_permissions;
};

class OpenFlags : public FileFlags {
public:
	explicit OpenFlags(){ m_flags = 0; }

	static OpenFlags create() { return OpenFlags(CREATE | READWRITE); }
	static OpenFlags create_append_write_only() { return OpenFlags(CREATE | WRITEONLY | APPEND); }
	static OpenFlags create_truncate() { return OpenFlags(CREATE | READWRITE | TRUNCATE); }
	static OpenFlags append() { return OpenFlags(READWRITE | APPEND); }
	static OpenFlags append_read_write() { return OpenFlags(READWRITE | APPEND); }
	static OpenFlags read_write() { return OpenFlags(READWRITE); }
	static OpenFlags read_only() { return OpenFlags(READONLY); }
	static OpenFlags write_only() { return OpenFlags(WRITEONLY); }
	static OpenFlags append_write_only() { return OpenFlags(WRITEONLY | APPEND); }

	bool is_read_only() const { return access() == READONLY; }
	bool is_write_only() const { return access() == WRITEONLY; }
	bool is_read_write() const { return access() == READWRITE; }
	int access() const { return m_flags & ACCESS_MODE; }

	bool is_create() const { return m_flags & CREATE; }
	bool is_exclusive() const { return m_flags & EXCLUSIVE; }
	bool is_truncate() const { return m_flags & TRUNCATE; }
	bool is_append() const { return m_flags & APPEND; }
	bool is_non_blocking() const { return m_flags & NONBLOCK; }


	OpenFlags & set_read_only(){ clear_access(); m_flags |= READONLY; return *this; }
	OpenFlags & set_write_only(){ clear_access(); m_flags |= WRITEONLY; return *this; }
	OpenFlags & set_read_write(){ clear_access(); m_flags |= READWRITE; return *this; }
	OpenFlags & set_create(){ m_flags |= CREATE; return *this; }
	OpenFlags & set_truncate(){ m_flags |= TRUNCATE; return *this; }
	OpenFlags & set_append(){ m_flags |= APPEND; return *this; }
	OpenFlags & set_exclusive(){ m_flags |= EXCLUSIVE; return *this; }
	OpenFlags & set_non_blocking(){ m_flags |= NONBLOCK; return *this; }

	int o_flags() const { return m_flags; }

private:
	OpenFlags(int flags){ m_flags = flags; }

	void clear_access(){ m_flags &= ~LINK_O_ACCMODE; }

	int m_flags;
};

class Access : public FileFlags {
public:
	explicit Access(enum o_access_flags access = read_ok | write_ok | file_ok | execute_ok ){ m_access = access; }

	bool is_read_ok() const { return m_access & read_ok; }
	bool is_write_ok() const { return m_access & write_ok; }
	bool is_file_ok() const { return m_access & file_ok; }
	bool is_execute_ok() const { return m_access & execute_ok; }

	Access & set_read_ok(){ m_access |= read_ok; return *this; }
	Access & set_write_ok(){ m_access |= write_ok; return *this; }
	Access & set_file_ok(){ m_access |= file_ok; return *this; }
	Access & set_execute_ok(){ m_access |= execute_ok; return *this; }

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
	Stat(const struct link_stat & st, bool is_local = false){
		m_is_local = is_local;
		m_stat = st;
	}
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
	Stat(const struct stat & st){
		m_stat = st;
	}
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

private:

#ifdef __link
	struct link_stat m_stat;
	bool m_is_local;
#else
	struct stat m_stat;
#endif

};

typedef Stat FileInfo;

} /* namespace sys */

#endif /* SAPI_SYS_FILEINFO_HPP_ */
