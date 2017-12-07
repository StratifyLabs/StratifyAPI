/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved

#ifndef FILE_HPP_
#define FILE_HPP_

#include <sos/link.h>
#include <fcntl.h>

#ifndef __link
#include <unistd.h>
#include "../sys/File.hpp"
#include "../sys/Aio.hpp"
#define MCU_INT_CAST(var) ((void*)(u32)var)
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
class File {
public:

#if defined __link
	File(link_transport_mdriver_t * d);
	File();
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
		ACCMODE /*! Access mode mask */ = LINK_O_ACCMODE
	};

	/*! \details List of options for \a whence argument of seek() */
	enum {
		SET /*! Set the location of the file descriptor */ = SEEK_SET,
		CURRENT /*! Set the location relative to the current location */ = SEEK_CUR,
		END /*! Set the location relative to the end of the file or device */ = SEEK_END
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
	static const char * name(const char * path);

	/*! \details Returns a pointer to the file suffix.
	 *
	 * @param path The path to search
	 * @return A pointer to the suffix
	 *
	 * For example:
	 *
	 * \code
	 * const char * path = "/home/data.txt";
	 * printf("Suffix is %s", File::suffix(path));
	 * \endcode
	 *
	 * The above code will output:
	 * \code
	 * Suffix is txt
	 * \endcode
	 *
	 */
	static const char * suffix(const char * path);

	/*! \details Deletes a file.
	 *
	 * @param path The path to the file
	 * @param driver Used only with link protocol
	 * @return Zero on success
	 *
	 */
	static int remove(const char * path, link_transport_mdriver_t * driver = 0);

	/*! \details Gets file stat data.
	 *
	 * @param path The path to the file
	 * @param st A pointer to the stat structure
	 * @param driver Used only with link protocol
	 * @return Zero on success
	 *
	 */
#if !defined __link
	static int stat(const char * path, struct stat * st);
#else
	static int stat(const char * path, struct link_stat * st, link_transport_mdriver_t * driver = 0);
#endif

	/*! \details Gets the size of the file.
	 *
	 * @param path The path to the file
	 * @param driver Used only with link protocol
	 * @return The number of bytes in the file or less than zero for an error
	 *
	 */
#if !defined __link
	static u32 size(const char * path);
#else
	static u32 size(const char * path, link_transport_mdriver_t * driver = 0);
#endif


	/*! \details Opens a file.
	 *
	 * @param name The path to the file
	 * @param flags The flags used to open the flag (e.g. File::READONLY)
	 * @return Zero on success
	 */
	virtual int open(const char * name, int flags);

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
	int open(const char * name, int access, int perms);

	/*! \details Opens a file for read/write.
	 *
	 * @param path The path to the file
	 *
	 */
	inline int open_readwrite(const char * path){
		return open(path, RDWR);
	}

	/*! \details Opens a read only file.
	 *
	 * @param path The path to the file
	 *
	 */
	inline int open_readonly(const char * path){
		return open(path, READONLY);
	}

	/*! \details Opens a file for appending.
	 *
	 * @param path The path to the file
	 *
	 */
	inline int open_append(const char * path){
		return open(path, APPEND | CREATE | WRITEONLY);
	}

	/*! \details Creates a new file (using the open() method).
	 *
	 * @param path The path to the file
	 * @param overwrite Overwrite (truncate) the existing file (defaults to true)
	 * @param perms The permissions to assign to the newly created file
	 */
	int create(const char * path, bool overwrite = true, int perms = 0666);

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

	/*! \details Write the file.
	 *
	 * @param buf A pointer to the source buffer
	 * @param nbyte The number of bytes to read
	 * @return The number of bytes read or less than zero on an error
	 */
	virtual int write(const void * buf, int nbyte) const;

	/*! \details Reads the file.
	 *
	 * @param loc The location of the file to read
	 * @param buf A pointer to the destination buffer
	 * @param nbyte The number of bytes to read
	 * @return The number of bytes read or less than zero on an error
	 */
	int read(int loc, void * buf, int nbyte) const;

	/*! \details writes the device at the location
	 *
	 * @param loc Location to write (not application to character devices)
	 * @param buf Pointer to the source data
	 * @param nbyte Number of bytes to write
	 * @return Number of bytes successfully written or -1 with errno set
	 */
	int write(int loc, const void * buf, int nbyte) const;

	/*! \details Reads a line from a file.
	 *
	 * @param buf Destination buffer
	 * @param nbyte Number of bytes available in buffer
	 * @param timeout Timeout in ms if line does not arrive
	 * @param term Terminating character of the line (default is newline)
	 * @return Number of bytes received
	 */
	int readline(char * buf, int nbyte, int timeout, char term = '\n') const;


	/*! \details Writes a var::String to the file.
	 *
	 * @param str The string to write
	 * @return The number of bytes written
	 */
	int write(const var::String & str) const {
		return write(str.c_str(), str.size());
	}

	File& operator<<(const char * a){ write(a, strlen(a)); return *this; }


	enum {
		GETS_BUFFER_SIZE = 128
	};


	/*! \details Seeks to a location in the file or on the device. */
	virtual int seek(int loc, int whence = LINK_SEEK_SET) const;


	/*! \details Reads up to n-1 bytes to \a s until end-of-file or \a term is reached.  */
	char * gets(char * s, int n, char term = '\n') const;

#ifndef __link
	/*! \details Reads a line in to the var::String until end-of-file or \a term is reached. */
	char * gets(var::String & s, char term = '\n') const { return gets(s.cdata(), s.capacity(), term); }
#endif


#ifdef __link
	void set_driver(link_transport_mdriver_t * d){ m_driver = d; }
	link_transport_mdriver_t * driver() const { return m_driver; }
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

	void set_fileno(int fd){ m_fd = fd; }


protected:
	mutable int m_fd;
#ifdef __link
	link_transport_mdriver_t * m_driver;
#endif

};

};

#endif /* FILE_HPP_ */
