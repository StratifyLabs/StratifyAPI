/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef HIO_HPP_
#define HIO_HPP_

#include <iface/dev/hio.h>

#include "../Hal/Phy.hpp"

namespace Sys {

class Hio : public Hal::Phy {
public:

	int attr(hio_attr_t & a);
	int report(void * dest, int size);
	int setreport(const void * src, int size);

};

};

#endif /* HIO_HPP_ */
