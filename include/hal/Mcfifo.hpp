/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


#ifndef HAL_MCFIFO_HPP_
#define HAL_MCFIFO_HPP_

#include <sos/dev/mcfifo.h>

#include "Periph.hpp"

namespace hal {

/*! \brief Multi-Channel Fifo Class
 *
 */
class Mcfifo : public Dev {
public:
	Mcfifo();


	int get_count() const;

	int get_info(mcfifo_info_t & info) const;

	int get_owner(int channel) const;

	int set_owner(int channel, int owner) const;


	int init_fifo(int channel) const;
	int flush_fifo(int channel) const;
	int exit_fifo(int channel) const;
	int get_fifo_info(int channel, fifo_info_t & info) const;
	int set_fifo_attr(int channel, fifo_attr_t & attr) const;


	int ioctl_fifo(int channel, int request, void * ctl = 0) const;

};

} /* namespace hal */

#endif /* HAL_MCFIFO_HPP_ */
