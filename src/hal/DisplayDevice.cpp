//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "../../include/hal/DisplayDevice.hpp"

#include "sys.hpp"

namespace hal {

DisplayDevice::DisplayDevice(){}

/*! \brief Pure virtual function to initialize the LCD */
int DisplayDevice::init(const char * name){
	//open and populate attr
	display_info_t info;

	if( name != 0 ){
		if ( Device::open(name, READWRITE) < 0 ){
			return -1;
		}

		if( ioctl(I_DISPLAY_GETINFO, &info) < 0 ){
			return -1;
		}

		set_data((sg_bmap_data_t*)info.mem, info.width, info.height);

		set_margin_left(info.margin_left);
		set_margin_right(info.margin_right);
		set_margin_top(info.margin_top);
		set_margin_bottom(info.margin_bottom);

		return 0;
	}
	return -1;
}

/*! \brief Pure virtual function that copies local LCD memory to the LCD screen */
void DisplayDevice::refresh() const {
	ioctl(I_DISPLAY_REFRESH);
}

int DisplayDevice::enable() const {
	return ioctl(I_DISPLAY_ENABLE);
}

/*! \brief Turn the LCD pixels off */
int DisplayDevice::disable() const {
	return ioctl(I_DISPLAY_DISABLE);
}

void DisplayDevice::wait(u16 resolution) const{
	bool ret;
	do {
		if( (ret = is_busy()) ){
			Timer::wait_usec(resolution);
		}
	} while( ret );
}

bool DisplayDevice::is_busy() const {
	return ioctl(I_DISPLAY_ISBUSY) == 1;
}

} /* namespace hal */
