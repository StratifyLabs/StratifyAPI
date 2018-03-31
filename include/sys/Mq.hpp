/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef MQ_HPP_
#define MQ_HPP_

#ifndef __link

#include "../api/SObject.hpp"
#include <sos/link.h>
#include <mqueue.h>
#include <fcntl.h>
#include <cstring>

namespace sys {

class Mq;

/*! \brief Message Queue Attribute Class */
class MqAttr {
	friend class Mq;
public:
    MqAttr(){ memset(&m_attr, 0, sizeof(m_attr)); }
	MqAttr(long f, long m, long s){
		m_attr.mq_flags = f;
		m_attr.mq_curmsgs = 0;
		m_attr.mq_maxmsg = m;
		m_attr.mq_msgsize = s;
	}

	/*! \details Message queue attribute flags */
	enum flags {
		NONBLOCK /*! \brief Non-blocking queue */ = LINK_O_NONBLOCK,
		RDWR /*! \brief Read/write queue */ = LINK_O_RDWR,
		READWRITE /*! \brief Read/write queue */ = LINK_O_RDWR,
		RDONLY /*! \brief Read only queue */ = LINK_O_RDONLY,
		READONLY /*! \brief Read only queue */ = LINK_O_RDONLY
	};

	inline long flags() const { return m_attr.mq_flags; }
	inline long curmsgs() const { return m_attr.mq_curmsgs; }
	inline long maxmsg() const { return m_attr.mq_maxmsg; }
	inline long msgsize() const { return m_attr.mq_msgsize; }

	inline void set_flags(long v) { m_attr.mq_flags = v; }
	inline void set_curmsgs(long v) { m_attr.mq_curmsgs = v; }
	inline void set_maxmsg(long v) { m_attr.mq_maxmsg = v; }
	inline void set_msgsize(long v) { m_attr.mq_msgsize = v; }

private:
	struct mq_attr m_attr;
};

/*! \brief Message Queue Class */
/*! \details This class is a wrapper for POSIX message queues.
 *
 */
class Mq : public api::SObject {
public:
	Mq();

	/*! \details Bitwise flags for open() */
	enum flags {
		CREATE /*! \brief Create a new message queue */ = O_CREAT,
		EXCL /*! \brief Create exclusively */ = O_EXCL,
		EXCLUSIVE /*! \brief Create exclusively */ = O_EXCL
	};


	/*! \details Opens a message queue.
	 *
	 * @param name The name of the message queue
	 * @param oflag Open flags
	 * @param mode Access mode
	 * @param attr Pointer to attributes
	 * @return Less than zero for an error (use perror())
	 */
	int open(const char * name, int oflag, mode_t mode = 0, const struct mq_attr * attr = 0);

	/*! \details Creates a new message queue.
	 *
	 * @param name  The name of the queue
	 * @param oflag EXCLUSIVE or 0
	 * @param mode Permissions
	 * @param flags Message queue flags
	 * @param maxmsg Max number of messages
	 * @param msgsize Message size
	 * @return Less than zero for an error (use perror())
	 */
	int create(const char * name,
			int oflag,
			mode_t mode,
			long flags,
			long maxmsg,
			long msgsize);
	/*! \brief Closes the message queue. */
	int close();

	/*! \details Returns true if the message queue is open. */
	bool is_open() const { return m_handle != -1; }

	/*! \details Is not currently supported. */
	int notify(const struct sigevent *notification);

	/*! \details Copies message queue attributes to mqstat. */
	int get_attr(struct mq_attr & mqstat);

	/*! \details Returns a copy of the message queue attributes. */
	MqAttr get_attr();
	int set_attr(const struct mq_attr * mqstat, struct mq_attr * omqstat = 0);
	int set_attr(const MqAttr & attr);

	/*! \details Receives a message from the queue.
	 *
	 * @param msg_ptr A pointer to the destination message
	 * @param msg_len The number of bytes available in \a msg_ptr
	 * @return The number of bytes read
	 *
	 */
	s32 receive(char * msg_ptr, u32 msg_len);

	s32 timedreceive(char * msg_ptr, u32 msg_len, const struct timespec * abs_timeout);
	s32 receive_timed(char * msg_ptr, u32 msg_len, const struct timespec * abs_timeout){ return timedreceive(msg_ptr, msg_len, abs_timeout); }

	/*! \details Sends a message to the queue.
	 *
	 * @param msg_ptr A pointer to the message to send
	 * @param msg_len The number of bytes to send
	 * @param msg_prio The message priority (default is 0, lowest priority)
	 * @return The number of bytes sent
	 *
	 */
	int send(const char * msg_ptr, u32 msg_len, unsigned msg_prio = 0);

	/*! \details Send a message to the queue with a timeout */
	int timedsend(const char * msg_ptr, u32 msg_len, unsigned msg_prio, const struct timespec * abs_timeout);
	int send_timed(const char * msg_ptr, u32 msg_len, unsigned msg_prio, const struct timespec * abs_timeout){ return timedsend(msg_ptr, msg_len, msg_prio, abs_timeout); }

	/*! \details Deletes a named message queue. */
	static int unlink(const char * name);

	/*! \details Returns the message priority of last message received. */
	unsigned msg_prio() const { return m_msg_prio; }

private:
	mqd_t m_handle;
	unsigned m_msg_prio;
};

}

#endif

#endif /* MQ_HPP_ */
