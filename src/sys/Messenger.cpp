/* Copyright 2017 tgil All Rights Reserved */

#include "var/String.hpp"
#include "sys/Timer.hpp"
#include "sys/Messenger.hpp"

using namespace sys;

Messenger::Messenger(int stack_size) : m_listener(stack_size){
	m_read_channel = 0;
	m_read_channel = 1;
	m_stop = true;
	m_is_stopped = true;
	m_max_message_size = 512;
	m_timeout_ms = 50;
}

int Messenger::start(const char * device, int read_channel, int write_channel){
	MutexAttr attr;
	m_read_channel = read_channel;
	m_write_channel = write_channel;
	if( m_device.open(device, File::RDWR | File::NONBLOCK) < 0 ){
		return -1;
	}
	m_stop = false;

	//initialize the mutex
	m_mutex.set_attr(MutexAttr()); //use defaults


	return m_listener.create(listener_work, this);
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
	var::Data message_data(m_max_message_size);

	m_is_stopped = false;

	while( m_stop == false ){

		m_device.seek(m_read_channel);

		m_mutex.lock();
		if( son.open_read_message(message_data) < 0 ){
			ret = -1;
		} else {
			ret = son.recv_message(m_device.fileno(), m_timeout_ms);
		}
		m_mutex.unlock();

		if( ret >= 0 ){
			handle_message(son);
		}

		Timer::wait_msec(10);
	}

	m_device.close();
	m_is_stopped = true;
}


int Messenger::send_message(fmt::Son & message){
	int ret;
	m_mutex.lock();
	m_device.seek(m_write_channel);
	ret = message.send_message(m_device.fileno(), m_timeout_ms);
	m_mutex.unlock();
	return ret;
}



