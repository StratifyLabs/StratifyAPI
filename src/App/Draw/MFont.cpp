//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include "Draw/MFont.hpp"
using namespace Draw;


static const char ascii_charset_[MFont::CHARSET_SIZE+1] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";


const char * MFont::charset(){
	return ascii_charset_;
}

int MFont::to_charset(char ascii){
	if( (ascii < ' ') ||
			(ascii > '~') ){
		return -1;
	}
	return (int)(ascii - ' ');
}
