//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <semaphore.h>
#include "Sys/Timer.hpp"
#include "Draw/MLcd.hpp"

#include <iface/link.h>
#include <errno.h>

#ifdef __link
#undef errno
#define errno link_errno
#endif

using namespace Draw;
using namespace Sys;


#ifndef __HWPL_ONLY__


MLcd::MLcd(mg_bitmap_t * mem, mg_size_t w, mg_size_t h) : MBitmap(mem,w,h){}



#else



#endif



