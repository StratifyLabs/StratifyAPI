/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef LPCPHY_HPP_
#define LPCPHY_HPP_

#include "../hal/Uart.hpp"
#include "../hal/Pin.hpp"

#define LPC_ISP_UNLOCK_CODE "23130"

#define LPCPHY_RAM_BUFFER_SIZE 512

namespace isp {

class LpcPhy {
public:
	LpcPhy(hal::Uart * uart, hal::Pin * reset, hal::Pin * ispreq){
		this->uart = uart;
		this->rst = reset;
		this->ispreq = ispreq;
		echo = 0;
		ram_buffer = 0x40000200;
		_is_lpc177x_8x = false;
	}


	int init(int pinassign);
	int exit();
	int open(int crystal);
	int close();
	int writemem(u32 loc, const void * buf, int nbyte, u32 sector);
	void set_ram_buffer(u32 addr);
	int readmem(u32 loc, void * buf, int nbyte);
	int reset();
	int start_bootloader();
	int sync_bootloader(u32 crystal);
	int sync_bootloader_lpc177x_8x(u32 crystal);
	int unlock(const char * unlock_code);
	int echo_off();
	int echo_on();
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
	u32 rd_part_id();
	u32 rd_boot_version();
	int memcmp(u32 addr0 /*! The beginning of the first block */,
			u32 addr1 /*! The beginning of the second block */,
			u32 size /*! The number of bytes to compare */);

private:
	char echo;
	hal::Uart * uart;
	hal::Pin * ispreq;
	hal::Pin * rst;
	u32 ram_buffer;
	int32_t lpc_write_data(void * src, u32 size);
	int32_t lpc_read_data(void * dest, u32 size);
	int get_line(void * buf, int nbyte, int max_wait);
	int lpc_rd_return_code(u16 timeout);
	int lpc_wait_response(const char * response, u16 timeout);
	int lpc_wait_ok(u16 timeout);

	bool _is_lpc177x_8x;
	bool is_lpc177x_8x(){ return _is_lpc177x_8x; }
	void set_lpc177x_8x(bool enabled = true){ _is_lpc177x_8x = enabled; }

	int sendcommand(const char * cmd, int timeout, int wait_ms = 0);

	int flush();

};

};

#endif /* LPCISP_HPP_ */
