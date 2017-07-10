/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_UART_HPP_
#define SAPI_UART_HPP_


#include <sos/dev/uart.h>
#include "Periph.hpp"


namespace hal {

/*! \brief UART Class
 * \details This class implements a serial UART port.
 *
 * Here is an example of how to use the UART.
 *
 * \code
 * #include <stfy/hal.hpp>
 *
 * int main(int argc, char * argv[]){
 * 	Uart uart(0); //use UART0
 * 	char buffer[256];
 * 	uart.init(115200); //initializes using 115200 and default parameters (see init())
 * 	uart.read(buffer, 256); //this will block until at least one byte arrives
 * 	uart.close(); //free the file descriptors and power down the device
 * 	return 0;
 * }
 * \endcode
 *
 * The above example opens using the UART in blocking mode.  If you want to be able
 * to read the UART without blocking until a byte arrives, you can use non-blocking mode.
 *
 * \code
 * #include <stfy/hal.hpp>
 *
 * int main(int argc, char * argv[]){
 * 	Uart uart(0);
 * 	char buffer[256];
 * 	uart.open(Uart::NONBLOCK|Uart::RDWR);
 *  //now set the attributes
 * 	uart.set_attr(115200, //115200 baud rate
 * 	0, //pin assignment 0 (P0.2 and P0.3 on LPC17xx)
 * 	Uart::NONE, // no parity
 * 	Uart::STOP1, //one stop bit
 * 	8 //8 bits per character
 * 	);
 * 	uart.read(buffer, 256); //returns immediately even if no data is available (errno is set to EAGAIN if no data)
 * 	uart.close(); //free the resources
 * }
 * 	\endcode
 *
 */
class Uart : public Periph<uart_info_t, uart_attr_t, 'u'> {
public:
	Uart(port_t port);

	enum {
		FLAG_IS_STOP1 /*! One stop bit */ = UART_FLAG_IS_STOP1,
		FLAG_IS_STOP2 /*! Two stop bits */ = UART_FLAG_IS_STOP2,
		FLAG_IS_STOP0_5 /*! 0.5 stop bits */ = UART_FLAG_IS_STOP0_5,
		FLAG_IS_STOP1_5 /*! 1.5 stop bits */ = UART_FLAG_IS_STOP1_5,
		FLAG_IS_PARITY_NONE /*! Indicates no parity */ = UART_FLAG_IS_PARITY_NONE,
		FLAG_IS_PARITY_ODD /*! Indicates odd parity */ = UART_FLAG_IS_PARITY_ODD,
		FLAG_IS_PARITY_EVEN /*! Indicates even parity */ = UART_FLAG_IS_PARITY_EVEN,
	};

	/*! \details Gets a single byte (if available from the UART).  Upon
	 * success, the byte is written to the value pointed to by \a c.
	 *
	 * \returns Zero on successfully reading a byte, -1 if no bytes are available.
	 */
	int get_byte(char & c);
	int get_byte(char * c){ return get_byte(*c); }
	/*! \details Flush the TX/RX buffers */
	int flush();


	using Periph::set_attr;

	int set_attr(u32 o_flags, mcu_pin_t tx, mcu_pin_t rx, u32 freq, u32 width = 8) const {
		uart_attr_t attr;
		attr.o_flags = o_flags;
		attr.pin_assignment[0] = tx;
		attr.pin_assignment[1] = rx;
		attr.pin_assignment[3] = mcu_invalid_pin();
		attr.pin_assignment[4] = mcu_invalid_pin();
		attr.freq = freq;
		attr.width = width;
		return set_attr(attr);
	}

	int init(u32 o_flags, mcu_pin_t tx, mcu_pin_t rx, u32 freq, u32 width = 8){
		if( open() < 0 ){
			return -1;
		}
		return set_attr(o_flags, tx, rx, freq, width);
	}

private:

};

};

#endif /* SAPI_UART_HPP_ */
