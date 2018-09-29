/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved

#ifndef SYS_FILE_HPP_
#define SYS_FILE_HPP_

#include <sos/link.h>

#include "../api/SysObject.hpp"
#include "../var/ConstString.hpp"

#if !defined __link
#include <unistd.h>
#include "../sys/Aio.hpp"
#define MCU_INT_CAST(var) ((void*)(u32)var)
#include <fcntl.h>
#else
#undef fileno
#define MCU_INT_CAST(var) ((void*)(u64)var)
#endif

#include "../var/String.hpp"

namespace sys {

/*! \brief File Class
 * \details This class is used to access files (and devices).  It uses the POSIX functions open(), read(), write(), close(), etc.  You
 * can always call these functions directly or use the standard C library to access files (fopen(), fread(), fwrite()--
 * these use more memory than this class or the POSIX functions).
 *
 * Here is an example of using this class:
 *
 * \code
 * #include <sapi/sys.hpp>
 * #include <sapi/var.hpp>
 *
 *
 * int main(int argc, char * argv[]){
 * 	File f;
 * 	String str;
 *
 *  //create a new file and write a string to it
 * 	f.create("/home/myfile.txt");
 * 	str = "Hello New File!\n";
 * 	f.write(str);
 * 	f.close();
 *
 *  //Now open the file we just closed
 * 	f.open("/home/myfile.txt");
 * 	str = "";
 * 	f.read(str.data(), str.capacity());
 * 	f.close();
 *
 *  //This is what was read from the file
 * 	printf("The String is %s\n", str.c_str());
 *
 * 	File::remove("/home/myfile.txt"); //delete the file
 * 	return 0;
 * }
 *
 * \endcode
 *
 */
class File : public api::SysWorkObject {
public:

#if defined __link
	File(link_transport_mdriver_t * driver = 0);
#else
	File();
#endif

	/*! \details These values are used as flags when opening devices or files */
	enum {
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
		FORMAT /*! Mode format mask */ = LINK_S_IFMT,
		FILE_SOCKET /*! Mode Socket mask */ = LINK_S_IFSOCK,
		REGULAR /*! Mode regular file value */ = LINK_S_IFREG,
		BLOCK /*! Mode block device value */ = LINK_S_IFBLK,
		CHARACTER /*! Mode character device value */ = LINK_S_IFCHR,
		DIRECTORY /*! Mode directory value */ = LINK_S_IFDIR,
		FIFO /*! Mode FIFO value */ = LINK_S_IFDIR,
		SYMBOLIC_LINK /*! Mode symbolic link value */ = LINK_S_IFLNK,
		READ_OK /*! Check if read access is allowed */ = LINK_R_OK,
		WRITE_OK /*! Check if write access is allowed */ = LINK_W_OK,
		EXECUTE_OK /*! Check if execute access is allowed */ = LINK_X_OK,
		FILE_OK /*! Check if file exists */ = LINK_F_OK,

	};

	/*! \details List of options for \a whence argument of seek() */
	enum {
		SET /*! Set the location of the file descriptor */ = LINK_SEEK_SET,
		CURRENT /*! Set the location relative to the current location */ = LINK_SEEK_CUR,
		END /*! Set the location relative to the end of the file or device */ = LINK_SEEK_END
	};

	/*! \details Gets the name of the file from a given path.
	 *
	 * \code
	 * const char * path = "/app/flash/HelloWorld";
	 * printf("Name is %s", File::name(path));
	 * \endcode
	 *
	 * The above code will output:
	 * \code
	 * Name is HelloWorld
	 * \endcode
	 *
	 */
	static var::ConstString name(const var::ConstString & path);


#if !defined __link
	/*! \details Returns an error if the access is not allowed.
	  *
	  * @param path path to the file
	  * @param o_access Bit-wise OR of access flags READ_OK | WRITE_OK | EXECUTE_OK | FILE_OK
	  *
	  */
	static int access(const var::ConstString & path, int o_access);

	/*! \details Returns true if the file exists.
	  *
	  */
	static bool exists(const var::ConstString & path){ return access(path, FILE_OK) >= 0; }
#endif

	/*! \details Returns a pointer to the file suffix.
	 *
	 * @param path The path to search
	 * @return A pointer to the suffix
	 *
	 * For example:
	 *
	 * \code
	  * const var::ConstString path = "/home/data.txt";
	  * printf("Suffix is %s", File::suffix(path).str());
	 * \endcode
	 *
	 * The above code will output:
	 * \code
	 * Suffix is txt
	 * \endcode
	 *
	 */
	static var::ConstString suffix(const var::ConstString & path);

	/*! \details Deletes a file.
	 *
	 * @param path The path to the file
	 * @return Zero on success
	 *
	 */
#if !defined __link
	static int remove(const var::ConstString & path);
#else
	static int remove(const var::ConstString & path, link_transport_mdriver_t * driver = 0);
#endif

