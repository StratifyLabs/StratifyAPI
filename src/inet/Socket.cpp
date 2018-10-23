#include "var.hpp"
#include "inet/Socket.hpp"

#if defined __win32
#define SHUT_RD SD_RECEIVE
#define SHUT_WR SD_SEND
#define SHUT_RDWR SD_BOTH
#endif

#if !defined INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

using namespace inet;

int Socket::m_is_initialized = 0;

var::Vector<SocketAddressInfo> SocketAddressInfo::fetch(
		const var::ConstString & node,
		const var::ConstString & server){
	var::Vector<SocketAddressInfo> result;

	struct addrinfo * info;

	Socket::initialize();
    //may return error number
    if( getaddrinfo(node.str(), server.str(), &m_addrinfo, &info) != 0 ){
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

	Socket::deinitialize();

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
		const struct sockaddr_in6 * addr = m_sockaddr.to<struct sockaddr_in6>();
#if defined __link
#if defined __macosx
		result.format("%04X:%04X:%04X:%04X:%04X:%04X:%04X:%04X",
						  ((u16)addr->sin6_addr.__u6_addr.__u6_addr8[0]) << 8 | addr->sin6_addr.__u6_addr.__u6_addr8[1],
				((u16)addr->sin6_addr.__u6_addr.__u6_addr8[2]) << 8 | addr->sin6_addr.__u6_addr.__u6_addr8[3],
				((u16)addr->sin6_addr.__u6_addr.__u6_addr8[4]) << 8 | addr->sin6_addr.__u6_addr.__u6_addr8[5],
				((u16)addr->sin6_addr.__u6_addr.__u6_addr8[6]) << 8 | addr->sin6_addr.__u6_addr.__u6_addr8[7],
				((u16)addr->sin6_addr.__u6_addr.__u6_addr8[8]) << 8 | addr->sin6_addr.__u6_addr.__u6_addr8[9],
				((u16)addr->sin6_addr.__u6_addr.__u6_addr8[10]) << 8 | addr->sin6_addr.__u6_addr.__u6_addr8[11],
				((u16)addr->sin6_addr.__u6_addr.__u6_addr8[12]) << 8 | addr->sin6_addr.__u6_addr.__u6_addr8[13],
				((u16)addr->sin6_addr.__u6_addr.__u6_addr8[14]) << 8 | addr->sin6_addr.__u6_addr.__u6_addr8[15]);
#else

#endif
#else
		result.format("%04X:%04X:%04X:%04X:%04X:%04X:%04X:%04X",
						  ((u16)addr->sin6_addr.un.u8_addr[0]) << 8 | addr->sin6_addr.un.u8_addr[1],
				((u16)addr->sin6_addr.un.u8_addr[2]) << 8 | addr->sin6_addr.un.u8_addr[3],
				((u16)addr->sin6_addr.un.u8_addr[4]) << 8 | addr->sin6_addr.un.u8_addr[5],
				((u16)addr->sin6_addr.un.u8_addr[6]) << 8 | addr->sin6_addr.un.u8_addr[7],
				((u16)addr->sin6_addr.un.u8_addr[8]) << 8 | addr->sin6_addr.un.u8_addr[9],
				((u16)addr->sin6_addr.un.u8_addr[10]) << 8 | addr->sin6_addr.un.u8_addr[11],
				((u16)addr->sin6_addr.un.u8_addr[12]) << 8 | addr->sin6_addr.un.u8_addr[13],
				((u16)addr->sin6_addr.un.u8_addr[14]) << 8 | addr->sin6_addr.un.u8_addr[15]);
#endif
	}

	return result;
}

int SocketAddressIpv4::set_address(const var::ConstString & addr){
	Token tokens(addr, ".");
	if( tokens.count() != 4 ){
		return -1;
	}

	set_address(tokens.at(0).atoi(),
					tokens.at(1).atoi(),
					tokens.at(2).atoi(),
					tokens.at(3).atoi());

	return 0;
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

int Socket::decode_socket_return(int value) const {
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
	return set_error_number_if_error(value);
#endif
}

int Socket::initialize() {
	if( m_is_initialized == 0 ){

#if defined __win32
		WSADATA wsadata;
		int result;

		// Initialize Winsock
		WSAStartup(MAKEWORD(1,0), &wsadata);
		result = WSAStartup(wsadata.wHighVersion, &wsadata);
		if (result != 0) {
			//set_error_number(EPIPE);
			return -1;
		}

#endif
	}

	m_is_initialized++;
	return 0;
}

int Socket::deinitialize(){
	if( m_is_initialized > 0 ){
		m_is_initialized--;
		if( m_is_initialized == 0 ){
#if defined __win32
			WSACleanup();
#endif
		}
	}
	return 0;
}

Socket::~Socket() {
	close();
}



int Socket::create(const SocketAddress & address){

	m_socket = socket(address.family(), address.type(), address.protocol());

	if( is_valid() == false ){
		//set_error_number_if_error(??);
		return -1;
	}

	return 0;
}


int Socket::bind_and_listen(const SocketAddress & addr, int backlog) const {
	int result = decode_socket_return( ::bind(m_socket, addr.to_sockaddr(), addr.length()) );
	if( result < 0 ){
		return result;
	}

	if( addr.protocol() == SocketAddressInfo::PROTOCOL_TCP ){
		result =  decode_socket_return( ::listen(m_socket, backlog) );
	}

	return result;
}

Socket Socket::accept(SocketAddress & address) const{
	Socket result;
	socklen_t len = sizeof(struct sockaddr_in6);
	address.m_sockaddr.alloc(len);
	result.m_socket = decode_socket_return(
				::accept(m_socket,
							address.m_sockaddr.to<struct sockaddr>(),
							&len)
				);
	address.m_sockaddr.set_size(len);
	return result;
}

int Socket::connect(const SocketAddress & address) {
	// Connect to server.
	return decode_socket_return(
				::connect(m_socket, address.to_sockaddr(), address.length())
				);
}

int Socket::write(const void * buf, int nbyte) const {
	return decode_socket_return( ::send(m_socket, (const char*)buf, nbyte, 0 ) );
}

int Socket::write_to(const void * buf, int nbyte, const struct sockaddr * ai_addr,socklen_t ai_addrlen) const {
    return decode_socket_return( ::sendto(m_socket, (const char*)buf, nbyte, 0, ai_addr, ai_addrlen));
}


int Socket::read(void * buf, int nbyte) const {
	return decode_socket_return( ::recv(m_socket, (char*)buf, nbyte, 0 ) );
}
int Socket::read_from(void * buf, int nbyte,struct sockaddr * ai_addr,socklen_t * ai_addrlen) const {
    return decode_socket_return( ::recvfrom(m_socket, (char*)buf, nbyte, 0 ,ai_addr, ai_addrlen) );
}

int Socket::shutdown(int how) const{
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
	int result = decode_socket_return( ::close(m_socket) );
	m_socket = INVALID_SOCKET;
	return result;
#endif
}

Socket & Socket::operator << (const SocketOption & option){
	decode_socket_return( ::setsockopt(m_socket,
												  option.m_level,
												  option.m_name,
												  option.m_option_value.to_char(),
												  option.m_option_value.size()) );
	return *this;
}
