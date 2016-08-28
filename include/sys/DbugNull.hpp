/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DBUGNULL_HPP_
#define DBUGNULL_HPP_

#include "Dbug.hpp"

/*! \brief Debug Null Class
 * \details This class is useful when you want to disable debugging.
 *
 */
class DbugNull : public Dbug {
public:
	DbugNull(){}

	int open(int mode = 0){ return 0; }
	int close(void){ return 0; }

	int read(void * buf, int nbyte){ return nbyte; }
	int write(const void * buf, int nbyte){ return nbyte; }
};

#endif /* DBUGNULL_HPP_ */
