//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef FONT8UTIL_HPP_
#define FONT8UTIL_HPP_

#include <stdint.h>

#define FONT8_COUNT 95

typedef struct {
	uint8_t w /*! \brief bit-width of font (8 maximum)*/;
	uint8_t h /*! \brief bit-height of font (8 maximum) */;
	uint8_t characters[FONT8_COUNT][8];
} font8_t;


#ifdef __cplusplus
extern "C" {
#endif

extern const font8_t font8_5x7;

#ifdef __cplusplus
}
#endif


/*! \brief Font Utility class
 * \details This class is used for creating
 * and saving new fonts.  The \ref Font class is for
 * using fonts.
 */
class Font8Util {
public:
	Font8Util();
	static int save(const char * path, const font8_t * f);

};

#endif /* FONT8UTIL_HPP_ */
