/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "sys/Sys.hpp"
#include "sys/requests.h"
#include "inet/SecureSocket.hpp"


using namespace inet;

SecureSocketApi SecureSocket::m_api;

SecureSocket::SecureSocket(){}


int SecureSocket::create(const SocketAddress & address){
	return api()->socket(&m_context, address.family(), address.type(), address.protocol());
}

//already documented in inet::Socket
int SecureSocket::connect(const SocketAddress & address){
	return api()->connect(m_context, address.to_sockaddr(), address.length(), address.canon_name().str());
}


//already documented in inet::Socket
int SecureSocket::bind_and_listen(const SocketAddress & address, int backlog) const {
	return -1;
}


//already documented in inet::Socket
Socket SecureSocket::accept(SocketAddress & address) const {
	return Socket();
}

//already documented in inet::Socket
int SecureSocket::shutdown(int how) const {
	return -1;
}

int SecureSocket::write(const void * buf, int nbyte) const {
	int bytes_written = 0;
	int result;
	do {
		result = api()->write(m_context, (const u8*)buf + bytes_written, nbyte - bytes_written);
		if( result > 0 ){
			bytes_written += result;
		}
	} while( result > 0 );
	return bytes_written;
}

int SecureSocket::read(void * buf, int nbyte) const {
	return api()->read(m_context, buf, nbyte);
}

int SecureSocket::close(){
	return api()->close(&m_context);
}
