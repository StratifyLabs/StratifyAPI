/*
 * Fifo.hpp
 *
 *  Created on: May 2, 2014
 *      Author: tgil
 */

#ifndef FIFO_HPP_
#define FIFO_HPP_

#include <sos/dev/fifo.h>
#include "Dev.hpp"

namespace hal {

class Fifo;

/*! \brief Fifo Attributes Class
 *
 */
class FifoAttr {
	friend class Fifo;
public:
	FifoAttr(){ memset(&m_attr, 0, sizeof(m_attr)); }
	/*! \details The number of bytes in the FIFO that are currently used (ie available
	 * for reading.
	 *
	 * @return The number of bytes available for reading.
	 */
	u32 used() const { return m_attr.used; }
	/*! \details This method accesses the maximum number of bytes allocated for the FIFO.
	 *
	 */
	u32 size() const { return m_attr.size; }
	/*! \details This method accesses whether or not the FIFO has overflowed
	 * since the last time the FIFO attributes have been read.
	 */
	bool is_overflow() const { return m_attr.overflow != 0; }
	bool overflow() const { return m_attr.overflow != 0; }
private:
	fifo_attr_t m_attr;
};

/*! \brief FIFO Class */
/*! \details The Fifo class connects to Fifo devices which can be used to:
 *
 * - Inter-process communication
 * - Buffering serial I/O from SPI, I2S, USB, etc
 *
 * For example, the default STDIO is implemented as two fifo buffers: one for input
 * and one for output. A typical application will write the STDOUT fifo while
 * the USB protocol will read it fifo and display it on the screen. Alternatively,
 * a separate application or the kernel could read the STDOUT and print it on a display
 * or send it out the UART.
 *
 * Fifo's can be opened in blocking and non-blocking modes when both reading and writing.
 * If Fifo::set_writeblock() is called, a write will block when the Fifo is full. Otherwise,
 * the Fifo will overflow and FifoAttr::is_overflow() will return true.
 *
 *
 * \code
 * #include <cstdio>
 * #include <stfy/hal.hpp>
 *
 * Fifo fifo;
 * FifoAttr attr;
 *
 * if( fifo.open("/dev/fifo", Fifo::RDWR) < 0 ){
 * 	perror("failed to open fifo");
 * }
 *
 * if( fifo.attr().used() > 0){
 * 	//fifo is not empty
 * }
 *
 *	//write some data to the fifo
 * fifo.write("Testing fifo write\n");
 *
 * fifo.close();
 * \endcode
 *
 */
class Fifo : public hal::Dev {
public:
	Fifo();

	/*! \details Read the Fifo Attributes */
	int get_info(fifo_attr_t & attr);
	int get_info(fifo_attr_t * attr){
		return get_info(*attr);
	}

	/*! \details Return the fifo attributes */
	const FifoAttr get_info(){
		FifoAttr a;
		get_info(&a.m_attr);
		return a;
	};

	/*! \details Flush the FIFO */
	int flush();
	/*! \details Initialize the FIFO. This function should only be called once
	 * even if several contexts access the FIFO.
	 *
	 *  @return Less than zero for an error.
	 *
	 */
	int init();
	/*! \details Exit the FIFO */
	int exit();
	/*! \details Set the FIFO write block */
	int set_writeblock();

};

};

#endif /* FIFO_HPP_ */
