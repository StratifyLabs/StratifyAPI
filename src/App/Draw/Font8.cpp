//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <draw/Font8.hpp>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <cstdio>
using namespace draw;


Font8::Font8(const font8_t * f){
	// TODO Auto-generated constructor stub
	this->m_font = f;

}


int Font8::load(const char * path){
	int fd;
	int ret;
	font8_t * tmp;

	if( m_font == 0 ){
		tmp = (font8_t *)malloc(sizeof(font8_t));
	} else {
		tmp = (font8_t *)m_font;
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
	m_font = tmp;
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
		w = m_font->characters[i][7];
		if( m_font->characters[i][7] == 0 ){
			w = m_font->w;
		}
		return MBitmap((mg_bitmap_t*)&(m_font->characters[i][0]), w, m_font->h, true);
	}

	return MBitmap((mg_bitmap_t*)0, 0, 0);

}
