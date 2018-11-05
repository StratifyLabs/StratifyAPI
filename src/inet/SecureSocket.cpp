#include <mbedtls_api.h>

#include "inet/SecureSocket.hpp"


using namespace inet;

SecureSocket::SecureSocket(){}


int SecureSocket::create(const SocketAddress & address){

#if defined __link
	return mbedtls_api.socket(&m_context, address.family(), address.type(), address.protocol());
#else
	//need to request the API from the system
	return -1;
#endif
}

//already documented in inet::Socket
int SecureSocket::connect(const SocketAddress & address){
#if defined __link
	return mbedtls_api.connect(m_context, address.to_sockaddr(), address.length(), address.canon_name().str());
#else
	//need to request the API from the system
	return -1;
#endif
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
#if defined __link
	int bytes_written = 0;
	int result;
	do {
		result = mbedtls_api.write(m_context, (const u8*)buf + bytes_written, nbyte - bytes_written);
		if( result > 0 ){
			bytes_written += result;
		}
	} while( result > 0 );
	return bytes_written;
#else
	//need to request the API from the system
	return -1;
#endif
}

int SecureSocket::read(void * buf, int nbyte) const {
#if defined __link
	return mbedtls_api.read(m_context, buf, nbyte);
#else
	//need to request the API from the system
	return -1;
#endif
}

int SecureSocket::close(){
#if defined __link
	return mbedtls_api.close(&m_context);
#else
	//need to request the API from the system
	return -1;
#endif
}
