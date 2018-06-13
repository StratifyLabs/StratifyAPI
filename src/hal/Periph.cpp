//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

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

u16 PeriphObject::m_fd_map[LINK_OPEN_MAX];

int PeriphObject::lookup_fileno() const {
    int i;
    for(i=0; i < LINK_OPEN_MAX; i++){
        if( m_fd_map[i] == m_periph_port ){
            return i;
        }
    }
    return -1;
}

void PeriphObject::update_fileno() const {
    if( (m_fd >= 0) && (m_fd_map[m_fd] == 0) ){ //fd is no longer valid
        m_fd = -1; //kill the fileno
    }
}


int PeriphObject::open(const char * name, int flags){
    //check map
    int fileno;
    int ret;
    fileno = lookup_fileno();
    if( fileno < 0 ){
        ret = Device::open(name, flags);
        if( m_fd >= 0 ){
            m_fd_map[m_fd] = m_periph_port;
        } else {
            return ret;
        }
    } else {
        m_fd = fileno;
    }

    return 0;
}

int PeriphObject::open(int flags){
    char buffer[LINK_NAME_MAX];
    int len;
    const char * name;
    u8 periph_type = m_periph_port >> 8;

    name = periph_name[periph_type];

    strncpy(buffer, "/dev/", LINK_NAME_MAX-1);
    strncat(buffer, name, LINK_NAME_MAX-1);
    len = strnlen(buffer, LINK_NAME_MAX-1);
    if( periph_type != CORE_PERIPH_RTC ){ //RTC is at /dev/rtc (there can only be one)
        if( m_periph_port != 0 ){
            buffer[len] = '0' + (m_periph_port & 0xFF);
            buffer[len+1] = '\0';
        } else {
            return -1;
        }
    }
    return open(buffer, flags);
}

int PeriphObject::close(){
    int ret = 0;
    update_fileno();
    if( m_fd >= 0 ){
        m_fd_map[m_fd] = 0;
        Device::close();
        m_fd = -1;
    }
    return ret;
}

int PeriphObject::read(void * buf, int nbyte) const {
    update_fileno();
    return File::read(buf, nbyte);
}

int PeriphObject::write(const void * buf, int nbyte) const {
    update_fileno();
    return File::write(buf, nbyte);
}

#ifndef __link
int PeriphObject::read(Aio & aio) const {
    update_fileno();
    return Device::read(aio);
}

int PeriphObject::write(Aio & aio) const {
    update_fileno();
    return Device::write(aio);
}
#endif

int PeriphObject::ioctl(int req, void * arg) const {
    update_fileno();
    return Device::ioctl(req, arg);
}

int PeriphObject::seek(int loc, int whence) const {
    update_fileno();
    return Device::seek(loc, whence);
}

int PeriphObject::fileno() const {
    update_fileno();
    return Device::fileno();
}


