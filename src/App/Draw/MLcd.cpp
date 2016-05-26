//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <draw/MLcd.hpp>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <semaphore.h>
#include <iface/link.h>
#include <errno.h>
#include <sys/Timer.hpp>

#ifdef __link
#undef errno
#define errno link_errno
#endif

using namespace draw;
using namespace sys;


#ifndef __HWPL_ONLY__


MLcd::MLcd(mg_bitmap_t * mem, mg_size_t w, mg_size_t h) : MBitmap(mem,w,h){}



#else



#endif



