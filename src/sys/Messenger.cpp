/* Copyright 2017 tgil All Rights Reserved */

#include "var/String.hpp"
#include "sys/Timer.hpp"
#include "sys/Messenger.hpp"

using namespace sys;

Messenger::Messenger(int stack_size) : m_listener(stack_size){
	m_read_channel = CHANNEL_DISABLED;
	m_write_channel = CHANNEL_DISABLED;
	m_stop = true;
	m_is_stopped = true;
	m_max_message_size = 512;
	m_timeout_ms = 50;
}

int Messenger::start(const char * device, int read_channel, int write_channel){
	MutexAttr attr;
	int ret;

	if( m_message_data.alloc(m_max_message_size)  < 0 ){
		m_read_channel = CHANNEL_DISABLED;
		m_write_channel = CHANNEL_DISABLED;
		return -2;
	}

	if( m_device.open(device, File::RDWR | File::NONBLOCK) < 0 ){
		m_read_channel = CHANNEL_DISABLED;
		m_write_channel = CHANNEL_DISABLED;
		return -3;
	}
	m_stop = false;

	//initialize the mutex
	m_mutex.set_attr(MutexAttr()); //use defaults

	m_read_channel = read_channel;
	m_write_channel = write_channel;

	if( m_read_channel != CHANNEL_DISABLED ){
		ret = m_listener.create(listener_work, this);
	} else {
		ret = 0;
	}

	return ret;
}

void Messenger::stop(){
	m_stop = true;
}

void *  Messenger::listener_work(void * args){
	Messenger * me = (Messenger*)args;
	me->listener();
	return 0;
}

void Messenger::listener(){
	int ret;
	fmt::Son son;

	m_is_stopped = false;

	while( m_stop == false ){

		m_mutex.lock();
		if( son.open_read_message(m_message_data) < 0 ){
			ret = -1;
		} else {
			m_device.seek(m_read_channel);
			ret = son.recv_message(m_device.fileno(), m_timeout_ms);
		}
		m_mutex.unlock();

		if( ret >= 0 ){
			handle_message(son);
		} else {
			Timer::wait_milliseconds(m_timeout_ms);
		}
	}

	m_device.close();
	m_is_stopped = true;
	m_message_data.free();
}


int Messenger::send_message(fmt::Son & message){
	int ret = -1;
	if( m_write_channel != CHANNEL_DISABLED ){
		m_mutex.lock();
		m_device.seek(m_write_channel);
		ret = message.send_message(m_device.fileno(), m_timeout_ms);
		m_mutex.unlock();
	}
	return ret;
}



