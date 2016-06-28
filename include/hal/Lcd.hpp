/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef MLCD_HPP_
#define MLCD_HPP_

#ifndef __link

#include "../sgfx/Bitmap.hpp"

namespace hal {


/*! \brief LCD
 * \details This is an abstract class for drawing graphics and text
 * on a monochrome LCD.  The typical implementation will keep a copy
 * of the bitmap in local memory and use the refresh() method to copy
 * the local memory to the LCD.
 *
 *
 */
class Lcd : public sgfx::Bitmap {
public:
	/*! \details Construct MLcd with known width, height, and memory pointer */
	Lcd(sg_bmap_data_t * mem, sg_size_t w, sg_size_t h);

	/*! \details Construct MLcd with unknown dimensions (use alloc(w,h) later) */
	Lcd(){}


	/*! \details Pure virtual function to initialize the LCD */
	virtual int init(const char * name = 0) = 0;

	/*! \details Turn the LCD pixels on */
	virtual int on(){ return -1; };

	/*! \details Turn the LCD pixels off */
	virtual int off(){ return -1; };

	enum {
		ORIENT_TOP,
		ORIENT_BOTTOM,
		ORIENT_LEFT,
		ORIENT_RIGHT
	};



};

};

#endif

#endif /* MLCD_HPP_ */
