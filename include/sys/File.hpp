/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef FILE_HPP_
#define FILE_HPP_

#include <iface/link.h>
#include <fcntl.h>

#ifndef __link
#include <unistd.h>
#include "../sys/File.hpp"
#include "../sys/Aio.hpp"
#include "../var/String.hpp"
#else
#undef fileno
#endif


namespace sys {

/*! \brief File Class
 * \details This class is used to file access.  It uses the POSIX functions open(), read(), write(), close(), etc.  You
 * can always call these functions directly or use the standard C library to access files (fopen(), fread(), fwrite()--
 * these use more memory than this class or the POSIX functions).
 *
 * Here is an example of using this class:
 *
 * \code
 * #include <stfy/sys.hpp>
 * #include <stfy/var.hpp>
 *
 *
 * int main(int argc, char * argv[]){
 * 	File f;
 * 	String str;
 *
 *  //create a new file and write a string to it
 * 	f.create("/home/myfile.txt");
 * 	str = "Hello New File!\n";
 * 	f.write(str.c_str(), str.size());
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
	File();

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

	/*! \details Get the name of the file from a given path */
	static const char * name(const char * path);

	/*! \details Deletes a file
	 *
	 * @param path The path to the file
	 *
	 */
	static int remove(const char * path, link_transport_mdriver_t * driver = 0);

	/*! \details Gets file stat data
	 *
	 * @param path The path to the file
	 * @param st A pointer to the stat structure
	 *
	 */
	static int stat(const char * path, struct link_stat * st, link_transport_mdriver_t * driver = 0);

	/*! \details Gets the size of the file.
	 *
	 * @param path The path to the file
	 */
	static ssize_t size(const char * path, link_transport_mdriver_t * driver = 0);


	/*! \details Open the specified file or device */
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
	ssize_t size() const;

	/*! \details Returns the location of the cursor in the device or file */
	int loc() const;

	/*! \details Return the current flags for the file */
	int flags() const;

	/*! \details Return the file number for accessing the file or device */
	int fileno() const;

	/*! \details Close the file or device */
	virtual int close();

	/*! \details reads the device. */
	virtual int read(void * buf, int nbyte) const;

	/*! \details writes data to the device */
	virtual int write(const void * buf, int nbyte) const;

	/*! \details reads the device at the location */
	int read(int loc, void * buf, int nbyte) const;

	/*! \details writes the device at the location
	 *
	 * @param loc Location to read (not application to character devices)
	 * @param buf Pointer to the source data
	 * @param nbyte Number of bytes to write
	 * @return Number of bytes successfully written or -1 with errno set
	 */
	int write(int loc, const void * buf, int nbyte) const;

	/*! \details writes a string to the device */
	int write(const char * s) const { return write(s, strlen(s)); }

	/*! \details Read a line from the device or file */
	int readline(char * buf, int nbyte, int timeout, char term) const;


#ifndef __link
	/*! \details Write a Var::String to the file
	 *
	 * @param str The string to write
	 * @return The number of bytes written
	 */
	int write(const var::String & str) const { return write(str.c_str(), str.size()); }
#endif

	enum {
		GETS_BUFFER_SIZE = 128
	};


	/*! \details Seek to a location in the file or on the device */
	virtual int seek(int loc, int whence = LINK_SEEK_SET) const;


	/*! \details Read up to n-1 bytes to \a s until end-of-file or \a term is reached.  */
	char * gets(char * s, int n, char term = '\n') const;

#ifndef __link
	/*! \details Read a line in to the string */
	char * gets(var::String & s, char term = '\n') const { return gets(s.cdata(), s.capacity(), term); }
#endif


#ifdef __link
	void set_driver(link_transport_mdriver_t * d){ m_driver = d; }
	link_transport_mdriver_t * driver() const { return m_driver; }
#endif

	/*! \details Executes an IO control request
	 *
	 * @param req The request value
	 * @param arg A pointer to the arguments
	 *
	 */
	virtual int ioctl(int req, void * arg) const;

	/*! \details ioctl() with request and const arg pointer */
	int ioctl(int req, const void * arg) const { return ioctl(req, (void*)arg); }
	/*! \details ioctl() with just a request */
	int ioctl(int req) const { return ioctl(req, (void*)NULL); }
	/*! \details ioctl() with request and integer arg */
	int ioctl(int req, int arg) const { return ioctl(req, (void*)(size_t)arg); }


protected:
	mutable int m_fd;
#ifdef __link
	link_transport_mdriver_t * m_driver;
#endif

};

};

#endif /* FILE_HPP_ */
