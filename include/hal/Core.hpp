/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved
#ifndef STFY_APP_CORE_HPP_
#define STFY_APP_CORE_HPP_

#include <iface/dev/core.h>
#include "Periph.hpp"


namespace hal {

/*! \brief Core Class
 * \details This is the Core class.  It is used to
 * access core MCU information such as clock speed and
 * to adjust interrupt priorities.
 *
 * Here is an example of how to read the MCU serial number:
 *
 * \code
 * #include <stfy/hal.hpp>
 * #include <cstdio>
 *
 * int main(int argc, char * argv[]){
 * 	int i;
 * 	Core core(0);
 * 	core_attr_t attr;
 * 	core.open();
 * 	core.attr(&attr);
 * 	printf("Serial Number is:");
 * 	for(i=3; i >= 0; i--){
 * 		printf("%X", attr.serial_number[i]);
 * 	}
 * 	printf("\n");
 * 	core.close(); //core is never "powered-down" but this frees the file descriptor
 * }
 * \endcode
 *
 */
class Core : public Periph {
public:

	/*! \details Core functional types */
	enum func {
		CORE /*! \brief Core Functionality */ = CORE_PERIPH_CORE,
		ADC /*! \brief Analog to Digital Converter */ = CORE_PERIPH_ADC,
		DAC /*! \brief Digital to Analog Converter */ = CORE_PERIPH_DAC,
		UART /*! \brief UART */ = CORE_PERIPH_UART,
		SPI /*! \brief SPI */ = CORE_PERIPH_SPI,
		USB /*! \brief USB */ = CORE_PERIPH_USB,
		CAN /*! \brief CAN */ = CORE_PERIPH_CAN,
		ENET /*! \brief ENET */ = CORE_PERIPH_ENET,
		I2C /*! \brief I2C */ = CORE_PERIPH_I2C,
		I2S /*! \brief I2S */ = CORE_PERIPH_I2S,
		MEM /*! \brief External memory interface */ = CORE_PERIPH_MEM,
		RTC /*! \brief RTC */ = CORE_PERIPH_RTC,
		CEC /*! \brief Consumer Electronic Control (Part of HDMI) */ = CORE_PERIPH_CEC,
		QEI /*! \brief Quadrature Encoder Interface */ = CORE_PERIPH_QEI,
		PWM /*! \brief PWM */ = CORE_PERIPH_PWM,
		PIO /*! \brief GPIO */ = CORE_PERIPH_PIO,
		TMR /*! \brief Timer (output compare and input capture) */ = CORE_PERIPH_TMR,
		EINT /*! \brief External interrupts */ = CORE_PERIPH_EINT,
		WDT /*! \brief Watch dog timer */ = CORE_PERIPH_WDT,
		BOD /*! \brief Brown out detection */ = CORE_PERIPH_BOD,
		DMA /*! \brief Direct Memory Access */ = CORE_PERIPH_DMA,
		JTAG /*! \brief JTAG */ = CORE_PERIPH_JTAG,
		RESET /*! \brief Reset */ = CORE_PERIPH_RESET,
		CLKOUT /*! \brief Clockout */ = CORE_PERIPH_CLKOUT,
		LCD /*! \brief LCD */ = CORE_PERIPH_LCD,
		LCD1 /*! \brief LCD */ = CORE_PERIPH_LCD1,
		LCD2 /*! \brief LCD */ = CORE_PERIPH_LCD2,
		LCD3 /*! \brief LCD */ = CORE_PERIPH_LCD3,
		EMC /*! \brief External Memory Controller */ = CORE_PERIPH_EMC,
		MCI /*! \brief Multimedia Card Interface */ = CORE_PERIPH_MCI,
		SSP /*! \brief SSP */ = CORE_PERIPH_SSP,
		MCPWM /*! \brief Motor Control PWM */ = CORE_PERIPH_MCPWM,
		NMI /*! \brief Non-maskable Interrupt */ = CORE_PERIPH_NMI,
		TRACE /*! \brief Trace data */ = CORE_PERIPH_TRACE,
	};

	Core(port_t port);

	/*! \details Get attributes of the Core peripheral and stores them in \a attr.
	 */
	int get_attr(core_attr_t & attr);
	/*! \details Set the attributes for the Core device */
	int set_attr(const core_attr_t & attr);
	/*! \details Execute a request to set the functionality of a pin.
	 */
	int set_pin_function(const core_pinfunc_t * req);

	/*! \details Change pin functionality using a core_pinfunc_t structure */
	inline int set_pin_function(const core_pinfunc_t & req){ return set_pin_function(&req); }

	/*! \details Change pin function.  This can be used
	 * if the default pinassign values are not adequate for your
	 * application.
	 *
	 * @param port The PIO port
	 * @param pin The PIO pin number
	 * @param func The function (e.g. Core::UART)
	 * @param func_port The functional port (e.g. 0 for UART0)
	 * @return Zero on success
	 */
	inline int set_pin_function(int port, int pin, int func, int func_port){
		core_pinfunc_t f;
		f.io.port = port;
		f.io.pin = pin;
		f.periph_func = func;
		f.periph_port = func_port;
		return set_pin_function(&f);
	}


	/*! \details Cause the device to sleep at the specified sleep level.
	 */
	int sleep(core_sleep_t level);
	/*! \details Reset the device. */
	void reset();
	/*! \details Invoke the device's bootloader. If no bootloader is availble, a
	 * normal reset will occur.
	 */
	void invoke_bootloader();


	/*! \details Configure the clock output pin. */
	int set_clkout(const core_clkout_t * clkout);

#ifdef __MCU_ONLY__
	int close();
#endif

private:

};

};

#endif /* STFY_APP_CORE_HPP_ */
