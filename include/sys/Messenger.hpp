/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
/* Copyright 2017 tgil All Rights Reserved */

#ifndef SAPI_SYS_MESSENGER_HPP_
#define SAPI_SYS_MESSENGER_HPP_

/*! \cond */

#include <pthread.h>
#include "../api/SysObject.hpp"
#include "Thread.hpp"
#include "../fmt/Son.hpp"
#include "File.hpp"
#include "Mutex.hpp"
#include "../var/ConstString.hpp"

namespace sys {

/*! \brief Messenger Class
 * \details This class creates a new thread dedicated to handling message passing.
 *
 */
class Messenger : public api::WorkObject {
public:

	enum {
		CHANNEL_DISABLED = 255
	};

	/*! \details Constructs a new messenger with the specified \a stack size for the thread.
	 *
	 * @param stack_size The number of bytes to use for the Messenger thread stack size
	 */
	Messenger(int stack_size = 2048);

	/*! \details Starts the messenger.
	 *
	 * @param device The path to the device where messages will be passed
	 * @param read_channel The channel to use for reading messages
	 * @param write_channel The channel to use for writing messages
	 * @return Zero on success
	 */
	int start(const char * device, int read_channel, int write_channel);

	/*! \details Stops the messenger.
	 *
	 */
	void stop();

	/*! \details Returns true if the messenger thread is running. */
	bool is_running() const { return !m_is_stopped; }

	/*! \details Sends a message.
	 *
	 * @param message A reference to the message
	 * @return Zero if message was successfully queued
	 *
	 * This method will acquire a mutex on the messenger
	 * before sending. The mutex may be locked waiting for an
	 * incoming message in which case send_message() will wait
	 * until that completes before sending.
	 *
	 */
	int send_message(fmt::Son & message);

	/*! \details Handles incoming messages.
	 *
	 * @param message A reference to the incoming message
	 *
	 * The default action for this method simply ignores
	 * all incoming messages. You can re-implement this method
	 * using an inherited class to handle messages.
	 *
	 */
	virtual void handle_message(fmt::Son & message){}

	u16 max_message_size() const { return m_max_message_size; }

	void set_max_message_size(u16 size){ m_max_message_size = size; }

	u16 timeout() const { return m_timeout_ms; }

	void set_timeout(u16 timeout_ms){
		m_timeout_ms = timeout_ms;
	}

	var::Data & data(){ return m_message_data; }

	int fileno() const { return m_device.fileno(); }

	u8 read_channel() const { return m_read_channel; }
	u8 write_channel() const { return m_write_channel; }

private:
	static void * listener_work(void * args);
	void listener();

	volatile bool m_stop;
	volatile bool m_is_stopped;
	Thread m_listener;
	u8 m_read_channel;
	u8 m_write_channel;
	u16 m_max_message_size;
	u16 m_timeout_ms;
	File m_device;
	sys::Mutex m_mutex;
	var::Data m_message_data;
};

}

/*! \endcond */


#endif /* SAPI_SYS_MESSENGER_HPP_ */
