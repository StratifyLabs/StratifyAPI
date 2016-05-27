//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <semaphore.h>
#include <iface/link.h>
#include <errno.h>

#include "hal/LcdObject.hpp"
#include "sys/Timer.hpp"

#ifdef __link
#undef errno
#define errno link_errno
#endif

using namespace hal;
using namespace sys;


#ifndef __MCU_ONLY__


LcdObject::LcdObject(sg_bmap_data_t * mem, sg_size_t w, sg_size_t h) : Bitmap(mem,w,h){}



#else



#endif



