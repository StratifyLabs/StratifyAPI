#include "var.hpp"
#include "inet/Socket.hpp"

#if defined __win32
#define SHUT_RD SD_RECEIVE
#define SHUT_WR SD_SEND
#define SHUT_RDWR SD_BOTH
#endif

using namespace inet;

bool Socket::m_is_initialized = false;

var::Vector<SocketAddressInfo> SocketAddressInfo::fetch(
		const var::ConstString & node,
		const var::ConstString & server){
	var::Vector<SocketAddressInfo> result;

	struct addrinfo * info;

	if( getaddrinfo(node.str(), server.str(), &m_addrinfo, &info) < 0 ){
		return result;
	}

	do {
		SocketAddressInfo value;
		value.m_addrinfo = *info;
		if( info->ai_addr ){
			value.m_sockaddr.copy_contents(var::Data(info->ai_addr, info->ai_addrlen));
		}

		if( info->ai_canonname ){
			value.m_canon_name = info->ai_canonname;
		}

		result.push_back(value);

		info = info->ai_next;

	} while(info);

	freeaddrinfo(info);

	return result;
}

#if 0
int Ipv4Address::set_address(const var::ConstString & address) {
	var::Token address_token(address,".");
	if( address_token.count() != 4 ){
		return -1;
	}

	set_address(var::ConstString(address_token.at(0)).atoi(),
					var::ConstString(address_token.at(0)).atoi(),
					var::ConstString(address_token.at(0)).atoi(),
					var::ConstString(address_token.at(0)).atoi());

	return 0;
}
#endif

u16 SocketAddress::port() const {
	if( family() == SocketAddressInfo::FAMILY_INET ){
		return htons(m_sockaddr.to<const sockaddr_in>()->sin_port);
	} else if( family() == SocketAddressInfo::FAMILY_INET6 ){
		return htons(m_sockaddr.to<const sockaddr_in6>()->sin6_port);
	}
	return 0;
}

void SocketAddress::set_port(u16 port){
	if( is_ipv4() ){
		m_sockaddr.to<struct sockaddr_in>()->sin_port = htons(port);
	} else if( is_ipv6() ){
		m_sockaddr.to<struct sockaddr_in6>()->sin6_port = htons(port);
	}
}

var::String SocketAddress::address_to_string() const {
	var::String result;
	if( is_ipv4() ){
		u32 address = address_ipv4();
		result.format("%d.%d.%d.%d",
						  (address >> 0) & 0xff,
						  (address >> 8) & 0xff,
						  (address >> 16) & 0xff,
						  (address >> 24) & 0xff
						  );
	}


	if( is_ipv6() ){
		result.format("ipv6 address");
	}

	return result;
}


Socket::Socket(){
#if defined __win32
	m_socket=INVALID_SOCKET;
#endif
	initialize();
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
			//case SOCKET_ERROR:
			//set error number
			//return -1;
		default:
			return value;
	}
#else
	return value;
#endif
}

int Socket::initialize() {
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



int Socket::create(const SocketAddress & address){

	m_address = address;

	m_socket = socket(address.family(), address.type(), address.protocol());

	if( is_valid() == false ){
		//set_error_number_if_error(??);
		return -1;
	}

	return 0;
}


int Socket::bind(const Socket & socket, const SocketAddress & addr){
	return ::bind(socket.fileno(), addr.m_sockaddr.to<struct sockaddr>(), addr.length());
}

int Socket::listen(int backlog){
	return decode_socket_return( ::listen(m_socket, backlog) );
}

Socket Socket::accept(){
	Socket result;
	result.m_socket = decode_socket_return( ::accept(m_socket, 0, 0) );
	return result;
}
int Socket::connect() {
	// Connect to server.

	return decode_socket_return(
				::connect(m_socket, m_address.to_sockaddr(), m_address.length())
				);
}

int Socket::write(const void * buf, int nbyte) {
	return decode_socket_return( ::send(m_socket, (const char*)buf, nbyte, 0 ) );
}


int Socket::read(void * buf, int nbyte) {
	return decode_socket_return( ::recv(m_socket, (char*)buf, nbyte, 0 ) );
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
