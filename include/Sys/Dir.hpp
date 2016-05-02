/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DIR_HPP_
#define DIR_HPP_

#ifdef __link
#include <iface/link.h>
#else
#include <dirent.h>
#endif

namespace Sys {

/*! \brief Dir class */
class Dir {
public:
	/*! \details Construct a Dir object */
	Dir();
	/*! \details Open a directory */
	int open(const char * name);
	/*! \details Close the directory */
	int close(void);

	inline bool is_open() const { return dirp != 0; }

	/*! \details This methods reads the next entry in the directory.
	 * @return A pointer to the name of the entry or 0 if no more entries exist
	 */
	const char * read(void);

	int size();

	/*! \details The name of the most recently read entry */
	inline const char * name(void){ return entry.d_name; }

	/*! \details The name of the most recently read entry */
	inline char * data(void){ return entry.d_name; }

	/*! \details The serial number of the most recently read entry */
	inline int ino(void){ return entry.d_ino; }

#ifndef __link
	/*! \details Count the total number of entries in the directory */
	int count(void);
	/*! \details Rewind the directory pointer */
	inline void rewind(){ if( dirp ) rewinddir(dirp); }
	/*! \details Seek to a location in the directory */
	inline void seek(long loc){ if( dirp ) seekdir(dirp, loc); }
	/*! \details The current pointer location in the directory */
	inline long tell(void){ if( dirp ){ return telldir(dirp); } return 0; }
#endif

	int ioctl(int req, void * arg){ return -1; }

private:
#ifdef __link
	int dirp;
	struct link_dirent entry;
#else
	DIR * dirp;
	struct dirent entry;
#endif

};

};

#endif /* DIR_HPP_ */
