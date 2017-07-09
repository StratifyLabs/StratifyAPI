/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

/*! \mainpage
 *
 * <small>This is the <b>Stratify Application Library</b> documentation.  If you consider yourself an expert, you can check out the
 * <a href="../../StratifyOS/html/">Stratify OS documentation</a>.</small>
 *
 * \section INTRO Introduction
 *
 * Stratify Lib is a library for Stratify OS applications that makes it easy to access hardware
 * and other system functions (you can also use standard \ref POSIX and \ref STDC functions if that
 * is your style).
 *
 * If you are new, it would be highly beneficial to quickly read through the naming conventions:
 * https://github.com/StratifyLabs/StratifyLib#naming-conventions.
 *
 * Here are some useful namespaces to check out:
 *
 * - \ref hal - Hardware Abstraction Layer
 * - \ref calc - Useful software algorithms
 * - \ref draw - Drawing on an LCD
 * - \ref fmt - Common file formats
 * - \ref sys - Access to timers and files and such
 * - \ref var - Variable storage in data and strings
 *
 *
 * Here are a couple of examples to give you an idea about how things work.  These are the objects
 * used in the examples.
 *
 * - hal::Uart - access to UART hardware
 * - hal::Pin - read and write pin values
 * - var::String - string manipulation
 * - sys::Timer - Timing and delays
 *
 * \code
 * #include <stfy/hal.hpp> //this will include all namespace classes and add the namespace i.e. "using namespace hal;"
 * #include <stfy/sys.hpp>
 *
 * int main(int argc, char * argv[]){
 *  Pin p(2, 10);
 *  Pin button(2,9);
 *  p.init(Pin::OUTPUT); //open the port and configure pin 10 as an output
 *  button.init(Pin::INPUT | Pin::PULLUP); //pin 9 is an input (pulled high)
 *
 *  //toggle p until the button is pressed
 *  while( button.value() != 0 ){
 *  	p = false;
 *  	Timer::wait_msec(100);
 *  	p = true;
 *  	Timer::wait_msec(100);
 *  }
 *
 * 	return 0;
 * }
 * \endcode
 *
 *
 * Here is an example of a UART loop back routine.
 *
 *
 * \code
 * #include <stfy/hal.hpp>
 * #include <stfy/var.hpp>
 *
 * int main(int argc, char * argv[]){
 *  Uart uart(0);
 *	int ret;
 *	String exitCommand = "exit\n";
 *	bool done = false;
 *  char buffer[64];
 *  uart.init(115200);
 *
 *  //loopback UART until exit is entered
 *  while( done == false ){
 *  	if( (ret = uart.read(buffer,64)) > 0 ){
 *  		if( exitCommand == buffer ){
 *  			done = true;
 *  		}
 *			uart.write(buffer, ret);
 *  	}
 *  }
 *
 * 	return 0;
 * }
 * \endcode
 *
 * The best way to learn is to get started:  https://github.com/StratifyLabs/HelloWorld#helloworld.
 *
 */



#ifndef SAPI_PHY_HPP_
#define SAPI_PHY_HPP_

#include "../sys/File.hpp"

namespace hal {


/*! \brief Base Applib Class for physical devices
 * \details This is a device class used for accessing physical devices or files.
 *
 * - MCU peripheral hardware (most devices have a class that inherits this class)
 * - Devices with built in drivers (see "/dev" folder on the device)
 * - Regular files (sys::File inherits this class)
 *
 */
class Dev : public sys::File {
public:
	Dev();

#ifndef __link
	/*! \details Reads the device asynchronously.
	 *
	 * \sa Aio
	 */
	virtual int read(sys::Aio & aio) const;

	/*! \details Writes the device asynchronously.
	 *
	 * \sa Aio
	 */	virtual int write(sys::Aio & aio) const;

	using File::read;
	using File::write;
#endif

protected:




};

};

#endif /* SAPI_PHYBASE_HPP_ */
