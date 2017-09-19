/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef HAL_HPP_
#define HAL_HPP_

/*! \brief Hardware Abstraction Layer
 */
namespace hal {};

#include "hal/Dev.hpp"
#include "hal/Adc.hpp"
#include "hal/Dac.hpp"
#include "hal/Core.hpp"
#include "hal/Eint.hpp"
#include "hal/Fifo.hpp"
#include "hal/Mcfifo.hpp"
#include "hal/Led.hpp"
#include "hal/I2C.hpp"
#include "hal/PinAssignment.hpp"
#include "hal/Pin.hpp"
#include "hal/Pwm.hpp"
#include "hal/Rtc.hpp"
#include "hal/Tmr.hpp"
#include "hal/Spi.hpp"
#include "hal/Uart.hpp"
#include "hal/Usb.hpp"

#if !defined __link
#include "hal/Display.hpp"
#include "hal/DisplayDev.hpp"
#include "hal/DevSignal.hpp"
#endif

using namespace hal;

#endif /* HAL_HPP_ */
