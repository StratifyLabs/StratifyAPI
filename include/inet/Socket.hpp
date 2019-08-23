/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_INET_SOCKET_HPP_
#define SAPI_INET_SOCKET_HPP_


#if defined __win32
#define _BSD_SOURCE
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdint.h>
typedef uint32_t in_addr_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#endif

#include <mcu/types.h>


#include "../api/InetObject.hpp"
#include "../fs/File.hpp"
#include "../var/String.hpp"
#include "../var/Vector.hpp"
#include "../chrono/MicroTime.hpp"

namespace inet {

class Socket;
class SocketAddress;

/*! \brief Socket Address Info
 * \details The Socket Address Info class is used
 * to get socket address information for a given
 * node and service.
 *
 * For example, if you have a web URL this class
 * can be used to converto that to information that can be
 * used to open a socket.
 *
 * The following is an example of how to use
 * this class for connected to a server.
 *
 * ```
 * #include <sapi/inet.hpp>
 * #include <sapi/var.hpp>
 *
 * SocketAddressInfo socket_address_info; //construct with defaults
 * Vector<SocketAddressInfo> result = socket_address_info.fetch_node("stratifylabs.co");
 *
 * if( result.count() > 0 ){
 *	 SocketAddress socket_address(result.at(0));
 *  Socket socket;
 *  socket.create(socket_address); //create a socket that can connect to stratifylabs.co
 *
 *  socket.connect(socket_address); //connect to stratifylabs.co
 *
 *  String request = "...";
 *  socket.write(request);
 *  //then read the response
 *
 *  socket.close();
 * }
 *
 * ```
 *
 *
 *
 */
class SocketAddressInfo : public::api::InetWorkObject {
public:

	/*! \details Enumerates the socket address family options. */
	enum family {
		FAMILY_NONE,
		FAMILY_INET /*! IPV4 */ = AF_INET,
		FAMILY_INET6 /*! IPV6 */ = AF_INET6
	};

	/*! \details Enumerates the socket address type options. */
	enum type {
		TYPE_NONE,
		TYPE_RAW /*! Raw socket data */ = SOCK_RAW,
		TYPE_STREAM /*! Streaming socket data */ = SOCK_STREAM,
		TYPE_DGRAM /*! Datagram socket data */ = SOCK_DGRAM
	};

	/*! \details Enumerates the socket address protocol options. */
	enum protocol {
		PROTOCOL_RAW /*! Raw protocol */ = IPPROTO_RAW,
		PROTOCOL_TCP /*! TCP Protocol */ = IPPROTO_TCP,
		PROTOCOL_UDP /*! UDP Procotol */ = IPPROTO_UDP,
		PROTOCOL_ICMP /*! ICMP Procotol */ = IPPROTO_ICMP,
		PROTOCOL_ICMP6 = IPPROTO_ICMPV6,
		PROTOCOL_ICMPV6 /*! ICMPv6 Protocol */ = IPPROTO_ICMPV6,
		PROTOCOL_IP /*! IP Protocol */ = IPPROTO_IP
	};

	/*! \details Constructs a new socket address infomation object.
	 *
	 * @param family Socket family (defautl is FAMILY_INET)
	 * @param type Socket type (default is TYPE_STREAM)
	 * @param protocol Socket protocol (default is PROTOCOL_TCP)
	 * @param flags Socket flags (default is none)
	 *
	 */
	SocketAddressInfo(int family = FAMILY_INET, int type = TYPE_STREAM, int protocol = PROTOCOL_TCP, int flags = 0);

	bool is_valid(){
		return m_sockaddr.size() > 0;
	}

	/*! \details Sets the flags used for getting address info.
	 *
	 */
	void set_flags(int value){ m_addrinfo.ai_flags = value; }

	/*! \details Sets the family for getting address info. */
	void set_family(int value){ m_addrinfo.ai_family = value; }
	/*! \details Sets the type for getting address info. */
	void set_type(int value){ m_addrinfo.ai_socktype = value; }
	/*! \details Sets the protocol for getting address info. */
	void set_protocol(int value){ m_addrinfo.ai_protocol = value; }

