/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

/*! \mainpage
 *
 * <small>This is the <b>Stratify Application Library</b> documentation.  If you consider yourself an expert, you can check out the
 * <a href="../../StratifyOS/html/">Stratify OS documentation</a>.</small>
 *
 * \section INTRO Introduction
 *
 * Here are some useful namespaces to check out:
 *
 * - Hal - Hardware Abstraction Layer
 * - Calc - Useful software algorithms
 * - Draw - Drawing on an LCD
 * - Fmt - Common file formats
 * - Sys - Access to timers and files and such
 * - Var - Variable storage in data and strings
 *
 * You can also use standard \ref POSIX and \ref STDC functions.
 *
 * Here are a couple of examples to give you an idea about how things work.  These are the objects
 * used in the examples.
 *
 * - Hal::Uart - access to UART hardware
 * - Hal::Pin - read and write pin values
 * - Var::String - string manipulation
 * - Sys::Timer - Timing and delays
 *
 * \code
 * #include <stfy/Hal.hpp> //this will include all namespace classes and add the namespace i.e. "using namespace Hal;"
 * #include <stfy/Sys.hpp>
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
 * #include <stfy/Hal.hpp>
 * #include <stfy/Var.hpp>
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
 * The best way to learn is to get started.
 *
 */



#ifndef STFY_APP_PHY_HPP_
#define STFY_APP_PHY_HPP_

#include <unistd.h>
#include <iface/link.h>
#include <fcntl.h>

namespace Hal {


/*! \brief Base Applib Class for Physical devices
 * \details This is the base class for physical devices.
 *
 */
class Phy {
public:
	Phy();
	enum {
		RDONLY /*! Open as read-only */ = LINK_O_RDONLY,
		READONLY /*! Open as read-only */ = LINK_O_RDONLY,
		WRONLY /*! Open as write-only */ = LINK_O_WRONLY,
		WRITEONLY /*! Open as write-only */ = LINK_O_WRONLY,
		CREATE /*! Create when opening (files) */ = LINK_O_CREAT,
		CREAT /*! Create when opening (files) */ = LINK_O_CREAT,
		TRUNCATE /*! Truncate when opening (files) */ = LINK_O_TRUNC,
		TRUNC /*! Truncate when opening (files) */ = LINK_O_TRUNC,
		APPEND /*! Append when opening (files)*/ = LINK_O_APPEND,
		EXCLUSIVE /*! Create exclusively (files) */ = LINK_O_EXCL,
		EXCL /*! Create exclusively (files) */ = LINK_O_EXCL,
		READWRITE /*! Open as read-write */ = LINK_O_RDWR,
		RDWR /*! Open as read-write */ = LINK_O_RDWR,
		NONBLOCK /*! Open as non-blocking */ = LINK_O_NONBLOCK,
		NDELAY /*! Open as non-blocking */ = LINK_O_NONBLOCK,
		ACCMODE /*! Access mode mask */ = LINK_O_ACCMODE

	};

	/*! \details List of options for \a whence argument of seek() */
	enum {
		SET = SEEK_SET,
		CURRENT = SEEK_CUR,
		END = SEEK_END

	};


#ifdef __link
	void sethandle(link_phy_t h){ handle = h; }
#endif

#ifndef __MCU_ONLY__
	/*! \details Return the file descriptor for the peripheral */
	virtual int ioctl(int req, void * arg) = 0;

	/*! \details ioctl() with request and const arg pointer */
	int ioctl(int req, const void * arg){ return ioctl(req, (void*)arg); }
	/*! \details ioctl() with just a request */
	int ioctl(int req){ return ioctl(req, (void*)NULL); }
	/*! \details ioctl() with request and integer arg */
	int ioctl(int req, int arg){ return ioctl(req, (void*)(size_t)arg); }
#endif

protected:
#ifdef __link
	link_phy_t handle;
#endif



};

};

#endif /* STFY_APP_PHYBASE_HPP_ */
