/*! \file */ //Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

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

SocketAddressInfo::SocketAddressInfo(int family, int type, int protocol, int flags){
	memset(&m_addrinfo, 0, sizeof(m_addrinfo));
	set_flags(flags);
	set_family(family);
	set_type(type);
	set_protocol(protocol);
	if( family == FAMILY_INET ){
		m_sockaddr.allocate(sizeof(struct sockaddr_in));
#if !defined __win32
		m_sockaddr.to<struct sockaddr_in>()->sin_len = m_sockaddr.size();
#else

#endif
		m_sockaddr.to<struct sockaddr_in>()->sin_family = family;
		memset(&(m_sockaddr.to<struct sockaddr_in>()->sin_addr), 0, sizeof(struct in_addr));
		memset(m_sockaddr.to<struct sockaddr_in>()->sin_zero, 0, sizeof(m_sockaddr.to<struct sockaddr_in>()->sin_zero));
	} else {
		m_sockaddr.allocate(sizeof(struct sockaddr_in6));
#if !defined __win32
		m_sockaddr.to<struct sockaddr_in6>()->sin6_len = m_sockaddr.size();
#else

#endif
		m_sockaddr.to<struct sockaddr_in6>()->sin6_family = family;
		m_sockaddr.to<struct sockaddr_in6>()->sin6_flowinfo = 0;
		m_sockaddr.to<struct sockaddr_in6>()->sin6_scope_id = 0;
		m_sockaddr.to<struct sockaddr_in6>()->sin6_port = 0;
		memset(&(m_sockaddr.to<struct sockaddr_in6>()->sin6_addr), 0, sizeof(struct in6_addr));
	}
}

var::Vector<SocketAddressInfo> SocketAddressInfo::fetch(
		Node node,
		Server server){
	var::Vector<SocketAddressInfo> result;
	int result_int;

	struct addrinfo * info_start;
	struct addrinfo * info;

	const char * server_cstring;
	const char * node_cstring;

	server_cstring =
			server.argument().is_empty() ?
				nullptr :
				server.argument().cstring();

	node_cstring =
			node.argument().is_empty() ?
				nullptr :
				node.argument().cstring();

	Socket::initialize();

	m_addrinfo.ai_flags |= AI_CANONNAME;

	if( (result_int = getaddrinfo(node_cstring, server_cstring, &m_addrinfo, &info)) != 0 ){
		set_error_number(result_int);
		return result;
	}

	info_start = info;
	do {
		SocketAddressInfo value;
		value.m_addrinfo = * info;
		if( info->ai_addr ){

			Reference address(
						Reference::ReadOnlyBuffer(info->ai_addr),
						Reference::Size(info->ai_addrlen)
						);

			value.m_sockaddr.copy_contents(address);
		}

		if( info->ai_canonname ){
			value.m_canon_name = info->ai_canonname;
		}

		result.push_back(value);

		info = info->ai_next;

	} while(info);

	freeaddrinfo(info_start);
	Socket::deinitialize();

	return result;
}

#if 0
int Ipv4Address::set_address(const var::String & address) {
	var::Token address_token(address,".");
	if( address_token.count() != 4 ){
		return -1;
	}

	set_address(var::String(address_token.at(0)).atoi(),
					var::String(address_token.at(0)).atoi(),
					var::String(address_token.at(0)).atoi(),
					var::String(address_token.at(0)).atoi());

	return 0;
}
#endif

