/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DIR_HPP_
#define DIR_HPP_

#ifdef __link
#include <iface/link.h>
#else
#include <dirent.h>
#endif

namespace sys {

/*! \brief Dir class */
class Dir {
public:
	/*! \details Construct a Dir object */
#if defined __link
	Dir(link_transport_mdriver_t * driver);
#else
	Dir();
#endif
	/*! \details Open a directory */
	int open(const char * name);
	/*! \details Close the directory */
	int close();

	inline bool is_open() const { return dirp != 0; }

	/*! \details This methods reads the next entry in the directory.
	 * @return A pointer to the name of the entry or 0 if no more entries exist
	 */
	const char * read();


	/*! \details The name of the most recently read entry */
	inline const char * name(){ return entry.d_name; }

	/*! \details The name of the most recently read entry */
	inline char * data(){ return entry.d_name; }

	/*! \details The serial number of the most recently read entry */
	inline int ino(){ return entry.d_ino; }

#ifndef __link
	/*! \details Count the total number of entries in the directory */
	int count();
	/*! \details Rewind the directory pointer */
	inline void rewind(){ if( dirp ) rewinddir(dirp); }
	/*! \details Seek to a location in the directory */
	inline void seek(long loc){ if( dirp ) seekdir(dirp, loc); }
	/*! \details The current pointer location in the directory */
	inline long tell(){ if( dirp ){ return telldir(dirp); } return 0; }
#else
	void set_driver(link_transport_mdriver_t * d){ _driver = d; }
#endif

	int ioctl(int req, void * arg){ return -1; }

private:
#ifdef __link
	int dirp;
	struct link_dirent entry;
	link_transport_mdriver_t * _driver;
	link_transport_mdriver_t * driver(){ return _driver; }
#else
	DIR * dirp;
	struct dirent entry;
#endif

};

};

#endif /* DIR_HPP_ */
