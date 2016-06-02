/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#ifndef AIO_HPP_
#define AIO_HPP_

#ifndef __link

#include <errno.h>
#include <aio.h>

namespace sys {

/*! \brief Asynchronous IO Class */
/*! \details The Asynchronous IO class is used for performing asynchronous operations on
 * hardware devices.  When calling synchronous IO, the read/write function is called and then
 * returns when the operation is complete.  With Asynchronous IO, the read/write function returns
 * immediately and the Aio class can be used to see when the operation completes.
 *
 * On Stratify OS, synchronous operations will always cause the running thread to yield the processor.
 * This can be avoided by using the Aio class.  Consider the following example:
 *
 * \code
 * #include <stfy/hal.hpp>
 * #include <stfy/Sys.hpp>
 * #include <stfy/Var.hpp>
 *
 * int main(int argc, char * argv[]){
 * 	String buf = "Hello World!\n";
 * 	Uart uart(0); //we will read/write data to UART port 0
 * 	Aio aio(buf.data(), buf.size()); //aio uses buf as it's data
 *
 * 	uart.init(2400); //init the UART with baudrate 2400
 *
 *  //First a synchronous write
 * 	uart.write(buf.c_str(), buf.size());  //this will return when the bytes are written
 *  //While bytes are writing, the processor will be used on another thread
 *
 *  //Now the AIO version
 * 	uart.write(aio);
 *  //This thread keeps the processor -- so we can do something else in parallel
 *
 * 	while( aio.done() == false ){
 *  	Timer::wait_msec(5); //wait for the operation to complete
 * 	}
 *
 * 	return 0;
 * }
 * \endcode
 *
 *
 */
class Aio {
public:

#ifdef __MCU_ONLY__
	Aio(volatile void * buf, int nbytes, int offset = 0){
		aio_var.buf = (void*)buf;
		aio_var.nbyte = nbytes;
		aio_var.loc = offset;
	}


	/*! \brief Return the buffer pointer */
	volatile void * buf() const { return aio_var.buf; }
	/*! \brief Set the buffer pointer */
	void set_buf(volatile void * buf){ aio_var.buf = (void*)buf; }
	/*! \brief Return the number of bytes to transfer */
	int nbytes() const { return aio_var.nbyte; }
	/*! \brief Set the number of bytes to transfer */
	void set_nbytes(int nbytes){ aio_var.nbyte = nbytes; }
	/*! \brief Return the offset (or channel for Dac, Adc, Pwm, etc) */
	int offset() const { return aio_var.loc; }
	/*! \brief Set the offset (or channel for Dac, Adc, Pwm, etc) */
	void set_offset(int offset){ aio_var.loc = offset; }
	/*! \brief Return the return value of the operation */
	int ret() const { return aio_var.nbyte; }
	/*! \brief Return the error number of the operation */
	int error() const { return aio_var.nbyte; }
	/*! \brief Check to see if operation is complete */
	bool done() const {
		if(  aio_var.flags & BUSY_FLAG ){
			return false;
		} else {
			return true;
		}
	}
#else
	/*! \details Declare an Aio object. */
	Aio(volatile void * buf, int nbytes, int offset = 0){
		aio_var.aio_buf = buf;
		aio_var.aio_nbytes = nbytes;
		aio_var.aio_offset = offset;
		aio_var.aio_sigevent.sigev_notify = SIGEV_NONE;
	}

	/*! \details Block until all transfers in list have completed or timeout is reached */
	static int suspend(struct aiocb * const list[], int nent, int timeout_usec = 0){
		struct timespec timeout;
		timeout.tv_sec = timeout_usec / 1000000;
		timeout.tv_nsec = (timeout_usec % 1000000) * 1000;
		if( timeout_usec == 0 ){
			return aio_suspend(list, nent, 0);
		} else {
			return aio_suspend(list, nent, &timeout);
		}
	}

	/*! \details Block until the operation completes or timeout is reached */
	int suspend(int timeout_usec = 0){
		struct aiocb * const list[1] = { &aio_var };
		return suspend(list, 1, timeout_usec);
	}

	/*! \details Return the buffer pointer */
	volatile void * buf() const { return aio_var.aio_buf; }
	/*! \details Set the buffer pointer */
	void set_buf(volatile void * buf){ aio_var.aio_buf = buf; }
	/*! \details Return the number of bytes to transfer */
	int nbytes() const { return aio_var.aio_nbytes; }
	/*! \details Set the number of bytes to transfer */
	void set_nbytes(int nbytes){ aio_var.aio_nbytes = nbytes; }
	/*! \details Return the offset (or channel for Dac, Adc, Pwm, etc) */
	int offset() const { return aio_var.aio_offset; }
	/*! \details Set the offset (or channcel for Dac, Adc, Pwm, etc) */
	void set_offset(int offset){ aio_var.aio_offset = offset; }
	/*! \details Return the return value of the operation */
	int ret(){ return aio_return(&aio_var); }
	/*! \details Return the error number of the operation */
	int error(){ return aio_error(&aio_var); }
	/*! \details Check to see if operation is complete */
	bool done() const {
		if( aio_error(&aio_var) == EINPROGRESS ){
			return false;
		} else {
			return true;
		}
	}

	/*! \details This method will cause the thread to receive a signal when the operation
	 * completes.
	 *
	 * @param number The signal number (ie SIGTERM) use -1 to prevent a signal from being sent
	 * @param value The signal value (passed as an argument to the handler if using siginfo)
	 * @return Zero on success
	 */
	void signal(int number, int value){
		if( number >= 0 ){
			aio_var.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
			aio_var.aio_sigevent.sigev_signo = number;
			aio_var.aio_sigevent.sigev_value.sival_int = value;
		} else {
			aio_var.aio_sigevent.sigev_notify = SIGEV_NONE;
		}
	}

#endif

	/*! \details Check to see if operation is still in progress */
	inline bool inprogress() const { return !done(); }


	struct aiocb aio_var;

private:
#ifdef __MCU_ONLY__
	friend class Usb;
	friend class Spi;
	device_transfer_t aio_var;
	enum {
		BUSY_FLAG = (1<<16)
	};
#else
#endif
};

};

#endif


#endif /* AIO_HPP_ */
