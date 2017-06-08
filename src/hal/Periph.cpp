//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "hal/Periph.hpp"
#include "sys/Timer.hpp"

using namespace hal;
using namespace sys;


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

u16 Periph::m_fd_map[LINK_OPEN_MAX];

Periph::Periph(core_periph_t periph, port_t port) {
	m_periph_port = (periph << 8) | port;
	m_fd = lookup_fileno();
}

int Periph::lookup_fileno() const {
	int i;
	for(i=0; i < LINK_OPEN_MAX; i++){
		if( m_fd_map[i] == m_periph_port ){
			return i;
		}
	}
	return -1;
}

void Periph::update_fileno() const {
	if( (m_fd >= 0) && (m_fd_map[m_fd] == 0) ){ //fd is no longer valid
		m_fd = -1; //kill the fileno
	}
}


int Periph::open(const char * name, int flags){
	//check map
	int fileno;
	fileno = lookup_fileno();
	if( fileno < 0 ){
		Dev::open(name, flags);
		if( m_fd > 0 ){
			m_fd_map[m_fd] = m_periph_port;
		} else {
			return -1;
		}
	} else {
		m_fd = fileno;
	}

	return 0;
}

int Periph::open(int flags){
	char buffer[LINK_NAME_MAX];
	int len;
	const char * name;

	name = periph_name[m_periph_port>>8];

	strcpy(buffer, "/dev/");
	strcat(buffer, name);
	len = strlen(buffer);
	if( m_periph_port != 0 ){
		buffer[len] = '0' + (m_periph_port & 0xFF);
		buffer[len+1] = '\0';
	} else {
		return -1;
	}
	return open(buffer, flags);
}

int Periph::close(){
	int ret = 0;
	update_fileno();
	if( m_fd >= 0 ){
		m_fd_map[m_fd] = 0;
		Dev::close();
		m_fd = -1;
	}
	return ret;
}

int Periph::read(void * buf, int nbyte) const {
	update_fileno();
	return Dev::read(buf, nbyte);
}

int Periph::write(const void * buf, int nbyte) const {
	update_fileno();
	return Dev::write(buf, nbyte);
}

#ifndef __link
int Periph::read(Aio & aio) const {
	update_fileno();
	return Dev::read(aio);
}

int Periph::write(Aio & aio) const {
	update_fileno();
	return Dev::write(aio);
}
#endif

int Periph::ioctl(int req, void * arg) const {
	update_fileno();
	return Dev::ioctl(req, arg);
}

int Periph::seek(int loc, int whence) const {
	update_fileno();
	return Dev::seek(loc, whence);
}

int Periph::fileno() const {
	update_fileno();
	return Dev::fileno();
}


