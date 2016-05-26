/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef HIO_HPP_
#define HIO_HPP_

#include <Hal/Dev.hpp>
#include <iface/dev/hio.h>


namespace Sys {

class Hio : public Hal::Dev {
public:

	int attr(hio_attr_t & a);
	int report(void * dest, int size);
	int set_report(const void * src, int size);

};

};

#endif /* HIO_HPP_ */
