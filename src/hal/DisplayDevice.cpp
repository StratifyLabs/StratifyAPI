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

		allocate(sgfx::Area(info.width, info.height));

		set_margin_left(info.margin_left);
		set_margin_right(info.margin_right);
		set_margin_top(info.margin_top);
		set_margin_bottom(info.margin_bottom);

		display_attr_t attr;
		attr.o_flags = DISPLAY_FLAG_INIT;
		if( ioctl(I_DISPLAY_SETATTR, &attr) < 0 ){
			return -1;
		}

		return 0;
	}
	return -1;
}

DisplayInfo DisplayDevice::get_info() const {
	display_info_t info;
	if( ioctl(I_DISPLAY_GETINFO, &info) < 0 ){
		return DisplayInfo();
	}
	return DisplayInfo(info);
}

DisplayPalette DisplayDevice::get_palette() const {
	display_palette_t palette;
	if( ioctl(I_DISPLAY_GETPALETTE, &palette) < 0 ){
		return DisplayPalette();
	}
	return DisplayPalette(palette);
}


int DisplayDevice::set_window(const sgfx::Region & region){
	display_attr_t attr;
	attr.o_flags = DISPLAY_FLAG_SET_WINDOW;
	attr.window_x = region.point().x();
	attr.window_y = region.point().y();
	attr.window_width = region.area().width();
	attr.window_height = region.area().height();
	return ioctl(I_DISPLAY_SETATTR, &attr);
}

int DisplayDevice::set_mode(enum mode value){
	display_attr_t attr;
	attr.o_flags = DISPLAY_FLAG_SET_MODE;
	if( value == PALETTE ){
		attr.o_flags |= DISPLAY_FLAG_IS_MODE_PALETTE;
	} else {
		attr.o_flags |= DISPLAY_FLAG_IS_MODE_RAW;
	}
	return ioctl(I_DISPLAY_SETATTR, &attr);
}

void DisplayDevice::refresh() const {

	//write the bitmap to the display

	ioctl(I_DISPLAY_REFRESH);
}

int DisplayDevice::enable() const {
	display_attr_t attr;
	attr.o_flags = DISPLAY_FLAG_ENABLE;
	return ioctl(I_DISPLAY_SETATTR, &attr);
}

void DisplayDevice::clear(){
	if( ioctl(I_DISPLAY_CLEAR) < 0 ){
		Data::clear();
	}
}

int DisplayDevice::disable() const {
	display_attr_t attr;
	attr.o_flags = DISPLAY_FLAG_DISABLE;
	return ioctl(I_DISPLAY_SETATTR, &attr);
}

void DisplayDevice::wait(const chrono::MicroTime & resolution) const{
	bool ret;
	do {
		if( (ret = is_busy()) ){
			resolution.wait();
		}
	} while( ret );
}

bool DisplayDevice::is_busy() const {
	return ioctl(I_DISPLAY_ISBUSY) == 1;
}

} /* namespace hal */
