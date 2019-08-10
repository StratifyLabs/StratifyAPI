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

class Permissions : api::FsInfoObject {
public:
	explicit Permissions(int mode = 0666){ m_permissions = mode; }

	enum mode_flags {
		PUBLIC_EXECUTE = 0001,
		PUBLIC_WRITE = 0002,
		PUBLIC_READ = 0004,
		GROUP_EXECUTE = 0010,
		GROUP_WRITE = 0020,
		GROUP_READ = 0040,
		OWNER_EXECUTE = 0100,
		OWNER_WRITE = 0200,
		OWNER_READ = 0400,
	};

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

class OpenFlags : api::FsInfoObject {
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

	enum o_open_flags {
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
		ACCESS_MODE /*! Access mode mask */ = LINK_O_ACCMODE
	};

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

class Access : public api::FsInfoObject {
public:
	explicit Access(int access = LINK_R_OK | LINK_W_OK | LINK_F_OK | LINK_X_OK ){ m_access = access; }
	enum o_flags {
		READ_OK /*! Check if read access is allowed */ = LINK_R_OK,
		WRITE_OK /*! Check if write access is allowed */ = LINK_W_OK,
		EXECUTE_OK /*! Check if execute access is allowed */ = LINK_X_OK,
		FILE_OK /*! Check if file exists */ = LINK_F_OK,
	};

	bool is_read_ok() const { return m_access & LINK_R_OK; }
	bool is_write_ok() const { return m_access & LINK_W_OK; }
	bool is_file_ok() const { return m_access & LINK_F_OK; }
	bool is_execute_ok() const { return m_access & LINK_X_OK; }

	Access & set_read_ok(){ m_access |= LINK_R_OK; return *this; }
	Access & set_write_ok(){ m_access |= LINK_W_OK; return *this; }
	Access & set_file_ok(){ m_access |= LINK_F_OK; return *this; }
	Access & set_execute_ok(){ m_access |= LINK_X_OK; return *this; }

	int o_access() const { return m_access; }

private:
	int m_access;
};

typedef api::Argument<const int&> Location;
typedef api::Argument<const int&> PageSize;
typedef api::Argument<const u32&> Size;

typedef api::Argument<const void*> SourceBuffer;
typedef api::Argument<void*> DestinationBuffer;

typedef api::Argument<const var::ConstString&> SourcePath;
typedef api::Argument<const var::ConstString&> DestinationPath;

typedef api::Argument<const var::ConstString&> SourceFilePath;
typedef api::Argument<const var::ConstString&> DestinationFilePath;

typedef api::Argument<const var::ConstString&> SourceDirectoryPath;
typedef api::Argument<const var::ConstString&> DestinationDirectoryPath;

typedef api::Argument<const var::ConstString&> SourceFileName;
typedef api::Argument<const var::ConstString&> DestinationFileName;



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
class Stat : public api::FsInfoObject {
public:

	enum mode {
		FORMAT /*! Mode format mask */ = LINK_S_IFMT,
		FILE_SOCKET /*! Mode Socket mask */ = LINK_S_IFSOCK,
		REGULAR /*! Mode regular file value */ = LINK_S_IFREG,
		BLOCK /*! Mode block device value */ = LINK_S_IFBLK,
		CHARACTER /*! Mode character device value */ = LINK_S_IFCHR,
		DIRECTORY /*! Mode directory value */ = LINK_S_IFDIR,
		FIFO /*! Mode FIFO value */ = LINK_S_IFDIR,
		SYMBOLIC_LINK /*! Mode symbolic link value */ = LINK_S_IFLNK
	};

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
