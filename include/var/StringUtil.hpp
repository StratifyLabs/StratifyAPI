/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef STRINGUTIL_HPP_
#define STRINGUTIL_HPP_

#include <stdint.h>

namespace var {

#if defined __armv7m || defined __armv7em
#define USE_SPRINTF
#endif

class StringUtil {
public:

	enum mode{
		BINARY /*! Binary mode */,
		OCTAL /*! Octal mode */,
		hexadecimal /*! Hexadecimal mode (lowercase) */,
		HEXADECIMAL /*! Hexadecimal mode (uppercase) */,
		DECIMAL /*! Decimal mode (Default); used exclusively by signed types */,
		MODE_TOTAL
	};


	enum {
		BUF_SIZE = 36
	};

	static int utoa_mode(enum StringUtil::mode mode, char dest[BUF_SIZE], uint32_t num, int width);

	/*! \brief Converts a signed integer to a string (base 10 only) */
	static int itoa(char dest[BUF_SIZE], int32_t num, int width = 0);
	/*! \brief Converts an unsigned integer to a string (any base--most useful are of course 2, 8, 10 and 16) */
	static int utoa(char dest[BUF_SIZE], uint32_t num, int base = 10, bool upper = true, int width = 0);
#ifdef USE_FLOAT
	static int ftoa(char dest[BUF_SIZE], float num, int width = 0);
#endif

private:
	static char htoc(int nibble);
	static char Htoc(int nibble);

};

}

#endif /* STRINGUTIL_HPP_ */
