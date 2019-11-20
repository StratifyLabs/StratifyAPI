/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_HAL_CORE_HPP_
#define SAPI_HAL_CORE_HPP_

#include <mcu/mcu.h>
#include <sos/dev/core.h>
#include "Periph.hpp"
#include "../var/String.hpp"
#include "../sys/Sys.hpp"

namespace hal {

class CoreFlags {
public:

	enum flags {
		IS_RESET_SOFTWARE = CORE_FLAG_IS_RESET_SOFTWARE /*! Software Reset (default if hardware reset cannot be determined) */,
		IS_RESET_POR = CORE_FLAG_IS_RESET_POR /*! Power on Reset */,
		IS_RESET_EXTERNAL = CORE_FLAG_IS_RESET_EXTERNAL /*! External Reset signal */,
		IS_RESET_WDT = CORE_FLAG_IS_RESET_WDT /*! Watchdog Timer Reset */,
		IS_RESET_BOR = CORE_FLAG_IS_RESET_BOR /*! Brown Out Reset */,
		IS_RESET_SYSTEM = CORE_FLAG_IS_RESET_SYSTEM /*! Software System Reset */,
		SET_CLKOUT = CORE_FLAG_SET_CLKOUT /*! Use the CPU Clock */,
		IS_CLKOUT_CPU = CORE_FLAG_IS_CLKOUT_CPU /*! Use the CPU Clock */,
		IS_CLKOUT_MAIN_OSC = CORE_FLAG_IS_CLKOUT_MAIN_OSC /*! Use the Main Oscillator */,
		IS_CLKOUT_INTERNAL_OSC = CORE_FLAG_IS_CLKOUT_INTERNAL_OSC /*! Use the Internal Oscillator */,
		IS_CLKOUT_USB = CORE_FLAG_IS_CLKOUT_USB /*! Use the USB Clock */,
		IS_CLKOUT_RTC = CORE_FLAG_IS_CLKOUT_RTC /*! Use the RTC Clock */,
		EXEC_SLEEP = CORE_FLAG_EXEC_SLEEP /*! Sleep */,
		EXEC_DEEPSLEEP = CORE_FLAG_EXEC_DEEPSLEEP /*! Deep sleep (preserve SRAM) */,
		EXEC_DEEPSLEEP_STOP = CORE_FLAG_EXEC_DEEPSLEEP_STOP /*! Deep sleep (preserve SRAM, stop clocks) */,
		EXEC_DEEPSLEEP_STANDBY = CORE_FLAG_EXEC_DEEPSLEEP_STANDBY /*! Turn the device off (lose SRAM) */,
		EXEC_RESET = CORE_FLAG_EXEC_RESET /*! Set this flag with I_CORE_SETATTR to reset the device */,
		EXEC_INVOKE_BOOTLOADER = CORE_FLAG_EXEC_INVOKE_BOOTLOADER /*! Set this flag with I_CORE_SETATTR to reset the device and start the bootloader */
	};

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
};

/*!
 * \brief The Core Info class
 */
class CoreInfo : public api::InfoObject, public CoreFlags {
public:
	/*! \details Constructs an empty Core Info object. */
	CoreInfo(){ memset(&m_info, 0, sizeof(m_info)); }

	/*! \details Constructs a Core Info object from a core_info_t data structure. */
	CoreInfo(const core_info_t & info){
		m_info = info;
	}

	/*! \details Returns the events that are supported by the core. */
	u32 o_events() const { return m_info.o_events; }
	/*! \details Returns the flags that are supported by the core. */
	u32 o_flags() const { return m_info.o_flags; }

	bool is_software_reset() const {
		return (o_flags() & (IS_RESET_SOFTWARE|IS_RESET_SYSTEM)) != 0;
	}

	bool is_power_on_reset() const {
		return (o_flags() & IS_RESET_POR) != 0;
	}

	bool is_external_reset() const {
		return (o_flags() & IS_RESET_EXTERNAL) != 0;
	}

	bool is_watchdog_timer_reset() const {
		return (o_flags() & IS_RESET_WDT) != 0;
	}

	bool is_brown_out_reset() const {
		return (o_flags() & IS_RESET_BOR) != 0;
	}

	/*! \details Returns the serial number of the MCU. */
	sys::SerialNumber serial_number() const{ return sys::SerialNumber(m_info.serial); }

private:
	core_info_t m_info;
};

class CoreAttributes : public PeriphAttributes<core_attr_t>, public CoreFlags {
public:

private:
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
class Core :
		public Periph<core_info_t, core_attr_t, CoreAttributes, CORE_IOC_IDENT_CHAR>,
		public CoreFlags {
public:

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
	int set_pin_function(
			const arg::McuPin & pin,
			enum func func,
			const arg::Port & port){
		core_pinfunc_t f;
		f.io.port = pin.argument().port;
		f.io.pin = pin.argument().pin;
		f.periph_func = func;
		f.periph_port = port.argument();
		return set_pin_function(f);
	}

	static CoreInfo get_info();

	/*! \details Loads the MCU board configuration data
	 * provided by the board support package.
	 *
	 * @param config A reference to the destination data
	 * @return Zero on success
	 */
	int get_mcu_board_config(
			mcu_board_config_t & config
			);

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

#endif /* SAPI_HAL_CORE_HPP_ */
