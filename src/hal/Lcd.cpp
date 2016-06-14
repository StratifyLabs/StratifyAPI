//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#if !defined __link

#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <semaphore.h>
#include <iface/link.h>
#include <errno.h>
#include <hal/Lcd.hpp>

#include "sys/Timer.hpp"

using namespace hal;
using namespace sys;


Lcd::Lcd(sg_bmap_data_t * mem, sg_size_t w, sg_size_t h) : Bitmap(mem,w,h){}


#endif



