/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


#include "var/Message.hpp"
#include "sys.hpp"
#include "fmt.hpp"

namespace var {

Message::Message(const char * device, int read_channel, int write_channel, int max_capacity) {
	// TODO Auto-generated constructor stub
	m_read_channel = read_channel;
	m_write_channel = write_channel;

	m_fd = ::open(device, File::RDWR | File::NONBLOCK);
}

Message::~Message(){
	if( m_fd >= 0 ){
		::close(m_fd);
	}
}

int Message::create(){
	clear();
	return create_image(data(), capacity());
}

int Message::edit(){
	return open_edit_image(data(), capacity());
}

int Message::size(){
	son_size_t data_size;
	if( seek("", &data_size) < 0 ){
		return -1;
	}
	return data_size;
}

int Message::save(const char * path){
	File f;
	int s = size();

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

	return 0;
}

int Message::send(){
	message_t msg;
	int bytes;
	int ret;
	int s = size();
	int count;

	if( s ) {

		msg.start = MESSAGE_START;
		msg.size = size();
		msg.data_checksum = 0;
		msg.checksum = 0;

		bytes = 0;
		do {
			ret = ::write(m_fd, cdata_const() + bytes, s - bytes);
			if( ret < 0 ){
				Timer::wait_msec(1);
				count++;
				if( count == timeout() ){
					return -1;
				}
			} else {
				bytes += ret;
			}
		} while( bytes < s );

	}

	return s;
}



} /* namespace var */
