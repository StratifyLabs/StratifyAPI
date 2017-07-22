/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DIR_HPP_
#define DIR_HPP_

#ifdef __link
#include <sos/link.h>
#else
#include <dirent.h>
#endif

namespace sys {

/*! \brief Dir class */
class Dir {
public:
	/*! \details Constructs a Dir object */
#if defined __link
	Dir(link_transport_mdriver_t * driver);
#else
	Dir();
#endif
	/*! \details Opens a directory. */
	int open(const char * name);
	/*! \details Closes the directory. */
	int close();

	/*! \details Returns a true if the directory is open. */
	bool is_open() const { return m_dirp != 0; }

	/*! \details Returns a pointer to the next entry or 0 if no more entries exist.
	 */
	const char * read();


	/*! \details The name of the most recently read entry */
	inline const char * name(){ return m_entry.d_name; }

	/*! \details The name of the most recently read entry */
	inline char * data(){ return m_entry.d_name; }

	/*! \details The serial number of the most recently read entry */
	inline int ino(){ return m_entry.d_ino; }

#ifndef __link
	/*! \details Count the total number of entries in the directory */
	int count();
	/*! \details Rewind the directory pointer */
	inline void rewind(){ if( m_dirp ) rewinddir(m_dirp); }
	/*! \details Seek to a location in the directory */
	inline void seek(long loc){ if( m_dirp ) seekdir(m_dirp, loc); }
	/*! \details The current pointer location in the directory */
	inline long tell(){ if( m_dirp ){ return telldir(m_dirp); } return 0; }
#else
	void set_driver(link_transport_mdriver_t * d){ m_driver = d; }
#endif

private:
#ifdef __link
	int m_dirp;
	struct link_dirent m_entry;
	link_transport_mdriver_t * m_driver;
	link_transport_mdriver_t * driver(){ return m_driver; }
#else
	DIR * m_dirp;
	struct dirent m_entry;
#endif


};

};

#endif /* DIR_HPP_ */
