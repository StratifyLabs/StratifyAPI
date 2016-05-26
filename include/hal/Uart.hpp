/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef STFY_APP_UART_HPP_
#define STFY_APP_UART_HPP_


#include <hal/Periph.hpp>
#include <iface/dev/uart.h>


namespace hal {

/*! \brief UART Class
 * \details This class implements a serial UART port.
 *
 * Here is an example of how to use the UART.
 *
 * \code
 * #include <stfy/Hal.hpp>
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
 * #include <stfy/Hal.hpp>
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
class Uart : public Periph {
public:
	Uart(port_t port);

	/*! \details Get UART attributes in \a attr */
	int get_attr(uart_attr_t * attr);
	/*! \details Set UART attributes to \a attr */
	int set_attr(const uart_attr_t * attr);
	/*! \details This method gets a single byte (if available from the UART).  Upon
	 * success, the byte is written to the value pointed to by \a c.
	 * \returns Zero on successfully reading a byte, -1 if no bytes are available.
	 */
	int get_byte(char * c);
	/*! \details Flush the TX/RX buffers */
	int flush();

	using Periph::write;

#ifdef __MCU_ONLY__
	void setfifo(const device_cfg_t * cfg){ fifocfg[port_] = cfg; }
	int read(void * buf, int nbyte);
	int write(const void * buf, int nbyte);
	int close();
#endif


	/*! \brief UART Parity values */
	/*! \details This is a list of the UART parity values */
	enum {
		NONE /*! No Parity */ = UART_PARITY_NONE,
		EVEN /*! Even Parity */ = UART_PARITY_EVEN,
		ODD /*! Odd Parity */ = UART_PARITY_ODD
	};

	/*! \brief UART Stop bits */
	/*! \details This is a list of the UART stop bit values */
	enum {
		STOP1 /*! One stop bit */ = UART_ATTR_STOP_BITS_1,
		STOP2 /*! Two stop bits */ = UART_ATTR_STOP_BITS_2
	};

	/*! \brief Set the attributes as specified */
	int set_attr(int baudrate /*! Baudrate */ = 19200,
			int pin_assign /*! Pin assignment */ = 0,
			int parity /*! Parity */ = UART_PARITY_NONE,
			int stop /*! Number of stop bits */ = UART_ATTR_STOP_BITS_2,
			int width /*! bits per character */ = 8){
		uart_attr_t attr;
		attr.baudrate = baudrate;
		attr.pin_assign = pin_assign;
		attr.parity = parity;
		attr.stop = stop;
		attr.width = width;
		attr.start = UART_ATTR_START_BITS_1;
		return set_attr(&attr);
	}

	/*! \details Initialize the UART to
	 * - 19200 baudrate
	 * - default pin assignment
	 * - No parity
	 * - 8 bit width
	 * - 2 stop bits
	 * \return Zero on success.
	 */
	int init(int baudrate = 19200,
			int pin_assign = 0,
			uart_parity_t parity = UART_PARITY_NONE,
			int stop = UART_ATTR_STOP_BITS_2,
			int width = 8){
		if( open() < 0 ){
			return -1;
		}
		return set_attr(baudrate, pin_assign, parity, stop, width);
	}

private:

};

};

#endif /* STFY_APP_UART_HPP_ */
