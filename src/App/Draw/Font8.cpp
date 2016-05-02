//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <cstdio>
#include "Draw/Font8.hpp"
using namespace Draw;


Font8::Font8(const font8_t * f){
	// TODO Auto-generated constructor stub
	this->f = f;

}


int Font8::load(const char * path){
	int fd;
	int ret;
	font8_t * tmp;

	if( f == 0 ){
		tmp = (font8_t *)malloc(sizeof(font8_t));
	} else {
		tmp = (font8_t *)f;
	}

	fd = ::open(path, O_RDWR);
	if ( fd < 0 ){
		free(tmp);
		return -1;
	}

	if( (ret  = ::read(fd, tmp, sizeof(font8_t))) < 0){
		::close(fd);
		free(tmp);
		return -1;
	}

	::close(fd);
	f = tmp;
	return 0;
}

MBitmap Font8::bitmap(char c) const{
	int i;
	int w;
	i = c - ' '; //space is the first available character
	if ( i < 0 ){
		return MBitmap((mg_bitmap_t*)0, 0, 0);
	}

	if( i < FONT8_COUNT ){
		w = f->characters[i][7];
		if( f->characters[i][7] == 0 ){
			w = f->w;
		}
		return MBitmap((mg_bitmap_t*)&(f->characters[i][0]), w, f->h, true);
	}

	return MBitmap((mg_bitmap_t*)0, 0, 0);

}
