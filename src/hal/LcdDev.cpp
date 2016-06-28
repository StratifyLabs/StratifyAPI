//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved
#if !defined __link


#include <errno.h>
#include <iface/dev/mlcd.h>
#include "sys/Timer.hpp"
#include "hal/LcdDev.hpp"

using namespace sys;
using namespace hal;

LcdDev::LcdDev(){}


/*! \brief Pure virtual function to initialize the LCD */
int LcdDev::init(const char * name){
	//open and populate attr
	mlcd_attr_t attr;

	if( name != 0 ){
		if ( Dev::open(name, READWRITE) < 0 ){
			return -1;
		}

		if( ioctl(I_MLCD_GETATTR, &attr) < 0 ){
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
void LcdDev::refresh() const {
	ioctl(I_MLCD_TOUCH);
}

int LcdDev::on(){
	return ioctl(I_MLCD_ON);
}

/*! \brief Turn the LCD pixels off */
int LcdDev::off(){
	return ioctl(I_MLCD_OFF);
}

void LcdDev::wait(u16 resolution) const{
	bool ret;
	do {
		if( (ret = busy()) ){
			Timer::wait_usec(resolution);
		}
	} while( ret );
}

bool LcdDev::busy() const {
	return ioctl(I_MLCD_BUSY) == 1;
}


#endif //__link not defined


