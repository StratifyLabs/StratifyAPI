/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_UART_HPP_
#define SAPI_UART_HPP_


#include <sos/dev/uart.h>
#include "Periph.hpp"


namespace hal {

/*! \brief UART Pin Assignment
 * \details This class allows simple manipulation of the uart_pin_assignment_t.
 */
class UartPinAssignment : public PinAssignment<uart_pin_assignment_t>{};

/*! \brief UART Class
 * \details This class implements a serial UART port.
 *
 * Here is an example of how to use the UART.
 *
 * \code
 * #include <sapi/hal.hpp>
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
 * #include <sapi/hal.hpp>
 *
 * int main(int argc, char * argv[]){
 * 	Uart uart(0);
 * 	char buffer[256];
 * 	uart.open(Uart::NONBLOCK|Uart::RDWR);
 *  //now set the attributes
 * 	uart.set_attr(Uart::FLAG_IS_STOP1|Uart::FLAG_IS_PARITY_NONE|Uart,
 * 	115200, //115200 baud rate
 *  );
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

	/*! \details Reads a single byte (if available from the UART).  Upon
	 * success, the byte is written to the value pointed to by \a c.
	 *
	 * \returns Zero on successfully reading a byte, -1 if no bytes are available.
	 */
	int get(char & c);
	int get_byte(char * c){ return get(*c); }

	/*! \details Writes a single byte on the UART.
	 *
	 * @param c The character to write
	 * @return Zero on success
	 */
	int put(char c);

	/*! \details Flushes the TX/RX buffers */
	int flush();


	using Periph::set_attr;

	/*! \details Sets the attributes of the Uart.
	 *
	 * @param o_flags Bitmask of attribute flags
	 * @param freq Baudrate
	 * @param width Number of bits (usually 8)
	 * @param pin_assignment A pointer to the pin assignment (null to use the default)
	 * @return Zero on success
	 */
	int set_attr(u32 o_flags, u32 freq, u32 width = 8, const uart_pin_assignment_t * pin_assignment = 0) const {
		uart_attr_t attr;
		attr.o_flags = o_flags;
		if( pin_assignment ){
			memcpy(&attr.pin_assignment, pin_assignment, sizeof(uart_pin_assignment_t));
		} else {
			memset(&attr.pin_assignment, 0xff, sizeof(uart_pin_assignment_t));
		}
		attr.freq = freq;
		attr.width = width;
		return set_attr(attr);
	}

	/*! \details Initializes the uart with the specified attributes.
	 *
	 * @param o_flags Bitmask of attribute flags
	 * @param freq Baudrate
	 * @param width Number of bits (usually 8)
	 * @param pin_assignment A pointer to the pin assignment (null to use the default)
	 * @return Zero on success
	 *
	 * This method calls open() and set_attr().
	 */
	int init(u32 o_flags, u32 freq, u32 width = 8, const uart_pin_assignment_t * pin_assignment = 0){
		if( open() < 0 ){
			return -1;
		}
		return set_attr(o_flags, freq, width, pin_assignment);
	}

	using Periph::init;
	using Periph::set_attr;

private:

};

};

#endif /* SAPI_UART_HPP_ */
