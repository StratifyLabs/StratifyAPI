/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef LPCPHY_HPP_
#define LPCPHY_HPP_


#include <mcu/types.h>

#include "Hal/Uart.hpp"
#include "Hal/Pin.hpp"

#define LPC_ISP_UNLOCK_CODE "23130"

#define LPCPHY_RAM_BUFFER_SIZE 512

namespace Isp {

class LpcPhy {
public:
	LpcPhy(Hal::Uart * uart, Hal::Pin * reset, Hal::Pin * ispreq){
		this->uart = uart;
		this->rst = reset;
		this->ispreq = ispreq;
		echo = 0;
		ram_buffer = 0x40000200;
	}


	int init(int pinassign);
	int exit(void);
	int open(int crystal);
	int close(void);
	int writemem(u32 loc, const void * buf, int nbyte, u32 sector);
	void set_ram_buffer(u32 addr);
	int readmem(u32 loc, void * buf, int nbyte);
	int reset(void);
	int start_bootloader(void);
	int sync_bootloader(u32 crystal);
	int sync_bootloader_lpc177x_8x(u32 crystal);
	int unlock(const char * unlock_code);
	int echo_off(void);
	int echo_on(void);
	int wr_ram(u32 ram_dest /*! The RAM destination address--must be a word boundary */,
			void * src /*! A pointer to the source data */,
			u32 size /*! The number of bytes to write--must be a multiple of 4 */);
	u32 rd_mem(void * dest /*! A pointer to the destination memory */,
			u32 src_addr /*! The source address to read--must be a word boundary */,
			u32 size /*! The number of bytes to read--must be a multiple of 4 */);
	int prep_sector(u32 start /*! The first sector to prepare */,
			u32 end /*! The last sector to prepare--must be >= start */);
	int cpy_ram_to_flash(u32 flash_addr /*! The flash address (destination memory) */,
			u32 ram_addr /*! The RAM address (source memory) */,
			u32 size /*! The number of bytes to copy */);
	int go(u32 addr /*! Where to start code execution */,
			char mode /*! 'T' for thumb mode and 'A' for arm mode--default is 'A' */);
	int erase_sector(u32 start /*! The first sector to erase */,
			u32 end /*! The last sector to erase--must be >= start */);
	int blank_check_sector(u32 start /*! The first sector to blank check */,
			u32 end /*! The last sector to blank check--must be >= start */);
	u32 rd_part_id(void);
	u32 rd_boot_version(void);
	int memcmp(u32 addr0 /*! The beginning of the first block */,
			u32 addr1 /*! The beginning of the second block */,
			u32 size /*! The number of bytes to compare */);

private:
	char echo;
	Hal::Uart * uart;
	Hal::Pin * ispreq;
	Hal::Pin * rst;
	u32 ram_buffer;
	int32_t lpc_write_data(void * src, u32 size);
	int32_t lpc_read_data(void * dest, u32 size);
	int get_line(void * buf, int nbyte, int max_wait);
	int lpc_rd_return_code(u16 timeout);
	int lpc_wait_response(const char * response, u16 timeout);
	int lpc_wait_ok(u16 timeout);

	bool islpc177x_8x_;
	bool islpc177x_8x(){ return islpc177x_8x_; }
	void setlpc177x_8x(bool enabled = true){ islpc177x_8x_ = enabled; }

	int sendcommand(const char * cmd, int timeout, int wait_ms = 0);

	int flush(void);

};

};

#endif /* LPCISP_HPP_ */
