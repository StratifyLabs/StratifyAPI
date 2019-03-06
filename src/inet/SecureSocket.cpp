/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "sys/Sys.hpp"
#include "sys/requests.h"
#include "inet/SecureSocket.hpp"


using namespace inet;

SecureSocketApi SecureSocket::m_api;

SecureSocket::SecureSocket(){
	m_ticket_lifetime = 0; //don't generate a ticket
}

SecureSocket::SecureSocket(u32 ticket_lifetime){
	m_ticket_lifetime = ticket_lifetime;
	m_ticket.set_size(0);
}

int SecureSocket::create(const SocketAddress & address){
	int result = api()->socket(&m_context, address.family(), address.type(), address.protocol());
	m_fd = api()->fileno(&m_context);
	return set_error_number_if_error(result);
}

//already documented in inet::Socket
int SecureSocket::connect(const SocketAddress & address){
	int result;

	if( m_ticket.size() > 0 ){
		result = api()->parse_ticket(m_context, m_ticket.to_void(), m_ticket.size());
		if( result < 0 ){
			printf("Ticket parse failed: 0x%X\n", result*-1);
		}
	}

	result = api()->connect(m_context, address.to_sockaddr(), address.length(), address.canon_name().str());

	if( m_ticket_lifetime && result == 0){
		m_ticket.set_size(2619);
		do {
			result = api()->write_ticket(m_context, m_ticket.to_void(), m_ticket.size(), m_ticket_lifetime);
			if( result == MBEDTLS_ERR_SSL_BUFFER_TOO_SMALL ){
				m_ticket.set_size(m_ticket.size() + 64);
			}
		} while( (result == MBEDTLS_ERR_SSL_BUFFER_TOO_SMALL) && (m_ticket.size() < 4096) );

		if( result > 0 ){
			m_ticket.set_size(result);
		}
	}
	return result;
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
	if( result < 0 && bytes_written == 0 ){
		return set_error_number_if_error( result );
	}
	return bytes_written;
}

int SecureSocket::read(void * buf, int nbyte) const {
	return set_error_number_if_error( api()->read(m_context, buf, nbyte) );
}

int SecureSocket::close(){
	int result = 0;
	if( m_fd != -1 ){
		result = set_error_number_if_error( api()->close(&m_context) );
		m_fd = -1;
	}
	return result;
}