	/*! \details Accesses the flags. */
	int flags() const { return m_addrinfo.ai_flags; }
	/*! \details Accesses the family. */
	int family() const { return m_addrinfo.ai_family; }
	/*! \details Accesses the type. */
	int type() const { return m_addrinfo.ai_socktype; }
	/*! \details Accesses the protocol. */
	int protocol() const { return m_addrinfo.ai_protocol; }

	const var::ConstString & canon_name() const { return m_canon_name; }

	/*! \details Fetches the socket address information from
	 * DNS servers based on the node and service specified.
	 *
	 * ```
	 * #include <sapi/var.hpp>
	 * #include <sapi/inet.hpp>
	 *
	 * SocketAddressInfo address_info;
	 * Vector<SocketAddressInfo> address_info.fetch_node("stratifylabs.co"); //get IP address and other info for stratifylabs.co
	 * ```
	 */
	var::Vector<SocketAddressInfo> fetch_node(
			const var::ConstString & node
			){
		return fetch(
					arg::NodeToFetch(node),
					arg::ServiceToFetch("")
					);
	}

	var::Vector<SocketAddressInfo> fetch_service(const var::ConstString & service){
		return fetch(
					arg::NodeToFetch(""),
					arg::ServiceToFetch(service)
					);
	}

	var::Vector<SocketAddressInfo> fetch(
			const arg::NodeToFetch node,
			const arg::ServiceToFetch service);

private:
	friend class SocketAddress;
	struct addrinfo m_addrinfo;
	var::Data m_sockaddr;
	var::String m_canon_name;
};

class SocketAddressIpv4 : public api::InetInfoObject {
public:

	static in_addr_t address(u8 a, u8 b, u8 c, u8 d){
		return a<<24 | b<<16 | c<<8 | d;
	}

	SocketAddressIpv4(in_addr_t address,
							u16 port,
							int protocol = SocketAddressInfo::PROTOCOL_TCP,
							int type = SocketAddressInfo::TYPE_STREAM){
		m_sockaddr_in.sin_family = AF_INET;
#if !defined __win32
		m_sockaddr_in.sin_len = sizeof(struct sockaddr_in);
#endif
		m_sockaddr_in.sin_addr.s_addr = htonl(address);
		m_sockaddr_in.sin_port = htons(port);
		memset(m_sockaddr_in.sin_zero, 0, sizeof(m_sockaddr_in.sin_zero));
		m_protocol = protocol;
		m_type = type;
	}

	void set_address(u8 a, u8 b, u8 c, u8 d){
		m_sockaddr_in.sin_addr.s_addr = address(a,b,c,d);
	}

	int set_address(const var::ConstString & addr);

private:
	friend class SocketAddress;
	struct sockaddr_in m_sockaddr_in;
	int m_protocol;
	int m_type;
};

class SocketAddress : public api::InetInfoObject {
public:

	/*!
	  * \details Constructor to set the sockaddr structure to 0.
	  */
	SocketAddress(){
		m_protocol = 0;
		m_type = 0;
	}

	bool is_valid(){
		return m_sockaddr.size() > 0;
	}

	SocketAddress(const SocketAddressIpv4 & ipv4){
		m_sockaddr.copy_contents(
					arg::SourceData(
						var::DataReference(ipv4.m_sockaddr_in)
						)
					);
		m_protocol = ipv4.m_protocol;
		m_type = ipv4.m_type;
	}

	SocketAddress(const SocketAddressInfo & info, u16 port = 0){
		m_sockaddr = info.m_sockaddr;
		m_protocol = info.m_addrinfo.ai_protocol;
		m_canon_name = info.m_canon_name;
		m_type = info.m_addrinfo.ai_socktype;
		set_port(port);
	}

	SocketAddress(const sockaddr_in & ipv4,
					  int protocol = SocketAddressInfo::PROTOCOL_TCP,
					  int type = SocketAddressInfo::TYPE_STREAM){
		m_sockaddr.copy_contents(
					arg::SourceData(
						var::DataReference(ipv4)
						)
					);
		m_protocol = protocol;
		m_type = type;
	}

	SocketAddress(const sockaddr_in6 & ipv6){
		m_sockaddr.copy_contents(
					arg::SourceData(
						var::DataReference(ipv6)
						)
					);
	}

