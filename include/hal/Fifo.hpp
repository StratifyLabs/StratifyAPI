/*
 * Fifo.hpp
 *
 *  Created on: May 2, 2014
 *      Author: tgil
 */

#ifndef FIFO_HPP_
#define FIFO_HPP_

#include <cstring>
#include <sos/dev/fifo.h>
#include "Dev.hpp"

namespace hal {

class Fifo;

/*! \brief Fifo Attributes Class
 *
 */
class FifoInfo {
	friend class Fifo;
public:
	FifoInfo(){ memset(&m_info, 0, sizeof(m_info)); }
	/*! \details The number of bytes in the FIFO that are currently used (ie available
	 * for reading.
	 *
	 * @return The number of bytes available for reading.
	 */
	u32 used() const { return m_info.used; }
	/*! \details This method accesses the maximum number of bytes allocated for the FIFO.
	 *
	 */
	u32 size() const { return m_info.size; }
	/*! \details This method accesses whether or not the FIFO has overflowed
	 * since the last time the FIFO attributes have been read.
	 */
	bool is_overflowidth() const { return m_info.overflow != 0; }
	bool overflowidth() const { return m_info.overflow != 0; }
private:
	fifo_info_t m_info;
};

/*! \brief FIFO Class */
/*! \details The Fifo class connects to Fifo devices which can be used for:
 *
 * - Inter-process communication
 * - Buffering serial I/O from SPI, I2S, USB, etc
 *
 * For example, the default STDIO is implemented as two fifo buffers: one for input
 * and one for output. A typical application will write the STDOUT fifo while
 * the USB protocol will read the fifo and display it on the screen. Alternatively,
 * a separate application or the kernel could read the STDOUT and print it on a display
 * or send it out the UART.
 *
 * Fifo's can be opened in blocking and non-blocking modes when both reading and writing.
 * If Fifo::set_writeblock() is called, a write will block when the Fifo is full. Otherwise,
 * the Fifo will overflow and FifoInfo::is_overflowidth() will return true.
 *
 *
 * \code
 * #include <cstdio>
 * #include <sapi/hal.hpp>
 *
 * Fifo fifo;
 * FifoInfo attr;
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
	int get_info(fifo_info_t & info) const;

	/*! \details Return the fifo attributes */
	const FifoInfo get_info() const {
		FifoInfo a;
		get_info(a.m_info);
		return a;
	};

	/*! \details Flushes the FIFO. */
	int flusheight() const;

	/*! \details Initialize the FIFO. This function should only be called once
	 * even if several contexts access the FIFO.
	 *
	 *  @return Less than zero for an error.
	 *
	 */
	int init() const;

	/*! \details Exit the FIFO */
	int exit() const;

	/*! \details Sets the FIFO in write block mode.
	 *
	 * If a write is attempted (and the file descriptor is not in non-blocking mode), the
	 * write will block until data is read from the FIFO.
	 *
	 */
	int set_writeblock() const;

};

};

#endif /* FIFO_HPP_ */
