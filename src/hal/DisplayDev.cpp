//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "sys.hpp"
#include "hal/DisplayDev.hpp"

namespace hal {

DisplayDev::DisplayDev(){}

/*! \brief Pure virtual function to initialize the LCD */
int DisplayDev::init(const char * name){
	//open and populate attr
	display_attr_t attr;

	if( name != 0 ){
		if ( Dev::open(name, READWRITE) < 0 ){
			return -1;
		}

		if( ioctl(I_DISPLAY_GETINFO, &attr) < 0 ){
			return -1;
		}

		set_data((sg_bmap_data_t*)attr.mem, attr.w, attr.h);

		set_margin_left(attr.margin_left);
		set_margin_right(attr.margin_right);
		set_margin_top(attr.margin_top);
		set_margin_bottom(attr.margin_bottom);

		return 0;
	}
	return -1;
}

/*! \brief Pure virtual function that copies local LCD memory to the LCD screen */
void DisplayDev::refresh() const {
	ioctl(I_DISPLAY_REFRESH);
}

int DisplayDev::on(){
	return ioctl(I_DISPLAY_ON);
}

/*! \brief Turn the LCD pixels off */
int DisplayDev::off(){
	return ioctl(I_DISPLAY_OFF);
}

void DisplayDev::wait(u16 resolution) const{
	bool ret;
	do {
		if( (ret = busy()) ){
			Timer::wait_usec(resolution);
		}
	} while( ret );
}

bool DisplayDev::busy() const {
	return ioctl(I_DISPLAY_ISBUSY) == 1;
}

} /* namespace hal */
