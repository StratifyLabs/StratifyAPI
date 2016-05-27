/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef MLCDDEVICE_HPP_
#define MLCDDEVICE_HPP_

#include <iface/dev/mlcd.h>
#include "Dev.hpp"
#include "LcdObject.hpp"

namespace hal {

class LcdDev : public Dev, public LcdObject {
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

#endif /* MLCDDEVICE_HPP_ */
