#ifndef APIOBJECT_HPP
#define APIOBJECT_HPP

/*! \mainpage
 *
 * <small>This is the <b>Stratify Application Programming Interface (API)</b> documentation.  This documentation is for building applications
 * for Stratify OS. If you are creating board support packages, you will want to take a look at the
 * <a href="../../StratifyOS/html/">Stratify OS documentation</a>.</small>
 *
 * \section Introduction
 *
 * StratifyAPI is a library for Stratify OS applications that makes it easy to access hardware
 * and other system functions (you can also use standard \ref POSIX and \ref STDC functions if that
 * is your style).
 *
 * If you are new, it would be highly beneficial to quickly read through the naming conventions:
 * https://github.com/StratifyLabs/StratifyLib#naming-conventions. Once you are familiar with those,
 * you can check out these useful namespaces to get started.
 *
 * - \ref hal - Hardware Abstraction Layer
 * - \ref calc - Useful software algorithms
 * - \ref draw - Drawing on an LCD
 * - \ref fmt - Common file formats
 * - \ref sys - Access to timers and files and such
 * - \ref var - Variable storage in data and strings
 *
 * The following examples will give you an idea about how things work. These are the objects
 * used in the examples.
 *
 * - hal::Uart - access to UART hardware
 * - hal::Pin - read and write pin values
 * - var::String - string manipulation
 * - sys::Timer - Timing and delays
 *
 * \code
 * #include <sapi/hal.hpp> //this will include all namespace classes and add the namespace i.e. "using namespace hal;"
 * #include <sapi/sys.hpp> //includes all sys classes plus "using namespace sys;"
 * #include <sapi/chrono.hpp> //includes all sys classes plus "using namespace sys;"
 *
 * int main(int argc, char * argv[]){
 *  Pin p(2, 10);
 *  Pin button(2,9);
 *  p.init(Pin::FLAG_SET_OUTPUT); //open the port and configure pin 10 as an output
 *  button.init(Pin::FLAG_SET_INPUT | Pin::FLAG_IS_PULLUP); //pin 9 is an input (pulled high)
 *
 *  //toggle p until the button is pressed
 *  while( button.get_value() != 0 ){
 *   p = false;
 *   ClockTime::wait_milliseconds(100);
 *   p = true;
 *   ClockTime::wait_milliseconds(100);
 *  }
 *
 * 	return 0;
 * }
 * \endcode
 *
 *
 * Here is an example of a UART loop back routine.
 *
 * \code
 * #include <sapi/hal.hpp>
 * #include <sapi/var.hpp>
 *
 * int main(int argc, char * argv[]){
 *  Uart uart(0);
 *	int ret;
 *	String exitCommand = "exit\n";
 *	bool done = false;
 *  char buffer[64];
 *  uart.init(); //initializes the UART with default settings
 *
 *  //loopback UART until exit is entered
 *  while( done == false ){
 *  	if( (ret = uart.read(buffer,64)) > 0 ){
 *  		if( exitCommand == buffer ){
 *  			done = true;
 *  		}
 *			uart.write(buffer, ret);
 *  	 }
 *  }
 *
 * 	return 0;
 * }
 * \endcode
 *
 * The best way to learn is to get started:  https://github.com/StratifyLabs/HelloWorld#helloworld.
 *
 */

#include <mcu/types.h>

namespace api {

/*! \brief API Object Class
 * \details The API Object class is the parent of all
 * other classes. The API namespace contains
 * two other classes that inherit this object
 *
 * - InfoObject
 * - WorkObject
 *
 * InfoObject is for classes that are used to access static
 * data and don't do any work or have the potential to have errors.
 * InfoObjects also include classes that consist purely of static
 * methods and members.
 *
 * WorkObject has an interface to setting the error and issuing
 * a fatal exit if a fatal error occurs.
 *
 * All object in the API inherit either InfoObject or WorkObject.
 *
 */
class ApiObject {
public:
    /*! \details Returns a pointer to a string
     * that shows the API version.
     */
    static const char * api_version(){ return "3.7.0"; }
};

}

#endif // APIOBJECT_HPP
