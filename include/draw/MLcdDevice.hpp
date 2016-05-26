/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef MLCDDEVICE_HPP_
#define MLCDDEVICE_HPP_

#include <iface/dev/mlcd.h>
#include "../draw/MLcd.hpp"
#include "../hal/Dev.hpp"

namespace draw {

class MLcdDevice : public hal::Dev, public MLcd {
public:
	MLcdDevice();

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
