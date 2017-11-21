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


	/*! \details Gets the number of channels in the multi-channel fifo. */
	int get_count() const;

	/*! \details Gets the info for the multi-channel fifo. */
	int get_info(mcfifo_info_t & info) const;

	/*! \details Gets the owner of the specified channel.
	 *
	 * @param channel The channel that is queried
	 * @return The owner value (typically a process ID)
	 *
	 * This method along with set_owner() provides a mechanism to reserve
	 * the channel for a particular application. If the owner has been
	 * set to a non-zero value, it is already being used by another
	 * process and should not be used.
	 *
	 */
	int get_owner(int channel) const;

	/*! \details Sets the owner of a channel.
	 *
	 * @param channel The channel
	 * @param owner The owner (usually the process ID)
	 * @return Zero on success
	 *
	 * This method is used with get_owner() to allow processes
	 * to reserve channels for passing information.
	 *
	 */
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
