/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DBUGSTDOUT_HPP_
#define DBUGSTDOUT_HPP_

#include "Dbug.hpp"

namespace sys {

/*! \brief Debug Stdout Class
 * \details This class uses Dbug and a standard output to
 * create a concrete debugging class.  The following is an example
 * of how to use the debugger:
 * \code
 * DbugStdout d; //use the standard output for debugging
 * d.setmode(Dbug::DECIMAL, 8); //decimal mode with 8 leading zeros
 * d << "My number in DECIMAL " << 1123 << "\n";
 * d.setmode(Dbug::BINARY, 8); //decimal mode with 8 leading zeros
 * d << "My number in BINARY " << 1123 << "\n";
 * \endcode
 *
 */
class DbugStdout : public Dbug {
public:
	DbugStdout();

	int open(void);
	int close(void);

	int read(void * buf, int nbyte);
	int write(const void * buf, int nbyte);
};

};

#endif /* DBUGSTDOUT_HPP_ */
