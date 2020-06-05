/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_HAL_UART_HPP_
#define SAPI_HAL_UART_HPP_


#include <sos/dev/uart.h>
#include "PinAssignment.hpp"
#include "Periph.hpp"


namespace hal {

class UartFlags {
public:
	enum flags {
		flag_set_line_coding_default /*! See \ref UART_FLAG_SET_LINE_CODING_DEFAULT */ = UART_FLAG_SET_LINE_CODING_DEFAULT,
		flag_set_line_coding /*! See \ref UART_FLAG_SET_LINE_CODING */ = UART_FLAG_SET_LINE_CODING,
		flag_is_stop1 /*! See \ref UART_FLAG_IS_STOP1 */ = UART_FLAG_IS_STOP1,
		flag_is_stop2 /*! See \ref UART_FLAG_IS_STOP2 */ = UART_FLAG_IS_STOP2,
		flag_is_stop0_5 /*! See \ref UART_FLAG_IS_STOP0_5 */ = UART_FLAG_IS_STOP0_5,
		flag_is_stop1_5 /*! See \ref UART_FLAG_IS_STOP1_5 */ = UART_FLAG_IS_STOP1_5,
		flag_is_parity_none /*! See \ref UART_FLAG_IS_PARITY_NONE */ = UART_FLAG_IS_PARITY_NONE,
		flag_is_parity_odd /*! See \ref UART_FLAG_IS_PARITY_ODD */ = UART_FLAG_IS_PARITY_ODD,
		flag_is_parity_even /*! See \ref UART_FLAG_IS_PARITY_EVEN */ = UART_FLAG_IS_PARITY_EVEN,
	};
};

API_OR_FLAGS_OPERATOR(UartFlags)


/*! \brief UART Pin Assignment
 * \details This class allows simple manipulation of the uart_pin_assignment_t.
 *
 * \code
 * UartPinAssignment pin_assignment;
 * pin_assignment->tx = mcu_pin(0,1);
 * pin_assignment->rx = mcu_pin(0,1);
 *
 * Uart uart(0);
 * uart.set_attr(Uart::FLAG_SET_LINE_CODING, 115200, 8, pin_assignment);
 * \endcode
 *
 * \sa hal::Uart
 */
class UartPinAssignment : public PinAssignment<uart_pin_assignment_t>{};


/*! \brief Uart Attributes Class
 * \details The Uart Attributes Class can be used to configure
 * the UART. The sys::Cli classes can populate a UartAttr object
 * using arguments passed on the command line.
 *
 * For example, the following sample code will initialize
 * a UART port based on parameters passed on the command line.
 *
 * \code
 * int main(int argc, char * argv[]){
 * 	UartAttr uart_attr;
 * 	Cli cli(argc, argv);
 *
 * 	cli.handle_uart(uart_attr);
 *
 * 	Uart uart(uart_attr.port());
 * 	uart.init(uart_attr);
 * }
 * \endcode
 *
 * \sa hal::Uart
 *
 */
class UartAttributes : public PinAssignmentPeriphAttributes<uart_attr_t, uart_pin_assignment_t>,
		public UartFlags {
public:

	/*! \details Constructs UART attributes with default settings.
	 *
	 * @param o_flags Flags for attibutes (default is UART_FLAG_SET_LINE_CODING_DEFAULT)
	 * @param freq UART frequency (bitrate; default is 115200)
	 * @param width UART byte width (default is 8)
	 *
	 */
	UartAttributes(){
		set_flags(flag_set_line_coding_default);
		set_freq(115200);
		set_width(8);
	}

	API_ACCESS_MEMBER_COMPOUND(UartAttributes,mcu_pin_t,attr.pin_assignment,tx)
	API_ACCESS_MEMBER_COMPOUND(UartAttributes,mcu_pin_t,attr.pin_assignment,rx)
	API_ACCESS_MEMBER_COMPOUND(UartAttributes,mcu_pin_t,attr.pin_assignment,cts)
	API_ACCESS_MEMBER_COMPOUND(UartAttributes,mcu_pin_t,attr.pin_assignment,rts)
	API_ACCESS_MEMBER_FUNDAMENTAL(UartAttributes,u32,attr,width)
	API_ACCESS_MEMBER_FUNDAMENTAL_WITH_ALIAS(UartAttributes,u32,attr,frequency,freq)

	UartAttributes & set_flags(u32 value){
		PeriphAttributes::set_flags(value); return *this;
	}

};

typedef UartAttributes UartAttr;

class UartInfo : public api::InfoObject {
public:
	UartInfo(){ memset(&m_info, 0, sizeof(m_info)); }
	UartInfo(const uart_info_t & info){ m_info = info; }

