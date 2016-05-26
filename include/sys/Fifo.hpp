/*
 * Fifo.hpp
 *
 *  Created on: May 2, 2014
 *      Author: tgil
 */

#ifndef FIFO_HPP_
#define FIFO_HPP_

#include <hal/Dev.hpp>
#include <mcu/types.h>
#include <iface/dev/fifo.h>



namespace sys {

class Fifo;

class FifoAttr {
	friend class Fifo;
public:
	FifoAttr(){ memset(&attr_, 0, sizeof(attr_)); }
	u32 used() const { return attr_.used; }
	u32 size() const { return attr_.size; }
	bool overflow() const { return attr_.overflow != 0; }
private:
	fifo_attr_t attr_;
};

/*! \brief FIFO Class */
/*! \details The Fifo class connects to Fifo devices.
 *
 * \code
 * #include <cstdio>
 * #include <stfy/Sys.hpp>
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

	/*! \details Copy the fifo attributes */
	int get_attr(fifo_attr_t * attr);

	/*! \details Return the fifo attributes */
	const FifoAttr get_attr(){
		FifoAttr a;
		get_attr(&a.attr_);
		return a;
	};

	/*! \details Flush the FIFO */
	int flush();
	/*! \details Initialize the FIFO */
	int init();
	/*! \details Exit the FIFO */
	int exit();
	/*! \details Set the FIFO write block */
	int set_writeblock();

};

};

#endif /* FIFO_HPP_ */
