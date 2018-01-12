//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include "hal/Dev.hpp"
#include "sys/Timer.hpp"
using namespace sys;
using namespace hal;

#if defined __link
Dev::Dev(link_transport_mdriver_t * d) : Device(d){}
#endif

Dev::Dev(){}