	bool is_valid() const {
		return m_info.o_flags != 0;
	}

	bool is_rx_fifo() const {
		return m_info.o_flags & UART_FLAG_IS_RX_FIFO;
	}

	u32 size() const { return m_info.size; }
	u32 size_ready() const { return m_info.size_ready; }


private:
	uart_info_t m_info;
};


/*! \brief UART Class
 * \details This class implements a serial UART port.
 *
 * Here is an example of how to use the UART using
 * default parameters provide by the board support package.
 *
 * \code
 * #include <sapi/hal.hpp>
 *
 * int main(int argc, char * argv[]){
 * 	Uart uart(0); //use UART0
 * 	char buffer[256];
 * 	uart.init(); //initializes using default parameters
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
 * 	UartPinAssignment pin_assignment;
 * 	uart.open(Uart::NONBLOCK|Uart::RDWR);
 *  	//now set the attributes
 * 	pin_assignment->tx = mcu_pin(0,0);
 *  	pin_assignment->rx = mcu_pin(0,1);
 * 	uart.set_attr(Uart::FLAG_IS_STOP1|Uart::FLAG_IS_PARITY_NONE,
 * 	115200, //115200 baud rate
 *  	8,
 *  	pin_assignment); //this value can be null to use the BSP's default pin assignment values
 * 	uart.read(buffer, 256); //returns immediately even if no data is available (errno is set to EAGAIN if no data)
 * 	uart.close(); //free the resources
 * }
 * 	\endcode
 *
 * 	The UART can also be configured directly using the POSIX API.
 *
 * 	\code
 * 	#include <sos/dev/uart.h>
 * 	#include <unistd.h>
 * 	#include <fcntl.h>
 *
 * 	int main(int argc, char * argv[]){
 * 		int fd;
 * 		uart_attr_t attr;
 * 		char str[] = "Hello!\n";
 * 		fd = open("/dev/uart0", O_RDWR);
 * 		if( fd < 0 ){
 * 			perror("Failed to open uart");
 * 		} else {
 *				attr.o_flags = UART_FLAG_SET_LINE_CODING | UART_FLAG_IS_PARITY_NONE;
 *				attr.width = 8;
 *				attr.freq = 115200;
 *				attr.pin_assignment.tx = mcu_pin(0,0);
 *				attr.pin_assignment.rx = mcu_pin(0,1);
 *				attr.pin_assignment.cts = mcu_pin(0xff,0xff); //don't use CTS
 *				attr.pin_assignment.rts = mcu_pin(0xff,0xff); //don't use RTS
 *				if( ioctl(fd, I_UART_SETATTR, &attr) < 0 ){
 *					perror("Failed to set uart attr");
 *				} else {
 *					write(fd, str, strlen(str));
 *				}
 *
 *				close(fd);
 * 		}
 *
 * 		return 0;
 * 	}
 * 	\endcode
 *
 * \sa hal::UartPinAssignment
 * \sa hal::UartAttr
 */
class Uart : public Periph<uart_info_t, uart_attr_t, UartAttributes, UART_IOC_IDENT_CHAR>,
		public UartFlags{
public:

	/*! \details Constructs a new Uart object.
	 *
	 * @param port The port to use (Zero is always the first port)
	 */
	Uart(port_t port);

	/*! \details Reads a single byte (if available from the UART).  Upon
	 * success, the byte is written to the value pointed to by \a c.
	 *
	 * \returns Zero on successfully reading a byte, -1 if no bytes are available.
	 */
	int get(char & c);

	/*! \details Writes a single byte on the UART.
	 *
	 * @param c The character to write
	 * @return Zero on success
	 */
	int put(char c);

	/*! \details Flushes the TX/RX buffers. */
	int flush();


	using Periph::get_info;
	UartInfo get_info() const;


private:

};

}

#endif /* SAPI_HAL_UART_HPP_ */
