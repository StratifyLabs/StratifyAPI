//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "../../include/hal/DisplayDevice.hpp"

#include "sys.hpp"

namespace hal {

DisplayDevice::DisplayDevice(){}

/*! \brief Pure virtual function to initialize the LCD */
int DisplayDevice::initialize(
		const var::String & name,
		IsAllocate is_allocate
		){
	//open and populate attr

	if( name.is_empty() == false ){

		//only open the device if it isn't already open
		if ( Device::open(
				  name,
				  fs::OpenFlags::read_write()) < 0 ){
			return -1;
		}

		display_info_t info;
		if( ioctl(
				 IoRequest(I_DISPLAY_GETINFO),
				 IoArgument(&info)
				 ) < 0 ){
			return -1;
		}

		if( is_allocate.argument() ){
			allocate(
						sgfx::Area(info.width, info.height),
						BitsPerPixel(info.bits_per_pixel)
						);
		} else {
			refer_to(
						ReadOnlyBuffer(nullptr),
						sgfx::Area(info.width, info.height),
						BitsPerPixel(info.bits_per_pixel)
						);
		}

		set_margin_left(info.margin_left);
		set_margin_right(info.margin_right);
		set_margin_top(info.margin_top);
		set_margin_bottom(info.margin_bottom);

		display_attr_t attr;
		attr.o_flags = DISPLAY_FLAG_INIT;
		if( ioctl(
				 IoRequest(I_DISPLAY_SETATTR),
				 IoArgument(&attr)
				 ) < 0 ){
			return -1;
		}

		return 0;
	}
	return -1;
}

DisplayInfo DisplayDevice::get_info() const {
	display_info_t info;
	if( ioctl(
			 IoRequest(I_DISPLAY_GETINFO),
			 IoArgument(&info)
			 ) < 0 ){
		return DisplayInfo();
	}
	return DisplayInfo(info);
}

DisplayPalette DisplayDevice::get_palette() const {
	display_palette_t palette;
	if( ioctl(
			 IoRequest(I_DISPLAY_GETPALETTE),
			 IoArgument(&palette)
			 ) < 0 ){
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
	return ioctl(
				IoRequest(I_DISPLAY_SETATTR),
				IoArgument(&attr)
				);
}

int DisplayDevice::set_mode(enum mode value){
	display_attr_t attr;
	attr.o_flags = DISPLAY_FLAG_SET_MODE;
	if( value == PALETTE ){
		attr.o_flags |= DISPLAY_FLAG_IS_MODE_PALETTE;
	} else {
		attr.o_flags |= DISPLAY_FLAG_IS_MODE_RAW;
	}
	return ioctl(
				IoRequest(I_DISPLAY_SETATTR),
				IoArgument(&attr)
				);
}

void DisplayDevice::refresh() const {

	//write the bitmap to the display

	ioctl(IoRequest(I_DISPLAY_REFRESH));
}

int DisplayDevice::enable() const {
	display_attr_t attr;
	attr.o_flags = DISPLAY_FLAG_ENABLE;
	return ioctl(
				IoRequest(I_DISPLAY_SETATTR),
				IoArgument(&attr)
				);
}

void DisplayDevice::clear(){
	display_attr_t attributes;
	attributes.o_flags = DISPLAY_FLAG_CLEAR;
	if( ioctl(
			 IoRequest(I_DISPLAY_CLEAR),
			 IoArgument(&attributes)
			 ) < 0 ){
		Data::clear();
	}
}

int DisplayDevice::disable() const {
	display_attr_t attr;
	attr.o_flags = DISPLAY_FLAG_DISABLE;
	return ioctl(
				IoRequest(I_DISPLAY_SETATTR),
				IoArgument(&attr)
				);
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
	return ioctl(IoRequest(I_DISPLAY_ISBUSY)) == 1;
}

} /* namespace hal */
