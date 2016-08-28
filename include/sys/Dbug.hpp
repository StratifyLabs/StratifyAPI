
/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DBUG_HPP_
#define DBUG_HPP_

#include <stdint.h>

namespace sys {

/*! \brief Debug Class
 * \details This is an abstract peripheral character class
 * that can be used for debugging.  For an example see: \ref DbugUart
 */
class Dbug {
public:
	Dbug();

	/*! \brief Output Modes */
	enum mode{
		BINARY /*! Binary mode */,
		OCTAL /*! Octal mode */,
		hexadecimal /*! Hexadecimal mode (lowercase) */,
		HEXADECIMAL /*! Hexadecimal mode (uppercase) */,
		DECIMAL /*! Decimal mode (Default); used exclusively by signed types */
	};

	enum {
		BUF_SIZE = 36
	};

	/*! \brief See detail below
	 * \details This sets the formatting modes when outputting integers.
	 *
	 */
	void setmode(Dbug::mode mode, int width = 0);

	/*! \brief See details below
	 * \details This outputs a simple string on the debug terminal.
	 *
	 */
	int puts(const char * s);

	/*! \brief Output a string */
	Dbug& operator<<(const char *);
	/*! \brief Output a char */
	Dbug& operator<<(char);
	/*! \brief Output an integer */
	Dbug& operator<<(int);
	/*! \brief Output an unsigned int */
	Dbug& operator<<(unsigned int);

#ifndef __link
	Dbug& operator<<(uint32_t a) { return this->operator<<((unsigned int)a); }
	Dbug& operator<<(int32_t a) { return this->operator<<((int)a); }
#endif



#ifdef USE_FLOAT
	Dbug& operator<<(float);
	Dbug& operator<<(double);
#endif

	virtual int write(const void * buf, int nbyte) = 0;

private:

	Dbug::mode current;
	int width;
};

};


#endif /* DEBUG_H_ */
