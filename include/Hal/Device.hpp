/*
 * Device.hpp
 *
 *  Created on: Apr 28, 2014
 *      Author: tgil
 */

#ifndef DEVICE_HPP_
#define DEVICE_HPP_

#include "Periph.hpp"

namespace Hal {

class Device : public Periph {
public:
	Device();
	int open(const char * name, int flags = 0);

private:
	int fd;
	bool is_initialized;
};

};

#endif /* DEVICE_HPP_ */
