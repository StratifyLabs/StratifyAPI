#include "inet/SecureSocket.hpp"

using namespace inet;

SecureSocket::SecureSocket(){

}


int SecureSocket::create(const SocketAddress & address){
	return -1;
}

//already documented in inet::Socket
int SecureSocket::connect(const SocketAddress & address){
	return -1;
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
	return -1;
}

int SecureSocket::read(void * buf, int nbyte) const {
	return -1;
}

int SecureSocket::close(){
	return -1;
}
