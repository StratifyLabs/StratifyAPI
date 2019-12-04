/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_HAL_DISPLAYDEVICE_HPP_
#define SAPI_HAL_DISPLAYDEVICE_HPP_

#include "Device.hpp"
#include "Display.hpp"

namespace hal {


/*! \brief Display Device
 * \details This class is a display device. It inherits
 * both Display and Dev so that a display
 * found at, for example, "/dev/display0" can
 * be drawn on.
 */
class DisplayDevice :
      public Display,
      public Device {
public:
	DisplayDevice();

	int initialize(const var::String & name);

	/*! \details Refreshes the display.
	 *
	 * This method will cause the driver to write the
	 * current video memory to the display.
	 */
	void refresh() const;

	void clear();

	int set_window(const sgfx::Region & region);

	enum mode {
		PALETTE,
		RAW
	};

	DisplayInfo get_info() const;
	DisplayPalette get_palette() const;

	int set_mode(enum mode value);

	/*! \details Returns true if the display is
	 * actively copying the video buffer to the display
	 *
	 * @return True if the LCD is busy
	 */
	bool is_busy() const;

	/*! \details Blocks until the display is not busy anymore. */
	void wait(const chrono::MicroTime & resolution) const;

	int enable() const;
	int disable() const;

	using Data::size;
};

} /* namespace hal */

#endif /* SAPI_HAL_DISPLAYDEVICE_HPP_ */