	void set_port(u16 port);

	u32 length() const { return m_sockaddr.size(); }

	int type() const { return m_type; }
	int protocol() const { return m_protocol; }

	void set_protocol(enum SocketAddressInfo::protocol value){ m_protocol = value; }
	void set_type(enum SocketAddressInfo::type value){ m_type = value; }

	u16 family() const {
		return m_sockaddr.to<const sockaddr>()->sa_family;
	}

	bool is_ipv4() const {
		return family() == SocketAddressInfo::FAMILY_INET;
	}

	bool is_ipv6() const {
		return family() == SocketAddressInfo::FAMILY_INET6;
	}

	u16 port() const;

	in_addr_t address_ipv4() const { return m_sockaddr.to<const sockaddr_in>()->sin_addr.s_addr; }
	var::String address_to_string() const;
	//struct in_addr6 address_ipv6() const { return m_sockaddr.to<const sockaddr_in6>()->sin6_addr.un; }

	const struct sockaddr * to_sockaddr() const {
		return m_sockaddr.to<struct sockaddr>();
	}

	const var::ConstString & canon_name() const { return m_canon_name; }

protected:
	friend class Socket;
	var::Data m_sockaddr;
	var::String m_canon_name;
	int m_protocol;
	int m_type;

};

class SocketOption : public api::InetInfoObject {
public:

	enum {
		LEVEL_SOCKET = SOL_SOCKET,
		LEVEL_IP = IPPROTO_IP,
		LEVEL_IPV6 = IPPROTO_IPV6,
		LEVEL_TCP = IPPROTO_TCP
	};

	SocketOption(int level = LEVEL_SOCKET){
		m_level = level;
	}

	enum {
		SOCKET_DEBUG = SO_DEBUG,
		SOCKET_BROADCAST = SO_BROADCAST,
		SOCKET_REUSE_ADDRESS = SO_REUSEADDR,
		SOCKET_REUSEADDR = SO_REUSEADDR,
#if !defined __win32
		SOCKET_REUSEPORT = SO_REUSEPORT,
		SOCKET_REUSE_PORT = SO_REUSEPORT,
#endif
		SOCKET_SET_SEND_SIZE = SO_SNDBUF,
		SOCKET_SNDBUF = SO_SNDBUF,
		SOCKET_SET_RECEIVE_SIZE = SO_RCVBUF,
		SOCKET_RCVBUF = SO_RCVBUF,
		SOCKET_DONT_ROUTE = SO_DONTROUTE,
		SOCKET_KEEP_ALIVE = SO_KEEPALIVE,
		SOCKET_LINGER = SO_LINGER,
		SOCKET_OOBINLINE = SO_OOBINLINE,
		SOCKET_RCVLOWAT = SO_RCVLOWAT,
		SOCKET_SET_RECEIVE_MINIMUM_SIZE = SO_RCVLOWAT,
		SOCKET_RECEIVE_TIMEOUT = SO_RCVTIMEO,
		SOCKET_RCVTIMEO = SO_RCVTIMEO,
		SOCKET_SNDLOWAT = SO_SNDLOWAT,
		SOCKET_SET_SEND_MINIMUM_SIZE = SO_SNDLOWAT,
		SOCKET_SEND_TIMEOUT = SO_SNDTIMEO,
		SOCKET_SNDTIMEO = SO_SNDTIMEO,

		IP_TYPE_OF_SERVICE = IP_TOS,
		IP_TIME_TO_LIVE = IP_TTL,
		IP_PACKET_INFO = IP_PKTINFO,

#if !defined __link
		TCP_NO_DELAY = TCP_NODELAY,
		TCP_KEEP_ALIVE = TCP_KEEPALIVE,
		TCP_KEEP_IDLE = TCP_KEEPIDLE,
		TCP_KEEP_INTERVAL = TCP_KEEPINTVL,
		TCP_KEEP_COUNT = TCP_KEEPCNT
#endif
	};

	SocketOption & socket_broadcast(bool value = true){
		m_level = LEVEL_SOCKET;
		return set_integer_value(SOCKET_BROADCAST, value);
	}