u16 SocketAddress::port() const {
	if( family() == SocketAddressInfo::FAMILY_INET ){
		return ntohs(m_sockaddr.to<const sockaddr_in>()->sin_port);
	} else if( family() == SocketAddressInfo::FAMILY_INET6 ){
		return ntohs(m_sockaddr.to<const sockaddr_in6>()->sin6_port);
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

int SocketAddressIpv4::set_address(const var::String & addr){
	Tokenizer tokens(
				addr,
				var::Tokenizer::Delimeters(".")
				);
	if( tokens.count() != 4 ){
		return -1;
	}

	set_address(String(tokens.at(0)).to_integer(),
					String(tokens.at(1)).to_integer(),
					String(tokens.at(2)).to_integer(),
					String(tokens.at(3)).to_integer());

	return 0;
}



Socket::Socket(){
	m_socket = SOCKET_INVALID;
	initialize();
}

bool Socket::is_valid() const {
	return m_socket != SOCKET_INVALID;
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

	if( m_socket != SOCKET_INVALID ){
		this->close();
	}

	m_socket = socket(
				address.family(),
				address.type(),
				address.protocol()
				);

	if( is_valid() == false ){
		//set_error_number_if_error(??);
		return -1;
	}

	return 0;
}

int Socket::bind(const SocketAddress & addr) const {
	return decode_socket_return(
				::bind(
					m_socket,
					addr.to_sockaddr(),
					static_cast<int>(addr.length())
					) );
}


int Socket::bind_and_listen(
		const SocketAddress & addr,
		ListenBacklogCount backlog
		) const {
	int result = bind(addr);
	if( result < 0 ){
		return result;
	}

	if( addr.protocol() == SocketAddressInfo::PROTOCOL_TCP ){
		result =  decode_socket_return(
					::listen(m_socket,
								backlog.argument()
								)
					);
	}

	return result;
}

Socket Socket::accept(
		SocketAddress & address
		) const{
	Socket result;
	socklen_t len = sizeof(struct sockaddr_in6);
	address.m_sockaddr.allocate(len);
	result.m_socket = decode_socket_return(
				::accept(
					m_socket,
					address.m_sockaddr.to<struct sockaddr>(),
					&len
					)
				);
	address.m_sockaddr.resize(len);
	return result;
}

int Socket::connect(const SocketAddress & address) {
	// Connect to server.

	if( m_socket == SOCKET_INVALID ){
		int result;
		if( (result = this->create(address)) < 0 ){
			return result;
		}
	}

	return decode_socket_return(
				::connect(
					m_socket,
					address.to_sockaddr(),
					static_cast<int>(address.length())
					)
				);
}

int Socket::write(const void * buf,
						Size nbyte
						) const {
	return decode_socket_return(
				::send(
					m_socket,
					(const char*)buf,
					static_cast<int>(nbyte.argument()),
					0
					)
				);
}

int Socket::write(const void * buf, Size nbyte, const struct sockaddr * ai_addr, socklen_t ai_addrlen) const {
	return decode_socket_return(
				::sendto(
					m_socket,
					(const char*)buf,
					static_cast<int>(nbyte.argument()),
					0,
					ai_addr,
					ai_addrlen
					)
				);
}


int Socket::read(
		void * buf,
		Size nbyte) const {
	return decode_socket_return(
				::recv(
					m_socket,
					(char*)buf,
					nbyte.argument(),
					0
					)
				);
}

int Socket::read(
		var::Reference & data,
		const SocketAddress & address
		){
	socklen_t address_len = address.m_sockaddr.size();
	return decode_socket_return(
				::recvfrom(
					m_socket,
				#if defined __win32
					data.to_char(),
				#else
					data.to_void(),
				#endif
					data.size(),
					0,
					address.m_sockaddr.to<struct sockaddr>(),
					&address_len) );
}

int Socket::read(
		void * buf,
		Size nbyte,
		const SocketAddress & address
		){
	socklen_t address_len = address.m_sockaddr.size();
	return decode_socket_return(
				::recvfrom(
					m_socket,
					(char*)buf,
					static_cast<int>(nbyte.argument()),
					0,
					address.m_sockaddr.to<struct sockaddr>(),
					&address_len
					)
				);
}

int Socket::read(
		void * buf,
		Size nbyte,
		struct sockaddr * ai_addr,
		socklen_t * ai_addrlen
		) const {
	return decode_socket_return(
				::recvfrom(
					m_socket,
					(char*)buf,
					static_cast<int>(nbyte.argument()),
					0,
					ai_addr,
					ai_addrlen
					)
				);
}

int Socket::shutdown(const fs::OpenFlags how) const{
	int socket_how = SHUT_RDWR;
	if( how.is_read_only() ){
		socket_how = SHUT_RD;
	} else if( how.is_write_only() ){
		socket_how = SHUT_WR;
	}
	return decode_socket_return( ::shutdown(m_socket, socket_how) );
}


int Socket::close() {
	int result = 0;
	if( m_socket != SOCKET_INVALID ){
#if defined __win32
		result = decode_socket_return( closesocket(m_socket) );
#else
		result = decode_socket_return( ::close(m_socket) );
#endif
		m_socket = SOCKET_INVALID;
	}
	return result;
}

Socket & Socket::operator << (const SocketOption & option){
	decode_socket_return( ::setsockopt(m_socket,
												  option.m_level,
												  option.m_name,
											  #if defined __win32
												  option.m_option_value.to_const_char(),
											  #else
												  option.m_option_value.to_const_void(),
											  #endif
												  option.m_option_value.size()) );
	return *this;
}
