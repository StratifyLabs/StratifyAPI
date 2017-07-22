/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef PHY_PERIPH_HPP_
#define PHY_PERIPH_HPP_

#include <sos/link.h>
#include <fcntl.h>
#include <cstdlib>
#include <cstring>
#include <signal.h>


#include "Dev.hpp"


namespace hal {

class PeriphParent : public Dev {
public:
	/*!  \details This method opens the peripheral.  For each instance, the peripheral
	 * only needs to be opened one time.  The port is typically opened with open() and
	 * configured with set_attr().  After that, other instances of the peripheral can read
	 * and write without opening again.
	 */
	int open(int o_mode = READWRITE);
	int ioctl(int req, void * arg) const;
	int seek(int loc, int whence) const;
	int fileno() const;
	int read(void * buf, int nbyte) const;
	int write(const void * buf, int nbyte) const;
#ifndef __link
	int read(sys::Aio & aio) const;
	int write(sys::Aio & aio) const;
#endif
	int close();

	using File::ioctl;
	using File::read;
	using File::write;

protected:
	u16 m_periph_port;

	int open(const char * name, int flags);

	void update_fileno() const;
	int lookup_fileno() const;

private:
	static u16 m_fd_map[LINK_OPEN_MAX];



};


/*! \brief Peripheral Class
 * \details This is an abstract class for a microcontroller peripheral.
 *
 * All peripherals have a common interface based on the following function:
 * - open()
 * - close()
 * - read()
 * - write()
 * - ioctl()
 *
 * The classes that inherit Periph implement a method for each ioctl() call available
 * on the peripheral.  For example, the UART has an ioctl request called I_UART_SETATTR so
 * the method Uart::set_attr() implements the ioctl request.
 *
 */
template<typename info_t, typename attr_t, char ident_char> class Periph : public PeriphParent {
public:
	typedef unsigned int port_t;
	Periph(core_periph_t periph, port_t port){
		m_periph_port = (periph << 8) | port;
		m_fd = lookup_fileno();
	}


	int get_info(info_t & info) const {
		return ioctl(_IOCTLR(ident_char, I_MCU_GETINFO, info_t), &info);
	}

	int set_attr(attr_t & attr) const {
		return ioctl(_IOCTLR(ident_char, I_MCU_SETATTR, attr_t), &attr);
	}

	int set_action(mcu_action_t & action) const {
		return ioctl(_IOCTLR(ident_char, I_MCU_SETACTION, mcu_action_t), &action);
	}

	int set_action(u32 channel, u32 event, s8 prio = 0, mcu_callback_t callback = 0, void * context = 0) const {
		mcu_action_t action;
		action.prio = prio;
		action.channel = channel;
		action.o_events = event;
		action.handler.callback = callback;
		action.handler.context = context;
		return set_action(action);
	}

	port_t port() const{ return m_periph_port & 0xFF; }

protected:


	int set_channel(const mcu_channel_t & channel, int request) const {
		return ioctl(_IOCTLR(ident_char, request, mcu_channel_t), &channel);
	}

	int set_channel(u32 loc, u32 value, int request) const {
		mcu_channel_t channel;
		channel.loc = loc;
		channel.value = value;
		return ioctl(_IOCTLR(ident_char, request, mcu_channel_t), &channel);
	}

	u32 get_channel(u32 loc, int request) const {
		mcu_channel_t channel;
		channel.loc = loc;
		if( ioctl(_IOCTLR(ident_char, request, mcu_channel_t), &channel) < 0 ){
			return (u32)-1;
		}
		return channel.value;
	}

};

};

#endif /* PHY_PERIPH_HPP_ */
