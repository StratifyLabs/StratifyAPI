/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved

#include "sys/Sys.hpp"
#include "var/Message.hpp"
#include "sys.hpp"
#include "fmt.hpp"

using namespace sys;

namespace var {

Message::Message(const hal::Dev & device, int max_capacity) : Data(max_capacity), m_dev(device){
	// TODO Auto-generated constructor stub
	m_state = INIT_STATE;
}

Message::~Message(){}

int Message::create(){
	int ret;
	clear();
	ret = create_message(data(), capacity());
	if( ret >= 0 ){
		m_state = CREATE_STATE;
	} else {
		clear();
		m_state = INIT_STATE;
	}
	return ret;
}

int Message::edit(){
	int ret = 0;
	if( m_state != EDIT_STATE ){
		close();
		ret = open_edit_message(data(), capacity());
		if( ret >= 0 ){
			m_state = EDIT_STATE;
		}
	}
	return ret;
}

int Message::read(){
	int ret = 0;
	if( m_state != READ_STATE ){
		close();
		ret = open_read_message(data(), capacity());
		if( ret >= 0 ){
			m_state = READ_STATE;
		}
	}
	return ret;
}



int Message::get_size(){
	return get_message_size();
}

int Message::save(const char * path){
	File f;
	int s = get_size();

	if( s > 0 ){
		if( f.create(path, false) < 0 ){
			return -1;
		}

		if( f.write(data(), s) != s ){
			return -1;
		}

		return f.close();

	}
	return 0;
}

int Message::load(const char * path){

	//get the size of the file -- see if it fits

	//not yet implemented
	return -1;
}


int Message::recv(){
	int ret = -1;
	if( read() < 0 ){
		return -1;
	}
	if( (ret = recv_message(m_dev.fileno(), m_timeout)) > 0 ){
		if( read() < 0 ){
			ret = -1;
		}
	}
	return ret;
}

int Message::send(){
	if( state() != READ_STATE ){
		if( close() < 0 ){ return -1; }
		if( read() < 0 ){ return -1; }
	}

	return send_message(m_dev.fileno(), m_timeout);
}

} /* namespace var */
