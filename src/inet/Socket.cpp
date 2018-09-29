
#include "inet/Socket.hpp"

using namespace inet;

bool Socket::m_is_initialized = false;

Socket::Socket(){
#if defined __win32
	m_socket=INVALID_SOCKET;
#endif
	init();
}

bool Socket::is_valid() const {
#if defined __win32
	return m_socket != INVALID_SOCKET;
#else
	return m_socket >= 0;
#endif
}

int Socket::decode_socket_return(int value){
#if defined __win32
	switch(value){
		case INVALID_SOCKET:
			//set error number
			return -1;
		case SOCKET_ERROR:
			//set error number
			return -1;
		default:
			return value;
	}
#else
	return value;
#endif
}

int Socket::init() {
	if( m_is_initialized ){
		return 0;
	}
	m_is_initialized = true;
#if defined __win32
	WSADATA wsadata;
	int result;

	// Initialize Winsock
	result = WSAStartup(MAKEWORD(2,2), &wsadata);
	if (result != 0) {
		//set_error_number(EPIPE);
		return -1;
	}

	return 0;
#else
	return 0;
#endif
}



int Socket::create(const SocketAttributes & socket_attributes){

	m_socketattributes = socket_attributes;
	int family = socket_attributes.family() == SocketAttributes::FAMILY_INET ? AF_INET : AF_UNSPEC;
	int type = socket_attributes.type() == SocketAttributes::TYPE_STREAM ? SOCK_STREAM : SOCK_DGRAM;
	int protocol = socket_attributes.protocol() == SocketAttributes::PROTOCOL_TCP ? IPPROTO_TCP : IPPROTO_UDP;


	m_socket = socket(family, type, protocol);

	if( is_valid() == false ){
		//set_error_number_if_error(??);
		return -1;
	}

	return 0;
}


int Socket::bind() {
	struct addrinfo *result = NULL;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = m_socketattributes.family() == SocketAttributes::FAMILY_INET ? AF_INET : AF_UNSPEC;
	hints.ai_socktype = m_socketattributes.type() == SocketAttributes::TYPE_STREAM ? SOCK_STREAM : SOCK_DGRAM;
	hints.ai_protocol = m_socketattributes.protocol() == SocketAttributes::PROTOCOL_TCP ? IPPROTO_TCP : IPPROTO_UDP;;
	int flags = m_socketattributes.flags();
	if(flags & SocketAttributes::FLAG_PASSIVE ){
		hints.ai_flags = AI_PASSIVE;
	}
#if defined __win32

	// Resolve the server address and port
	int ret = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if ( ret != 0 ) {
		printf("getaddrinfo failed with error: %d\n", ret);
		return WS_ERROR;
	}
	// Setup the TCP listening socket
	ret = decode_socket_return( ::bind( m_socket, result->ai_addr, (int)result->ai_addrlen) );
	if (ret == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		set_error_number(WS_ERR_NET_BIND_FAILED);
		return WS_ERROR;
	} else {
		memcpy((void*)&(m_socketattributes.ipv4_address().m_sockaddr),(void*)result->ai_addr, sizeof(result->ai_addrlen));
	}
	freeaddrinfo(result);
	return WS_OK;
#else
	return -1;
#endif
}

int Socket::listen(){
#if defined __win32
	int ret = decode_socket_return( ::listen(m_socket, SOMAXCONN) );
	if (ret == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		set_error_number(WS_ERR_NET_LISTEN_FAILED);
		return WS_ERROR;
	}
	return WS_OK;
#else
	return -1;
#endif
}

Socket Socket::accept(){
	Socket result;
	result.m_socket = decode_socket_return( ::accept(m_socket, 0, 0) );
	return result;
}
int Socket::connect() {
	// Connect to server.
	struct sockaddr_in* addr = (struct sockaddr_in*)&(m_socketattributes.ipv4_address().m_sockaddr);
	return decode_socket_return( ::connect(m_socket, (struct sockaddr*)addr, sizeof(*addr)) );
}

int Socket::write(const void * buf, int nbyte) {
	return decode_socket_return( ::send(m_socket, buf, nbyte, 0 ) );
}


int Socket::read(void * buf, int nbyte) {
	return decode_socket_return( ::recv(m_socket, buf, nbyte, 0 ) );
}

int Socket::shutdown(int how){
	int socket_how = SHUT_RDWR;
	if( (how & ACCESS_MODE) == READONLY ){
		socket_how = SHUT_RD;
	} else if( (how & ACCESS_MODE) == WRITEONLY ){
		socket_how = SHUT_WR;
	}
	return decode_socket_return( ::shutdown(m_socket, socket_how) );
}


int Socket::close() {
#if defined __win32
	return decode_socket_return( closesocket(m_socket) );
#else
	return ::close(m_socket);
#endif
}


Socket::~Socket() {
	close();
}



//TODO:1)Move target_link_libraries(lws2_32.a) from CMakefiles.txt of sl to StratifyAPI
//2)Add variables to store Address family, protocol to be used.
