

#ifndef SAPI_DEV_HPP_
#define SAPI_DEV_HPP_

#include "Device.hpp"

namespace hal {

class Dev : public Device {
public:
#if defined __link
	Dev(link_transport_mdriver_t * d);
	Dev();
#else
	Dev();
#endif

};

};

#endif /* SAPI_DEV_HPP_ */
