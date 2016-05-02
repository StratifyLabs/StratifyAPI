//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "Hal/Periph.hpp"
#include "Sys/Timer.hpp"

using namespace Hal;
using namespace Sys;


#define NO_PORT_NUMBER 6

static const char * const periph_name[CORE_PERIPH_TOTAL] = {
		"resd" /* CORE_PERIPH_RESERVED RESERVED */,
		"core" /* CORE_PERIPH_CORE Core Functionality */,
		"adc" /* CORE_PERIPH_ADC Analog to Digital Converter */,
		"dac" /* CORE_PERIPH_DAC Digital to Analog Converter */,
		"uart" /* CORE_PERIPH_UART UART */,
		"spi" /* CORE_PERIPH_SPI SPI */,
		"usb" /* CORE_PERIPH_USB USB */,
		"can" /* CORE_PERIPH_CAN CAN */,
		"enet" /* CORE_PERIPH_ENET ENET */,
		"i2c" /* CORE_PERIPH_I2C I2C */,
		"i2s" /* CORE_PERIPH_I2S I2S */,
		"mem" /* CORE_PERIPH_MEM External memory interface */,
		"rtc" /* CORE_PERIPH_RTC RTC */,
		"cec" /* CORE_PERIPH_CEC Consumer Electronic Control (Part of HDMI) */,
		"qei" /* CORE_PERIPH_QEI Quadrature Encoder Interface */,
		"pwm" /* CORE_PERIPH_PWM PWM */,
		"pio" /* CORE_PERIPH_PIO GPIO */,
		"tmr" /* CORE_PERIPH_TMR Timer (output compare and input capture) */,
		"eint" /* CORE_PERIPH_EINT External interrupts */,
		"wdt" /* CORE_PERIPH_WDT Watch dog timer */,
		"bod" /* CORE_PERIPH_BOD Brown out detection */,
		"dma" /* CORE_PERIPH_DMA Direct Memory Access */,
		"jtag" /* CORE_PERIPH_JTAG JTAG */,
		"reset" /* CORE_PERIPH_RESET Reset */,
		"clkout" /* CORE_PERIPH_CLKOUT Clockout */,
		"lcd" /* CORE_PERIPH_LCD LCD */,
		"lcd" /* CORE_PERIPH_LCD1 LCD */,
		"lcd" /* CORE_PERIPH_LCD2 LCD */,
		"lcd" /* CORE_PERIPH_LCD3 LCD */,
		"emc" /* CORE_PERIPH_EMC External Memory Controller */,
		"mci" /* CORE_PERIPH_MCI Multimedia Card Interface */,
		"ssp" /* CORE_PERIPH_SSP SSP */,
		"mcpwm" /* CORE_PERIPH_MCPWM Motor Control PWM */,
		"nmi" /* CORE_PERIPH_NMI Non-maskable Interrupt */,
		"trace" /* CORE_PERIPH_TRACE Trace data */
};

u16 Periph::fd_map[OPEN_MAX];

Periph::Periph(core_periph_t periph, port_t port) {
	periph_port = (periph << 8) | port;
	fd = lookup_fileno();
}

int Periph::lookup_fileno(){
	int i;
	for(i=0; i < OPEN_MAX; i++){
		if( fd_map[i] == periph_port ){
			return i;
		}
	}
	return -1;
}

void Periph::update_fileno(){
	if( (fd >= 0) && (fd_map[fd] == 0) ){ //fd is no longer valid
		fd = -1; //kill the fileno
	}
}


int Periph::open(const char * name, int flags){
	//check map
	int fileno;

	fileno = lookup_fileno();

	if( fileno < 0 ){
		fd = ::open(name, O_RDWR | flags);
		if( fd > 0 ){
			fd_map[fd] = periph_port;
		} else {
			return -1;
		}
	} else {
		fd = fileno;
	}

	return 0;
}

int Periph::open(int flags){
	char buffer[LINK_NAME_MAX];
	int len;
	const char * name;

	name = periph_name[periph_port>>8];

	strcpy(buffer, "/dev/");
	strcat(buffer, name);
	len = strlen(buffer);
	if( periph_port != 0 ){
		buffer[len] = '0' + (periph_port & 0xFF);
		buffer[len+1] = '\0';
	} else {
		return -1;
	}
	return open(buffer, flags);
}

int Periph::close(){
	int ret = 0;
	update_fileno();
	if( fd >= 0 ){
		ret = ::close(fd);
		fd_map[fd] = 0;
		fd = -1;
	}
	return ret;
}

int Periph::read(void * buf, int nbyte){
	update_fileno();
	return ::read(fd, buf, nbyte);
}

int Periph::write(const void * buf, int nbyte){
	update_fileno();
	return ::write(fd, buf, nbyte);
}

int Periph::read(Aio & aio){
	aio.aio_var.aio_fildes = fd;
	return ::aio_read(&(aio.aio_var));
}

int Periph::write(Aio & aio){
	update_fileno();
	aio.aio_var.aio_fildes = fd;
	return ::aio_write(&(aio.aio_var));
}

int Periph::ioctl(int req, void * arg){
	update_fileno();
	return ::ioctl(fd, req, arg);
}

int Periph::seek(int loc, int whence){
	update_fileno();
	return ::lseek(fd, loc, whence);
}

int Periph::read(int loc, void * buf, int nbyte){
	if( seek(loc) < 0 ){
		return -1;
	}

	return read(buf, nbyte);
}

int Periph::write(int loc, const void * buf, int nbyte){
	if( seek(loc) < 0 ){
		return -1;
	}
	return write(buf, nbyte);
}

int Periph::fileno(void){
	update_fileno();
	return fd;
}



int Periph::readline(char * buf, int nbyte, int timeout, char term){
	int t;
	int bytes_recv;
	char c;
	t = 0;
	bytes_recv = 0;
	do {
		if( read(&c, 1) == 1 ){
			*buf = c;
			buf++;
			bytes_recv++;
			if( c == term ){
				return bytes_recv;
			}
		} else {
			t++;
			Timer::wait_msec(1);
		}
	} while( (bytes_recv < nbyte) && (t < timeout) );

	return bytes_recv;
}
