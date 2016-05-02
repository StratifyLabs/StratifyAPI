/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef MLCD_HPP_
#define MLCD_HPP_

#include "MBitmap.hpp"

namespace Draw {

/*! \brief Monochrome LCD
 * \details This is an abstract class for drawing graphics and text
 * on a monochrome LCD.  The typical implementation will keep a copy
 * of the bitmap in local memory and use the refresh() method to copy
 * the local memory to the LCD.
 *
 *
 */
class MLcd : public MBitmap {
public:
	/*! \brief Construct MLcd with known width, height, and memory pointer */
	MLcd(mg_bitmap_t * mem, mg_size_t w, mg_size_t h);

	/*! \brief Construct MLcd with unknown dimensions (use alloc(w,h) later) */
	MLcd(){}


	/*! \brief Pure virtual function to initialize the LCD */
	virtual int init(const char * name = 0) = 0;

	/*! \brief Pure virtual function that copies local LCD memory to the LCD screen */
	virtual void refresh(void){}

	/*! \brief Turn the LCD pixels on */
	virtual int on(){ return -1; };

	/*! \brief Turn the LCD pixels off */
	virtual int off(){ return -1; };

	/*! \brief Hold LCD shared mem */
	/*! \details This tells the LCD driver the memory is being modified
	 * and the screen won't be updated until it is released.
	 * @return Zero on success
	 */
	virtual int hold(){ return -1; };

	/*! \brief Wait for the LCD shared mem to be available
	 *
	 * @return Zero on success
	 */
	virtual int wait(){ return -1; };

	/*! \brief Release LCD shared mem */
	/*! \details This tells the LCD driver the memory is done being
	 * modified and the screen should update on the next refresh
	 * event
	 * @return Zero on success
	 */
	virtual int release(){ return -1; };

	virtual int touch(){ return release(); }

	enum {
		ORIENT_TOP,
		ORIENT_BOTTOM,
		ORIENT_LEFT,
		ORIENT_RIGHT
	};

	virtual int orient_y(){ return ORIENT_TOP; }
	virtual int orient_x(){ return ORIENT_LEFT; }


};

};

#endif /* MLCD_HPP_ */
