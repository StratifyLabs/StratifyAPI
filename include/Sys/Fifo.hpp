/*
 * Fifo.hpp
 *
 *  Created on: May 2, 2014
 *      Author: tgil
 */

#ifndef FIFO_HPP_
#define FIFO_HPP_

#include <sys/types.h>
#include <iface/dev/fifo.h>

#include "../Hal/Phy.hpp"

namespace Sys {

class Fifo;

class FifoAttr {
	friend class Fifo;
public:
	FifoAttr(){ memset(&attr_, 0, sizeof(attr_)); }
	uint32_t used() const { return attr_.used; }
	uint32_t size() const { return attr_.size; }
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
class Fifo : public Hal::Phy {
public:
	Fifo();

	/*! \details Copy the fifo attributes */
	int attr(fifo_attr_t * attr);

	/*! \details Return the fifo attributes */
	const FifoAttr attr(){
		FifoAttr a;
		attr(&a.attr_);
		return a;
	};

	/*! \details Flush the FIFO */
	int flush();
	/*! \details Initialize the FIFO */
	int init();
	/*! \details Exit the FIFO */
	int exit();
	/*! \details Set the FIFO write block */
	int setwriteblock();

};

};

#endif /* FIFO_HPP_ */
