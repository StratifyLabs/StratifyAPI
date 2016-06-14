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
	void refresh();
	void clear();
	int on();
	int off();
	int hold();
	int wait();
	int release();
	int orient_y() const;
	int orient_x() const;

	bool busy();

protected:
	//mlcd_attr_t attr;
};

};

#endif

#endif /* MLCDDEVICE_HPP_ */