	SocketOption & socket_reuse_address(bool value = true){
		m_level = LEVEL_SOCKET;
		return set_integer_value(SOCKET_REUSE_ADDRESS, value);
	}

	SocketOption & socket_reuse_port(bool value = true){
#if !defined __win32
		m_level = LEVEL_SOCKET;
		return set_integer_value(SOCKET_REUSE_PORT, value);
#else
		//windows doesn't support this -- ignore it
		return *this;
#endif
	}

	SocketOption & socket_dont_route(bool value = true){
		m_level = LEVEL_SOCKET;
		return set_integer_value(SOCKET_DONT_ROUTE, value);
	}

	SocketOption & socket_keep_alive(bool value = true){
		m_level = LEVEL_SOCKET;
		return set_integer_value(SOCKET_KEEP_ALIVE, value);
	}

	SocketOption & socket_send_size(int value){
		m_level = LEVEL_SOCKET;
		return set_integer_value(SOCKET_SET_SEND_SIZE, value);
	}

	SocketOption & socket_send_minimum_size(int value){
		m_level = LEVEL_SOCKET;
		return set_integer_value(SOCKET_SET_SEND_MINIMUM_SIZE, value);
	}

	SocketOption & socket_receive_size(int value){
		m_level = LEVEL_SOCKET;
		return set_integer_value(SOCKET_SET_RECEIVE_SIZE, value);
	}

	SocketOption & socket_receive_minimum_size(int value){
		m_level = LEVEL_SOCKET;
		return set_integer_value(SOCKET_SET_RECEIVE_MINIMUM_SIZE, value);
	}

	SocketOption & socket_send_timeout(const chrono::ClockTime & timeout){
		m_level = LEVEL_SOCKET;
		return set_timeout(SOCKET_SEND_TIMEOUT, timeout);
	}

	SocketOption & socket_receive_timeout(const chrono::ClockTime & timeout){
		m_level = LEVEL_SOCKET;
		return set_timeout(SOCKET_RECEIVE_TIMEOUT, timeout);
	}

	SocketOption & ip_type_of_service(int service){
		m_level = LEVEL_IP;
		return set_integer_value(IP_TYPE_OF_SERVICE, service);
	}

	SocketOption & ip_time_to_live(int ttl){
		m_level = LEVEL_IP;
		return set_integer_value(IP_TIME_TO_LIVE, ttl);
	}

private:

	friend class Socket;
	SocketOption & set_integer_value(int name, int value){
		m_name = name;
		m_option_value.allocate(
					arg::Size(sizeof(int))
					);
		*m_option_value.to<int>() = value;
		return *this;
	}

	SocketOption & set_timeout(int name, const chrono::ClockTime & timeout){
		m_name = name;
		m_option_value.allocate(
					arg::Size(sizeof(struct timeval))
					);
		m_option_value.to<struct timeval>()->tv_sec = timeout.seconds();
		m_option_value.to<struct timeval>()->tv_usec = timeout.nanoseconds() / 1000UL;
		return *this;
	}

	int m_level;
	int m_name;
	var::Data m_option_value;

};


/*! \brief Socket Class
 * \details The Socket class is for accessing
 * websockets that run on the TCP/IP stack.
 *
 *
 * For a client, the typical exchange looks like this:
 *
 * ```msc
 * Note left of Client: Create Socket
 * Client->Server: connect()
 * Note right of Server: accept() and bind()
 * Client->Server: write()
 * Server->Client: read()
 * Client->Server: close()
 * ```
 *
 *
 */
class Socket : public fs::File {
public:
	Socket();
	~Socket();

	/*!
	  * \details Creates a new socket.
	  *
	  * @params attributes The Socket Attributes
	  *
	  * @return Zero on success
	  */
	virtual int create(const arg::SourceSocketAddress address);

	/*!
	  * \details Connects to the server using the SocketAddress
	  * object passed to create() method.
	  * @return Less than zero on error or zero on success
	  */
	virtual int connect(const arg::SourceSocketAddress address);


