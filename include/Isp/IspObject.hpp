/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef ISPOBJECT_HPP_
#define ISPOBJECT_HPP_


namespace Isp {

class IspObject {
public:
	IspObject(){}

	virtual int program(const char * filename, int crystal, const char * dev, int (*progress)(float)) = 0;
	virtual int read(const char * filename, int crystal, int (*progress)(float)) = 0;
	virtual char ** getlist(void) = 0;
	virtual int copy_names(char * device, char * pio0, char * pio1) = 0;
	virtual int initphy(int uart_pinassign) = 0;
	virtual int exitphy(void) = 0;
	virtual int reset(void) = 0;

	static int name_maxsize(){ return 32; }

};
};

#endif /* ISPOBJECT_HPP_ */
