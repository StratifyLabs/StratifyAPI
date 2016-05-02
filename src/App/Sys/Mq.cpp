//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include "Sys/Mq.hpp"
using namespace Sys;

Mq::Mq(){
	handle = -1;
	msg_prio_ = 0;
}

int Mq::open(const char * name,
		int oflag,
		mode_t mode,
		const struct mq_attr * attr){
	handle = mq_open(name, oflag, mode, attr);
	return handle;
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
	handle = mq_open(name, CREATE | oflag, mode, &attr);
	return handle;
}

int Mq::close(){
	int ret;
	ret = mq_close(handle);
	handle = -1;
	return ret;
}

int Mq::notify(const struct sigevent *notification){
	return mq_notify(handle, notification);
}

int Mq::attr(struct mq_attr *mqstat){
	return mq_getattr(handle, mqstat);
}
MqAttr Mq::attr(){
	MqAttr a;
	mq_getattr(handle, &(a._attr));
	return a;
}

int Mq::setattr(const struct mq_attr * mqstat,
		struct mq_attr * omqstat){
	return mq_setattr(handle, mqstat, omqstat);
}

int Mq::setattr(const MqAttr & attr){
	return mq_setattr(handle, &(attr._attr), 0);
}

ssize_t Mq::receive(char * msg_ptr,
		size_t msg_len){
	return mq_receive(handle, msg_ptr, msg_len, &msg_prio_);
}
ssize_t Mq::timedreceive(char * msg_ptr,
		size_t msg_len,
		const struct timespec * abs_timeout){
	return mq_timedreceive(handle, msg_ptr, msg_len, &msg_prio_, abs_timeout);
}

int Mq::send(const char * msg_ptr,
		size_t msg_len,
		unsigned msg_prio){
	return mq_send(handle, msg_ptr, msg_len, msg_prio);
}

int Mq::timedsend(const char * msg_ptr,
		size_t msg_len,
		unsigned msg_prio,
		const struct timespec * abs_timeout){
	return mq_timedsend(handle, msg_ptr, msg_len, msg_prio, abs_timeout);

}

int Mq::unlink(const char * name){
	return mq_unlink(name);
}

