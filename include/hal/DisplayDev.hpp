/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef HAL_DISPLAYDEV_HPP_
#define HAL_DISPLAYDEV_HPP_

#include "Dev.hpp"
#include "Display.hpp"

namespace hal {

class DisplayDev : public Display, public Dev {
public:
	DisplayDev();

	int init(const char * name);

	/*! \details Refreshes the display.
	 *
	 * This method will cause the driver to write the
	 * current video memory to the display.
	 */
	void refresh() const;

	/*! \details Returns true if the display is
	 * actively copying the video buffer to the display
	 *
	 * @return True if the LCD is busy
	 */
	bool busy() const;

	/*! \details Blocks until the display is not busy anymore. */
	void wait(u16 resolution) const;

	int on();
	int off();
};

} /* namespace hal */

#endif /* HAL_DISPLAYDEV_HPP_ */
