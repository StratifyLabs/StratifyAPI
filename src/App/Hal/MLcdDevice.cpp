//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <draw/MLcdDevice.hpp>
#include <errno.h>
#include <iface/dev/mlcd.h>
#include <sys/Timer.hpp>

using namespace sys;
using namespace draw;



MLcdDevice::MLcdDevice() {

}


/*! \brief Pure virtual function to initialize the LCD */
int MLcdDevice::init(const char * name){
	//open and populate attr
	mlcd_attr_t attr;

	if( name != 0 ){
		if ( Dev::open(name, READWRITE) < 0 ){
			return -1;
		}

		if( ioctl(I_MLCD_GETATTR, &attr) < 0 ){
			return -1;
		}

		set_data((mg_bitmap_t*)attr.mem, attr.w, attr.h);

		set_margin_left(attr.margin_left);
		set_margin_right(attr.margin_right);
		set_margin_top(attr.margin_top);
		set_margin_bottom(attr.margin_bottom);

		return 0;
	}
	return -1;
}

void MLcdDevice::clear(){
	MBitmap::clear();
}

/*! \brief Pure virtual function that copies local LCD memory to the LCD screen */
void MLcdDevice::refresh(){
	ioctl(I_MLCD_TOUCH);
}

int MLcdDevice::on(){
	return ioctl(I_MLCD_ON);
}

/*! \brief Turn the LCD pixels off */
int MLcdDevice::off(){
	return ioctl(I_MLCD_OFF);
}

int MLcdDevice::hold(){
	return ioctl(I_MLCD_HOLD);
}

int MLcdDevice::wait(){
	int ret;
	do {
		errno = 0;
		ret = hold();
		if( ret < 0 ){
			Timer::wait_usec(2000);
		}
	} while( errno == EAGAIN );
	return ret;
}

int MLcdDevice::release(){
	return ioctl(I_MLCD_RELEASE);
}


int MLcdDevice::orient_y() const {
	return -1;
}

bool MLcdDevice::busy(){
	return (ioctl(I_MLCD_BUSY) != 0 );
}

int MLcdDevice::orient_x() const {
	return -1;
}


