/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_CORE_HPP_
#define SAPI_CORE_HPP_

#include <mcu/mcu.h>
#include <sos/dev/core.h>
#include "Periph.hpp"
#include "../var/String.hpp"

namespace hal {


class SerialNumber {
public:
    SerialNumber();
    int get();

    static SerialNumber from_string(const char * str);


    SerialNumber& operator = (const char * str);

    bool operator == (const SerialNumber & serial_number);

    void print() const;
    var::String to_string() const;

private:
    mcu_sn_t m_serial_number;
};

/*! \brief Core Class
 * \details This is the Core class.  It is used to
 * access core MCU information such as clock speed and
 * to adjust interrupt priorities.
 *
 * Here is an example of how to read the MCU serial number:
 *
 * \code
 * #include <sapi/hal.hpp>
 * #include <cstdio>
 *
 * int main(int argc, char * argv[]){
 * 	int i;
 * 	Core core(0);
 * 	core_attr_t attr;
 * 	core.open();
 * 	core.get_attr(&attr);
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
class Core : public Periph<core_info_t, core_attr_t, 'c'> {
public:

	/*! \details Core functional types */
	enum func {
		CORE /*! Core Functionality */ = CORE_PERIPH_CORE,
		ADC /*! Analog to Digital Converter */ = CORE_PERIPH_ADC,
		DAC /*! Digital to Analog Converter */ = CORE_PERIPH_DAC,
		UART /*! UART */ = CORE_PERIPH_UART,
		SPI /*! SPI */ = CORE_PERIPH_SPI,
		USB /*! USB */ = CORE_PERIPH_USB,
		CAN /*! CAN */ = CORE_PERIPH_CAN,
		ENET /*! ENET */ = CORE_PERIPH_ENET,
		I2C /*! I2C */ = CORE_PERIPH_I2C,
		I2S /*! I2S */ = CORE_PERIPH_I2S,
		MEM /*! External memory interface */ = CORE_PERIPH_MEM,
		RTC /*! RTC */ = CORE_PERIPH_RTC,
		CEC /*! Consumer Electronic Control (Part of HDMI) */ = CORE_PERIPH_CEC,
		QEI /*! Quadrature Encoder Interface */ = CORE_PERIPH_QEI,
		PWM /*! PWM */ = CORE_PERIPH_PWM,
		PIO /*! GPIO */ = CORE_PERIPH_PIO,
		TMR /*! Timer (output compare and input capture) */ = CORE_PERIPH_TMR,
		EINT /*! External interrupts */ = CORE_PERIPH_EINT,
		WDT /*! Watch dog timer */ = CORE_PERIPH_WDT,
		BOD /*! Brown out detection */ = CORE_PERIPH_BOD,
		DMA /*! Direct Memory Access */ = CORE_PERIPH_DMA,
		JTAG /*! JTAG */ = CORE_PERIPH_JTAG,
		RESET /*! Reset */ = CORE_PERIPH_RESET,
		CLKOUT /*! Clockout */ = CORE_PERIPH_CLKOUT,
		LCD /*! LCD */ = CORE_PERIPH_LCD,
		LCD1 /*! LCD */ = CORE_PERIPH_LCD1,
		LCD2 /*! LCD */ = CORE_PERIPH_LCD2,
		LCD3 /*! LCD */ = CORE_PERIPH_LCD3,
		EMC /*! External Memory Controller */ = CORE_PERIPH_EMC,
		MCI /*! Multimedia Card Interface */ = CORE_PERIPH_MCI,
		SSP /*! SSP */ = CORE_PERIPH_SSP,
		MCPWM /*! Motor Control PWM */ = CORE_PERIPH_MCPWM,
		NMI /*! Non-maskable Interrupt */ = CORE_PERIPH_NMI,
		TRACE /*! Trace data */ = CORE_PERIPH_TRACE,
	};

	/*! \details Contructs a new core object using the specified port.
	 *
	 * @param port The core port (zero for all single core devices)
	 */
	Core(port_t port);


	/*! \details Changes pin functionality using a core_pinfunc_t structure. */
	int set_pin_function(const core_pinfunc_t & req);

	/*! \details Changes pin function.
	 *
	 * @param port The PIO port
	 * @param pin The PIO pin number
	 * @param func The function (e.g. Core::UART)
	 * @param func_port The functional port (e.g. 0 for UART0)
	 * @return Zero on success
	 *
	 * This can be used if the default pinassign values are not adequate for your
	 * application.
	 *
	 */
	int set_pin_function(int port, int pin, int func, int func_port){
		core_pinfunc_t f;
		f.io.port = port;
		f.io.pin = pin;
		f.periph_func = func;
		f.periph_port = func_port;
		return set_pin_function(f);
	}


	/*! \details Loads the MCU board configuration data
	 * provided by the board support package.
	 *
	 * @param config A reference to the destination data
	 * @return Zero on success
	 */
	int get_mcu_board_config(mcu_board_config_t & config);

	/*! \details Sets the core CPU clock divider.
	 *
	 * @param value The amount to divide the clock by
	 * @return Zero on success
	 *
	 * This method will change the CPU clock speed (if supported). If
	 * the native clock speed is 120MHz the following code will make
	 * it 60MHz
	 *
	 * \code
	 * Core core(0);
	 * core.open();
	 * core.set_clock_divide(2); //set to 60MHz
	 * core.set_clock_divide(1); //restore to 120MHz
	 * core.close();
	 * \endcode
	 *
	 */
	int set_clock_divide(u32 value);

	/*! \details Resets the device. */
	void reset();

	/*! \details Invokes the device's bootloader. If no bootloader is available, a
	 * normal reset will occur.
	 */
	void invoke_bootloader();

private:

};

}

#endif /* SAPI_CORE_HPP_ */