	/*! \details Gets file stat data.
	 *
	 * @param path The path to the file
	 * @param st A pointer to the stat structure
	 * @return Zero on success
	 *
	 */
#if !defined __link
	static int stat(const var::ConstString & path, struct stat * st);
#else
	static int stat(const var::ConstString & path, struct link_stat * st, link_transport_mdriver_t * driver = 0);
#endif

	/*! \details Gets the size of the file.
	 *
	 * @param path The path to the file
	 * @return The number of bytes in the file or less than zero for an error
	 *
	 */
#if !defined __link
	static u32 size(const var::ConstString & path);
#else
	static u32 size(const var::ConstString & path, link_transport_mdriver_t * driver);
#endif


	/*! \details Opens a file.
	 *
	 * @param name The path to the file
	 * @param flags The flags used to open the flag (e.g. File::READONLY)
	 * @return Zero on success
	 */
	virtual int open(const var::ConstString & name, int flags = RDWR);

	/*! \details Opens a file.
	 *
	 * @param name The filename to open
	 * @param access The access ie RDWR
	 * @param perms Permission settings
	 * @return Zero on success
	 *
	 * If the object already has a file open, the
	 * file will be closed.
	 *
	 */
	int open(const var::ConstString & name, int access, int perms);

	/*! \details Opens a file for read/write.
	 *
	 * @param path The path to the file
	 *
	 */
	inline int open_readwrite(const var::ConstString & path){
		return open(path, RDWR);
	}

	/*! \details Opens a read only file.
	 *
	 * @param path The path to the file
	 *
	 */
	inline int open_readonly(const var::ConstString & path){
		return open(path, READONLY);
	}

	/*! \details Opens a file for appending.
	 *
	 * @param path The path to the file
	 *
	 */
	inline int open_append(const var::ConstString & path){
		return open(path, APPEND | CREATE | WRITEONLY);
	}

	/*! \details Creates a new file (using the open() method).
	 *
	 * @param path The path to the file
	 * @param overwrite Overwrite (truncate) the existing file (defaults to true)
	 * @param perms The permissions to assign to the newly created file
	 */
	int create(const var::ConstString & path, bool overwrite = true, int perms = 0666);

	/*! \details Returns the file size. */
	u32 size() const;

	/*! \details Returns the location of the cursor in the device or file. */
	int loc() const;

	/*! \details Return the current flags for the file. */
	int flags() const;

	/*! \details Return the file number for accessing the file or device. */
	int fileno() const;

	/*! \details Closes the file or device. */
	virtual int close();

	/*! \details Reads the file.
	 *
	 * @param buf A pointer to the destination buffer
	 * @param nbyte The number of bytes to read
	 * @return The number of bytes read or less than zero on an error
	 */
	virtual int read(void * buf, int nbyte) const;

	/*! \details Reads the file into a var::Data object.
	  *
	  * @param data The destination data object
	  * @return The number of bytes read
	  *
	  * This method will read up to data.size() bytes.
	  *
	  */
	int read(var::Data & data) const {
		int result = read(data.to_void(), data.size());
		if( result > 0 ){ data.set_size(result); }
		return result;
	}

	/*! \details Write the file.
	 *
	 * @param buf A pointer to the source buffer
	 * @param nbyte The number of bytes to read
	  * @return The number of bytes written or less than zero on an error
	 */
	virtual int write(const void * buf, int nbyte) const;

	/*! \details Writes the file using a var::Data object. */
	int write(const var::Data & data) const { return write(data.to_void(), data.size()); }

	/*! \details Writes a var::ConstString to the file.
	  *
	  * @param str The string to write
	  * @return The number of bytes written
	  */
	int write(const var::ConstString & str) const { return write(str.str(), str.length()); }

	/*! \details Writes a var::String to the file.
	  *
	  * @param str The string to write
	  * @return The number of bytes written
	  */
	int write(const var::String & str) const { return write(str.str(), str.length()); }

	/*! \details Reads the file.
	 *
	 * @param loc The location of the file to read
	 * @param buf A pointer to the destination buffer
	 * @param nbyte The number of bytes to read
	 * @return The number of bytes read or less than zero on an error
	 */
	int read(int loc, void * buf, int nbyte) const;

	/*! \details Reads the file using a var::Data object. */
	int read(int loc, var::Data & data) const {
		int result = read(loc, data.to_void(), data.size());
		if( result > 0 ){ data.set_size(result); }
		return result;
	}

	/*! \details Writes the file at the location specified.
	 *
	 * @param loc Location to write (not application to character devices)
	 * @param buf Pointer to the source data
	 * @param nbyte Number of bytes to write
	  * @return Number of bytes successfully written or less than zero with errno set
	 */
	int write(int loc, const void * buf, int nbyte) const;

	/*! \details Writes the file using a var::Data object at the location specified. */
	int write(int loc, const var::Data & data) const { return write(loc, data.to_void(), data.size()); }

