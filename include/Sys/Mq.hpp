/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef MQ_HPP_
#define MQ_HPP_

#include <iface/link.h>
#include <mqueue.h>
#include <fcntl.h>
#include <cstring>

namespace Sys {

class Mq;

/*! \brief Message Queue Attribute Class */
class MqAttr {
	friend class Mq;
public:
	MqAttr(){ memset(&_attr, 0, sizeof(_attr)); };
	MqAttr(long f, long m, long s){
		_attr.mq_flags = f;
		_attr.mq_curmsgs = 0;
		_attr.mq_maxmsg = m;
		_attr.mq_msgsize = s;
	}

	/*! \details Message queue attribute flags */
	enum flags {
		NONBLOCK /*! \brief Non-blocking queue */ = LINK_O_NONBLOCK,
		RDWR /*! \brief Read/write queue */ = LINK_O_RDWR,
		READWRITE /*! \brief Read/write queue */ = LINK_O_RDWR,
		RDONLY /*! \brief Read only queue */ = LINK_O_RDONLY,
		READONLY /*! \brief Read only queue */ = LINK_O_RDONLY
	};

	inline long flags() const { return _attr.mq_flags; }
	inline long curmsgs() const { return _attr.mq_curmsgs; }
	inline long maxmsg() const { return _attr.mq_maxmsg; }
	inline long msgsize() const { return _attr.mq_msgsize; }

	inline void setflags(long v) { _attr.mq_flags = v; }
	inline void setcurmsgs(long v) { _attr.mq_curmsgs = v; }
	inline void setmaxmsg(long v) { _attr.mq_maxmsg = v; }
	inline void setmsgsize(long v) { _attr.mq_msgsize = v; }

private:
	struct mq_attr _attr;
};

/*! \brief Message Queue Class */
/*! \details This class is a wrapper for POSIX message queues.
 *
 */
class Mq {
public:
	Mq();

	/*! \details Bitwise flags for open() */
	enum flags {
		CREATE /*! \brief Create a new message queue */ = O_CREAT,
		EXCL /*! \brief Create exclusively */ = O_EXCL,
		EXCLUSIVE /*! \brief Create exclusively */ = O_EXCL
	};


	/*! \detailsls This method opens a message queue
	 *
	 * @param name The name of the message queue
	 * @param oflag Open flags
	 * @param mode Access mode
	 * @param attr Pointer to attributes
	 * @return Less than zero for an error (use perror())
	 */
	int open(const char * name, int oflag, mode_t mode = 0, const struct mq_attr * attr = 0);

	/*! \details This method creates a new message queue.
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
	/*! \brief Close the message queue */
	int close();

	inline bool is_open() const { return handle != -1; }

	/*! \details Not currently supported */
	int notify(const struct sigevent *notification);

	/*! \details Copy message queue attributes to mqstat */
	int attr(struct mq_attr *mqstat);

	/*! \details Return the message queue attributes */
	MqAttr attr();
	int setattr(const struct mq_attr * mqstat, struct mq_attr * omqstat = 0);
	int setattr(const MqAttr & attr);

	/*! \details Receive a message from the queue */
	ssize_t receive(char * msg_ptr, size_t msg_len);

	/*! \details Receive a message from the queue with a timeout */
	ssize_t timedreceive(char * msg_ptr, size_t msg_len, const struct timespec * abs_timeout);

	/*! \details Send a message to the queue */
	int send(const char * msg_ptr, size_t msg_len, unsigned msg_prio = 0);

	/*! \details Send a message to the queue with a timeout */
	int timedsend(const char * msg_ptr, size_t msg_len, unsigned msg_prio, const struct timespec * abs_timeout);

	/*! \details Delete a message queue */
	static int unlink(const char * name);

	/*! \details Message priority of last message received */
	unsigned msg_prio() const { return msg_prio_; }

private:
	mqd_t handle;
	unsigned msg_prio_;
};

};

#endif /* MQ_HPP_ */
