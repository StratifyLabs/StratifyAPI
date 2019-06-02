//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "../../include/hal/DisplayDevice.hpp"

#include "sys.hpp"

namespace hal {

DisplayDevice::DisplayDevice(){}

/*! \brief Pure virtual function to initialize the LCD */
int DisplayDevice::initialize(const var::ConstString & name){
	//open and populate attr

	if( name != 0 ){

		//only open the device if it isn't already open
		if ( Device::open(name, READWRITE) < 0 ){
			return -1;
		}

		display_info_t info;
		if( ioctl(I_DISPLAY_GETINFO, &info) < 0 ){
			return -1;
		}

		set_bits_per_pixel(info.bits_per_pixel);
		if( info.mem != 0 ){
			set_data((sg_bmap_data_t*)info.mem, info.width, info.height);
		} else {
			allocate(sgfx::Area(info.width, info.height));
		}

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

void DisplayDevice::clear(){
	if( ioctl(I_DISPLAY_CLEAR) < 0 ){
		Data::clear();
	}
}

DisplayInfo DisplayDevice::get_info() const {
	display_info_t info;
	if( ioctl(I_DISPLAY_GETINFO, &info) < 0 ){
		return DisplayInfo();
	}
	return DisplayInfo(info);
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
