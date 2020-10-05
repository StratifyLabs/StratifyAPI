
set(SOURCELIST
  Adc.cpp
  CFifo.cpp
  Core.cpp
  Dac.cpp
	Device.cpp
	Drive.cpp
	Eint.cpp
	FFifo.cpp
	Fifo.cpp
	I2C.cpp
	I2S.cpp
	Led.cpp
  Periph.cpp
  Pio.cpp
  Pwm.cpp
	Rtc.cpp
	Spi.cpp
	Switchboard.cpp
	StreamFFifo.cpp
	Tmr.cpp
	Uart.cpp
  Usb.cpp
  )

if( ${SOS_BUILD_CONFIG} STREQUAL arm )
  list(APPEND SOURCELIST ${SOURCELIST}
    Display.cpp
    DisplayDevice.cpp
    util_c.c)
endif()

set(SOURCES ${SOURCELIST} PARENT_SCOPE)
