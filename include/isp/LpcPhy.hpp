/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef LPCPHY_HPP_
#define LPCPHY_HPP_

#include "../hal/Uart.hpp"
#include "../hal/Pin.hpp"
#include "../sys/Trace.hpp"

#define LPC_ISP_UNLOCK_CODE "23130"

#define LPCPHY_RAM_BUFFER_SIZE 512

namespace isp {

class LpcPhy {
public:
	LpcPhy(hal::Uart & uart, hal::Pin & reset, hal::Pin & ispreq) : m_uart(uart), m_reset(reset), m_ispreq(ispreq){
		m_echo = 0;
		m_ram_buffer = 0x40000200;
		m_is_return_code_newline = false;
		m_is_uuencode = false;
		m_pinassign = 0;
		m_max_speed = MAX_SPEED_115200;
	}


	int init(int pinassign);
	int exit();
	int open(int crystal);
	int close();
	int write_memory(u32 loc, const void * buf, int nbyte, u32 sector);
	void set_ram_buffer(u32 addr);
	int read_memory(u32 loc, void * buf, int nbyte);
	int reset();
	int start_bootloader();
	int sync_bootloader(u32 crystal);
	int sync_bootloader_lpc177x_8x(u32 crystal);
	int unlock(const char * unlock_code);
	int disable_echo();
	int enable_echo();
	int write_ram(u32 ram_dest /*! The RAM destination address--must be a word boundary */,
			void * src /*! A pointer to the source data */,
			u32 size /*! The number of bytes to write--must be a multiple of 4 */);
	u32 read_mem(void * dest /*! A pointer to the destination memory */,
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
	u32 read_part_id();
	u32 read_boot_version();
	int compare_memory(u32 addr0 /*! The beginning of the first block */,
			u32 addr1 /*! The beginning of the second block */,
			u32 size /*! The number of bytes to compare */);

	void set_uuencode(bool v = true){ m_is_uuencode = v; }
	bool is_uuencode() const  { return m_is_uuencode; }

	void set_max_speed(u16 v){
		m_max_speed = v;
		if( m_max_speed > MAX_SPEED_9600 ){
			m_max_speed = MAX_SPEED_9600;
		}
	}

	enum {
		MAX_SPEED_115200,
		MAX_SPEED_78600,
		MAX_SPEED_57600,
		MAX_SPEED_38400,
		MAX_SPEED_19200,
		MAX_SPEED_9600
	};

private:
	char m_echo;
	hal::Uart & m_uart;
	hal::Pin & m_reset;
	hal::Pin & m_ispreq;
	u32 m_ram_buffer;
	int m_pinassign;
	bool m_is_return_code_newline;
	bool m_is_uuencode;
	u16 m_max_speed;

	int send_command(const char * cmd, int timeout, int wait_ms = 0);
	s32 write_data(void * src, u32 size);
	s32 read_data(void * dest, u32 size);
	int get_line(void * buf, int nbyte, int max_wait);
	int read_return_code(u16 timeout);
	int wait_response(const char * response, u16 timeout);
	int wait_ok(u16 timeout);


	bool is_return_code_newline(){ return m_is_return_code_newline; }
	void set_return_code_newline(bool enabled = true){ m_is_return_code_newline = enabled; }


	sys::Trace m_trace;
	int flush();

};

};

#endif /* LPCISP_HPP_ */
