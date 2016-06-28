/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef MLCDDEVICE_HPP_
#define MLCDDEVICE_HPP_

#ifndef __link

#include <iface/dev/mlcd.h>
#include "Dev.hpp"
#include "Lcd.hpp"


namespace hal {

class LcdDev : public Lcd, public Dev {
public:
	LcdDev();

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

protected:
	//mlcd_attr_t attr;
};

};

#endif

#endif /* MLCDDEVICE_HPP_ */
