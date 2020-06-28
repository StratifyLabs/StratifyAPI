/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved
#if !defined __link


#include "sys/Mq.hpp"
using namespace sys;

Mq::Mq(){
	m_handle = -1;
	m_msg_prio = 0;
}

int Mq::open(
		const var::String & name,
		int oflag,
		const fs::Permissions & permissions,
		const struct mq_attr * attr){
	m_handle = mq_open(
				name.cstring(),
				oflag,
				permissions.permissions(),
				attr);
	return m_handle;
}

int Mq::create(
		const var::String & name,
		int oflag,
		const fs::Permissions & permissions,
		long flags,
		long maxmsg,
		long msgsize){
	struct mq_attr attr;
	attr.mq_flags = flags;
	attr.mq_maxmsg = maxmsg;
	attr.mq_msgsize = msgsize;
	attr.mq_curmsgs = 0;
	m_handle = mq_open(
				name.cstring(),
				flag_create | oflag,
				permissions.permissions(),
				&attr
				);
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

int Mq::get_attr(struct mq_attr & mqstat){
	return mq_getattr(m_handle, &mqstat);
}

MqAttributes Mq::get_attributes(){
	MqAttributes a;
	mq_getattr(m_handle, &(a.m_attr));
	return a;
}

int Mq::set_attributes(const struct mq_attr * mqstat,
							  struct mq_attr * omqstat){
	return mq_setattr(m_handle, mqstat, omqstat);
}

int Mq::set_attributes(const MqAttributes & attr){
	return mq_setattr(m_handle, &(attr.m_attr), 0);
}

s32 Mq::receive(char * msg_ptr,
					 u32 msg_len){
	return mq_receive(m_handle, msg_ptr, msg_len, &m_msg_prio);
}
s32 Mq::timedreceive(char * msg_ptr,
							u32 msg_len,
							const struct timespec * abs_timeout){
	return mq_timedreceive(m_handle, msg_ptr, msg_len, &m_msg_prio, abs_timeout);
}

int Mq::send(const char * msg_ptr,
				 u32 msg_len,
				 unsigned msg_prio){
	return mq_send(m_handle, msg_ptr, msg_len, msg_prio);
}

int Mq::timedsend(const char * msg_ptr,
						u32 msg_len,
						unsigned msg_prio,
						const struct timespec * abs_timeout){
	return mq_timedsend(m_handle, msg_ptr, msg_len, msg_prio, abs_timeout);

}

int Mq::unlink(const char * name){
	return mq_unlink(name);
}

#endif