	/*!
	  * \details Binds and listens to the port for which the socket was created.
	  * @return Less than zero on error and zero on success
	  *
	  * This method will always bind but it will only listen
	  * when using TCP sockets where listen is applicable.
	  *
	  */
	virtual int bind_and_listen(
			const arg::SourceSocketAddress address,
			const arg::ListenBacklogCount backlog = arg::ListenBacklogCount(4)) const;


	virtual int bind(const arg::SourceSocketAddress address) const;

	/*!
	  * \details Accepts a socket connection on a socket that is listening.
	  *
	  * @return A valid Socket if the operation is successful.
	  */
	Socket accept(arg::DestinationSocketAddress address) const;

	/*! \details Shuts down the socket.
	 *
	 * @param how Use READWRITE, READONLY, or WRITEONLY to disable the specified operations
	 *
	 * \code
	 * Socket s;
	 * s.shutdown(); //disables further send/receive operations
	 * s.shutdown(Socket::READWRITE); //disables further send/receive operations
	 * s.shutdown(Socket::WRITEONLY); //disables further send operations
	 * s.shutdown(Socket::READONLY); //disables further receive operations
	 * \endcode
	 *
	 * @return Zero on success
	 */
	virtual int shutdown(const fs::OpenFlags how = fs::OpenFlags::read_write()) const;

	//already documented in fs::File
	using File::write;
	virtual int write(
			const arg::SourceBuffer buf,
			const arg::Size nbyte
			) const;

	//already documented in fs::File
	using File::read;
	virtual int read(
			arg::DestinationBuffer buf,
			const arg::Size nbyte
			) const;


	/*@brief use for get ip address from recved data in socket
	  * necceserly set ai_addrlen before use "recvfrom"
	  * getaddrinfo(use self port)->socket->bind-> read_from
	  * @param ai_addr - will address info accept
	  * @param ai_addrlen - write address ip len (IPv4 or IPv6) before use!!!
	  * */
	int read(
			arg::DestinationData data,
			arg::DestinationSocketAddress address
			);
	int read(
			arg::DestinationBuffer buf,
			const arg::Size nbyte,
			arg::DestinationSocketAddress address
			);

	int read(
			arg::DestinationBuffer buf,
			const arg::Size nbyte,
			struct sockaddr * ai_addr,
			socklen_t * ai_addrlen
			) const;

	/*! \brief Writes to the address specified.
	  *
	  * @param data The data to write
	  * @param address The address to write to.
	  *
	  * This method implements the socket call sendto().
	  *
	  */
	int write(
			const arg::SourceData data,
			const arg::SourceSocketAddress address
			){
		return write(
					arg::SourceBuffer(data.argument().to_const_void()),
					arg::Size(data.argument().size()),
					address);
	}
	int write(
			const arg::SourceBuffer buf,
			const arg::Size nbyte,
			const struct sockaddr * ai_addr,
			socklen_t ai_addrlen) const;
	int write(
			const arg::SourceBuffer buf,
			const arg::Size nbyte,
			const arg::SourceSocketAddress socket_address) const {
		return write(
					buf,
					nbyte,
					socket_address.argument().to_sockaddr(),
					socket_address.argument().length()
					);
	}

	//already documented in fs::File
	virtual int close();

	bool is_valid() const;

	/*! \details Sets options for the socket.
	 *
	 * @param option The option to set for the socket
	 *
	 * \code
	 *
	 * SocketAddressIpv4 address(0, 8080);
	 * Socket socket;
	 * socket.create(address);
	 * socket << SocketOption().set_reuse_address() << SocketOption().set_reuse_port();
	 * socket.bind(address);
	 *
	 * \endcode
	 *
	 */
	Socket & operator << (const SocketOption & option);


	static int initialize();
	static int deinitialize();

protected:

	/*! \cond */
	int decode_socket_return(int value) const;

#if defined __win32
	::SOCKET m_socket;
	enum {
		SOCKET_INVALID = INVALID_SOCKET
	};
#else
	enum {
		SOCKET_INVALID = -1
	};
	//socket on all other platforms is a file handler
	int m_socket;
#endif

private:
	static int m_is_initialized;
	/*! \endcond */

};

}

#endif // SAPISAPI_INET_SOCKET_HPP__SOCKET_HPP
