//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include "sys/Dbug.hpp"
#include "var/StringUtil.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>

using namespace var;
using namespace sys;

Dbug::Dbug() {
	// TODO Auto-generated constructor stub
	current = DECIMAL;
	width = 0;
}

void Dbug::setmode(Dbug::mode mode, int leading_zeros){
	this->current = mode;
	this->width = leading_zeros;
}

int Dbug::puts(const char * str){
	int ret;
	int total;
	total = 0;
	while( (ret = this->write(str, strlen(str))) >  0){
		str+=ret;
		total+=ret;
	}
	return total;
}

Dbug& Dbug::operator<<(const char * str){
	this->puts(str);
	return *this;
}

Dbug& Dbug::operator<<(char a){
	return this->operator<<((int)a);
}


Dbug& Dbug::operator<<(int a){
	char buffer[BUF_SIZE];
	int len;
	len = StringUtil::itoa(buffer, a);
	if( len > BUF_SIZE ){
		len = BUF_SIZE;
	}
	if( len < 0 ){
		return *this;
	}
	this->write(buffer, len);
	return *this;
}

Dbug& Dbug::operator<<(unsigned int a){
	char buffer[BUF_SIZE];
	int len = 0;
	switch(current){
	case BINARY:
		len = StringUtil::utoa(buffer, a, 2, false, width);
		break;
	case OCTAL:
		//convert to octal
		len = StringUtil::utoa(buffer, a, 8, false, width);
		break;
	case hexadecimal:
		//convert to hex
		len = StringUtil::utoa(buffer, a, 16, false, width);
		break;
	case HEXADECIMAL:
		//convert to HEX
		len = StringUtil::utoa(buffer, a, 16, true, width);
		break;
	case DECIMAL:
		len = StringUtil::utoa(buffer, a, 10, width);
		break;
	}

	if( len > BUF_SIZE ){
		len = BUF_SIZE;
	}
	if( len <= 0 ){
		return *this;
	}

	this->write(buffer, len);
	return *this;
}

#ifdef USE_FLOAT
Dbug& Dbug::operator<<(float a){
	int len;
	char buffer[24];
	len = ftoa(buffer, a, width);
	this->write(buffer, len);
	return *this;
}

Dbug& Dbug::operator<<(double a){
	//convert to double
	int len;
	char buffer[24];
	len = ftoa(buffer, a, width);
	this->write(buffer, len);
	return *this;
}
#endif


