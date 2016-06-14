//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved
#if !defined __link


#include "sys/Mq.hpp"
using namespace sys;

Mq::Mq(){
	m_handle = -1;
	m_msg_prio = 0;
}

int Mq::open(const char * name,
		int oflag,
		mode_t mode,
		const struct mq_attr * attr){
	m_handle = mq_open(name, oflag, mode, attr);
	return m_handle;
}

int Mq::create(const char * name,
		int oflag,
		mode_t mode,
		long flags,
		long maxmsg,
		long msgsize){
	struct mq_attr attr;
	attr.mq_flags = flags;
	attr.mq_maxmsg = maxmsg;
	attr.mq_msgsize = msgsize;
	attr.mq_curmsgs = 0;
	m_handle = mq_open(name, CREATE | oflag, mode, &attr);
	return m_handle;
}

int Mq::close(){
	int ret;
	ret = mq_close(m_handle);
	m_handle = -1;
	return ret;
}

int Mq::notify(const struct sigevent *notification){
	return mq_notify(m_handle, notification);
}

int Mq::get_attr(struct mq_attr *mqstat){
	return mq_getattr(m_handle, mqstat);
}
MqAttr Mq::get_attr(){
	MqAttr a;
	mq_getattr(m_handle, &(a.m_attr));
	return a;
}

int Mq::set_attr(const struct mq_attr * mqstat,
		struct mq_attr * omqstat){
	return mq_setattr(m_handle, mqstat, omqstat);
}

int Mq::set_attr(const MqAttr & attr){
	return mq_setattr(m_handle, &(attr.m_attr), 0);
}

ssize_t Mq::receive(char * msg_ptr,
		size_t msg_len){
	return mq_receive(m_handle, msg_ptr, msg_len, &m_msg_prio);
}
ssize_t Mq::timedreceive(char * msg_ptr,
		size_t msg_len,
		const struct timespec * abs_timeout){
	return mq_timedreceive(m_handle, msg_ptr, msg_len, &m_msg_prio, abs_timeout);
}

int Mq::send(const char * msg_ptr,
		size_t msg_len,
		unsigned msg_prio){
	return mq_send(m_handle, msg_ptr, msg_len, msg_prio);
}

int Mq::timedsend(const char * msg_ptr,
		size_t msg_len,
		unsigned msg_prio,
		const struct timespec * abs_timeout){
	return mq_timedsend(m_handle, msg_ptr, msg_len, msg_prio, abs_timeout);

}

int Mq::unlink(const char * name){
	return mq_unlink(name);
}

#endif

