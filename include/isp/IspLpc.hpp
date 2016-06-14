/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef LPCISP_HPP_
#define LPCISP_HPP_

#include <mcu/types.h>
#include "../hal/Uart.hpp"
#include "../hal/Pin.hpp"
#include "Isp.hpp"
#include "LpcPhy.hpp"

namespace isp {

class IspLpc : public Isp {
public:
	IspLpc(hal::Uart * uart, hal::Pin * rst, hal::Pin * ispreq) : phy(uart, rst, ispreq){}

	int program(const char * filename, int crystal, const char * dev, int (*progress)(float));
	int read(const char * filename, int crystal, int (*progress)(float));
	char ** getlist();

	int copy_names(char * device, char * pio0, char * pio1);
	int init_phy(int uart_pinassign){ return phy.init(uart_pinassign); }
	int exit_phy(){ return phy.exit(); }
	int reset(){ return phy.reset(); }


private:
	LpcPhy phy;
	int init_prog_interface(int crystal);
	int erase_dev();
	u32 write_progmem(void * data, u32 addr, u32 size, int (*update_disp)(float));
	u32 read_progmem(void * data, u32 addr, u32 size, int (*update_disp)(float));
	u16 verify_progmem(
			void * data,
			void * original_data,
			u32 addr,
			u32 size,
			int (*update_disp)(float));

	int write_vector_checksum(unsigned char * hex_buffer, const char * dev);
	int prog_shutdown();
};

};

#endif /* LPCISP_HPP_ */
