/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef FILE_HPP_
#define FILE_HPP_

#include <Hal/Dev.hpp>
#include "../Var/String.hpp"

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
class File : public Hal::Dev {
public:
	File();

	/*! \details Delete a file */
	static int remove(const char * name, link_transport_mdriver_t * driver = 0);

	/*! \details Get file stat data
	 *
	 * @param name The path to the file
	 * @param st A pointer to the stat structure
	 *
	 */
	static int stat(const char * name, struct link_stat * st, link_transport_mdriver_t * driver = 0);

	/*! \details Get the size of the file */
	static ssize_t size(const char * name, link_transport_mdriver_t * driver = 0);


	/*! \details This opens a file.  If the object already has a file open, the open
	 * file will be closed first.
	 *
	 * @param name The filename to open
	 * @param access The access ie RDWR
	 * @param perms Permission settings
	 * @return Zero on success
	 */
	int open(const char * name, int access, int perms);
	using Dev::open;

	/*! \details Open file for read/write */
	inline int open_readwrite(const char * name){
		return open(name, RDWR);
	}

	/*! \details Open a read only file */
	inline int open_readonly(const char * name){ return open(name, READONLY); }

	/*! \details Open a file for appending */
	inline int open_append(const char * name){ return open(name, APPEND | CREATE | WRITEONLY); }

	/*! \details Create a new file (using the open() method) */
	int create(const char * name, bool overwrite = true, int perms = 0666);

	/*! \details Return the file size */
	ssize_t size() const;

private:

};

};

#endif /* FILE_HPP_ */
