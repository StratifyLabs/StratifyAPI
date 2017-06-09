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

	/*! \details This method will cause the display to
	 * copy the memory in the video buffer over to the display
	 */
	void refresh() const;

	/*! \details This method will return true if the display is
	 * actively copying the video buffer to the display
	 *
	 * @return True if the LCD is busy
	 */
	bool busy() const;

	/*! \details This method will block until the LCD is not busy anymore */
	void wait(u16 resolution) const;

	/*! \details This method will turn the display on
	 *
	 * @return 0 on success
	 */
	int on();

	/*! \details This method will turn the display off
	 *
	 * @return 0 on success
	 */
	int off();
};

} /* namespace hal */

#endif /* HAL_DISPLAYDEV_HPP_ */