	/*! \details Writes the file using a var::ConstString object at the location specified. */
	int write(int loc, const var::ConstString & str) const { return write(loc, str.str(), str.length()); }

	/*! \details Writes the file using a var::String object at the location specified. */
	int write(int loc, const var::String & str) const { return write(loc, str.str(), str.length()); }

	/*! \details Reads a line from a file.
	 *
	 * @param buf Destination buffer
	 * @param nbyte Number of bytes available in buffer
	 * @param timeout_msec Timeout in ms if line does not arrive
	 * @param terminator Terminating character of the line (default is newline)
	 * @return Number of bytes received
	 */
	int readline(char * buf, int nbyte, int timeout_msec, char terminator = '\n') const;

	File& operator<<(const var::ConstString & a){ write(a.str(), a.length()); return *this; }

	/*! \details Seeks to a location in the file or on the device. */
	virtual int seek(int loc, int whence = LINK_SEEK_SET) const;

	/*! \details Reads a line in to the var::String until end-of-file or \a term is reached. */
	var::String gets(char term = '\n') const;

	char * gets(var::String & s, char term = '\n') const;


#ifndef __link
	[[deprecated("Use gets(var::String & s) instead")]]
#endif
	char * gets(char * s, int n, char term = '\n') const;

#ifdef __link
	static void set_default_driver(link_transport_mdriver_t * driver){
		m_default_driver = driver;
	}
	void set_driver(link_transport_mdriver_t * driver){ m_driver = driver; }
	link_transport_mdriver_t * driver() const { return m_driver; }
	static link_transport_mdriver_t * default_driver(){ return m_default_driver; }

#endif

	/*! \details Executes an IO control request.
	 *
	 * @param req The request value
	 * @param arg A pointer to the arguments
	 *
	 * This method is typically only implemented for devices
	 * and other special file systems.
	 *
	 */
	virtual int ioctl(int req, void * arg) const;

	/*! \details Executes an ioctl() with request and const arg pointer.
	 *
	 * @param req The request value
	 * @param arg A pointer to the arguments
	 * @return Depends on request
	 *
	 */
	int ioctl(int req, const void * arg) const { return ioctl(req, (void*)arg); }
	/*! \details Executes an ioctl() with just a request.
	 *
	 * @param req The request value
	 * @return Depends on request
	 *
	 * The arg value for the ioctl is set to NULL.
	 *
	 */
	int ioctl(int req) const { return ioctl(req, (void*)NULL); }
	/*! \details Executes an ioctl() with request and integer arg.
	 *
	 * @param req The request value
	 * @param arg An integer value (used with some requests)
	 * @return Depends on request
	 */
	int ioctl(int req, int arg) const { return ioctl(req, MCU_INT_CAST(arg)); }

	/*! \details Sets the file descriptor for this object. */
	void set_fileno(int fd){ m_fd = fd; }

	/*! \details Sets the file descriptor of this object to the file descriptor of file.
	  *
	  * @param file A reference to the file to share the fileno with.
	  *
	  * The reference \a file must already be opened and have a valid file
	  * number. This won't bind the file number to file just assign
	  * it based on the state of file.fileno() when this method is called.
	  *
	  *
	  */
	void set_fileno(const File & file){ m_fd = file.fileno(); }

	/*! \details Copies a file from the source to the destination.
	  *
	  * \param source_path Path to the source file
	  * \param dest_path Path to the destination file (new location)
	  * \return Zero on success
	  *
	  *
	  */
#if !defined __link
	static int copy(var::String & source_path, var::String & dest_path);
#else
	static int copy(const var::String & source_path, const var::String & dest_path, link_transport_mdriver_t * driver = 0);
#endif

	/*! \details Renames a file.
	  *
	  * \param old_path Current path to the file (will be old path after rename)
	  * \param new_path New path to the file
	  * \return Zero on success
	  *
	  */
#if !defined __link
	static int rename(const var::ConstString & old_path, const var::ConstString & new_path);
#else
	static int rename(const var::ConstString & old_path, const var::ConstString & new_path, link_transport_mdriver_t * driver = 0);
#endif


protected:

	static int copy(File & source, File & dest, const var::ConstString & source_path, const var::ConstString & dest_path);

	enum {
		GETS_BUFFER_SIZE = 128
	};

	mutable int m_fd;
#ifdef __link
	mutable link_transport_mdriver_t * m_driver;
	static link_transport_mdriver_t * m_default_driver;
	int check_driver() const {
		if( m_driver == 0 ){
			m_driver = m_default_driver;
			if( m_driver == 0 ){
				set_error_number(2);
				return -1;
			}
		}
		return 0;
	}

	static link_transport_mdriver_t * check_driver(link_transport_mdriver_t * driver){
		if( driver == 0 ){
			return m_default_driver;
		}
		return driver;
	}

#endif

};

}

#endif /* SYS_FILE_HPP_ */
