/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef FILE_HPP_
#define FILE_HPP_

#include "../Var/String.hpp"
#include "../Hal/Phy.hpp"

#ifdef fileno
#undef fileno
#endif

namespace Sys {

/*! \brief File Class
 * \details This class is used to file access.  It uses the POSIX functions open(), read(), write(), close(), etc.  You
 * can always call these functions directly or use the standard C library to access files (fopen(), fread(), fwrite()--
 * these use more memory than this class or the POSIX functions).
 *
 * Here is an example of using this class:
 *
 * \code
 * #include <stfy/Sys.hpp>
 * #include <stfy/Var.hpp>
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
class File : public Hal::Phy {
public:
	File();

	/*! \details Delete a file */
	static int remove(const char * name, link_transport_phy_t handle = LINK_PHY_OPEN_ERROR);

	/*! \details Get file stat data
	 *
	 * @param name The path to the file
	 * @param st A pointer to the stat structure
	 *
	 */
	static int stat(const char * name, struct link_stat * st, link_transport_phy_t handle = LINK_PHY_OPEN_ERROR);

	/*! \details Get the size of the file */
	static ssize_t size(const char * name, link_transport_phy_t handle = LINK_PHY_OPEN_ERROR);

	/*! \details Get the name of the file from a given path */
	static const char * name(const char * path);


	/*! \details This opens a file.  If the object already has a file open, the open
	 * file will be closed first.
	 *
	 * @param name The filename to open
	 * @param access The access ie RDWR
	 * @param perms Permission settings
	 * @return Zero on success
	 */
	int open(const char * name, int access = RDWR, int perms = 0666);

	/*! \details Open file for read/write */
	inline int open_readwrite(const char * name){
		return open(name);
	}

	/*! \details Open a read only file */
	inline int open_readonly(const char * name){ return open(name, READONLY); }

	/*! \details Open a file for appending */
	inline int open_append(const char * name){ return open(name, APPEND | CREATE | WRITEONLY); }

	/*! \details Create a new file (using the open() method) */
	int create(const char * name, bool overwrite = true, int perms = 0666);

	/*! \details Read the file at the current location */
	int read(void * buf, int nbyte) const;

	/*! \details Read the file from the specified location */
	int read(int loc, void * buf, int nbyte) const;

	/*! \details Write the file */
	int write(const void * buf, int nbyte) const;

	/*! \details Write the file at the specified location */
	int write(int loc, const void * buf, int nbyte) const;

	/*! \details Write a Var::String to the file
	 *
	 * @param str The string to write
	 * @return The number of bytes written
	 */
	inline int write(const Var::String & str) const {
		return write(str.c_str(), str.size());
	}

	/*! \details Write a string to the file
	 *
	 * @param str A pointer to the string
	 * @return The number of bytes written
	 */
	inline int write(const char * str) const {
		return write(str, strlen(str));
	}

	/*! \details Seek to the specified location in the file
	 *
	 * @param loc Location parameter
	 * @param whence How to interpret location (SEEK_SET, SEEK_CUR, or SEEK_END)
	 */
	int seek(int loc, int whence = SEEK_SET) const;

	/*! \details Return the file size */
	ssize_t size(void) const;

	/*! \details Close the file */
	int close(void);

	/*! \details Return the file descriptor */
	int fileno() const;

	/*! \details Return the current offset location in the file */
	int loc() const;

	/*! \details Return the current flags for the file */
	int flags() const;

	/*! \details Read up to n-1 bytes to \a s until end-of-file or \a term is reached.  */
	char * gets(char * s, int n, char term = '\n') const;

	/*! \details Read a line in to the string */
	char * gets(Var::String * s, char term = '\n') const { return gets(s->cdata(), s->capacity(), term); }

	/*! \details Read a line in to the string */
	char * gets(Var::String & s, char term = '\n') const { return gets(s.cdata(), s.capacity(), term); }

#ifndef __MCU_ONLY__
	/*! \details Return the file descriptor for the peripheral */
	using Phy::ioctl;
	int ioctl(int req, void * arg);
#endif


private:
	int fd;

	enum {
		GETS_BUFFER_SIZE = 128
	};

};

};

#endif /* FILE_HPP_ */
