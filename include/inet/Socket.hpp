#ifndef SAPI_SOCKET_HPP
#define SAPI_SOCKET_HPP

#include <mcu/types.h>

#if defined __win32
#define _BSD_SOURCE
#include <winsock2.h>
#include <ws2tcpip.h>
typedef u32 in_addr_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#endif

#include "../api/InetObject.hpp"
#include "../sys/File.hpp"
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
 */
class SocketAddressInfo : public::api::InetInfoObject {
public:

	enum family {
		FAMILY_NONE,
		FAMILY_INET = AF_INET,
		FAMILY_INET6 = AF_INET6
	};

	enum type {
		TYPE_NONE,
		TYPE_RAW = SOCK_RAW,
		TYPE_STREAM = SOCK_STREAM,
		TYPE_DGRAM = SOCK_DGRAM
	};

	enum protocol {
		PROTOCOL_RAW = IPPROTO_RAW,
		PROTOCOL_TCP = IPPROTO_TCP,
		PROTOCOL_UDP = IPPROTO_UDP,
		PROTOCOL_ICMP = IPPROTO_ICMP,
		PROTOCOL_ICMP6 = IPPROTO_ICMPV6,
		PROTOCOL_ICMPV6 = IPPROTO_ICMPV6,
		PROTOCOL_IP = IPPROTO_IP
	};

	/*! \details Constructs a new socket address infomation object.
	 *
	 * @param family Socket family (defautl is FAMILY_INET)
	 * @param type Socket type (default is TYPE_STREAM)
	 * @param protocol Socket protocol (default is PROTOCOL_TCP)
	 * @param flags Socket flags (default is none)
	 *
	 */
	SocketAddressInfo(int family = FAMILY_INET, int type = TYPE_STREAM, int protocol = PROTOCOL_TCP, int flags = 0){
		set_flags(flags);
		set_family(family);
		set_type(type);
		set_protocol(protocol);
	}

	void set_flags(int value){ m_addrinfo.ai_flags = value; }
	void set_family(int value){ m_addrinfo.ai_family = value; }
	void set_type(int value){ m_addrinfo.ai_socktype = value; }
	void set_protocol(int value){ m_addrinfo.ai_protocol = value; }

	int flags() const { return m_addrinfo.ai_flags; }
	int family() const { return m_addrinfo.ai_family; }
	int type() const { return m_addrinfo.ai_socktype; }
	int protocol() const { return m_addrinfo.ai_protocol; }

	/*! \details Fetches the socket address information from
	 * DNS servers based on the node and service specified.
	 *
	 * \code
	 * #include <sapi/var.hpp>
	 * #include <sapi/inet.hpp>
	 *
	 * SocketAddressInfo address_info;
	 * Vector<SocketAddressInfo> address_info.fetch_node("stratifylabs.co"); //get IP address and other info for stratifylabs.co
	 *
	 */
	var::Vector<SocketAddressInfo> fetch_node(const var::ConstString & node){
		return fetch(node, "");
	}

	var::Vector<SocketAddressInfo> fetch_service(const var::ConstString & service){
		return fetch("", service);
	}

	var::Vector<SocketAddressInfo> fetch(
			const var::ConstString & node,
			const var::ConstString & service);

private:
	friend class SocketAddress;
	struct addrinfo m_addrinfo;
	var::Data m_sockaddr;
	var::String m_canon_name;
};

class SocketAddressIpv4 : public api::InetInfoObject {
public:

	static in_addr_t address(u8 a, u8 b, u8 c, u8 d){
		return a | b<<8 | c<<16 | d<<24;
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

	SocketAddress(const SocketAddressIpv4 & ipv4){
		m_sockaddr.copy_contents(var::Data((void*)&ipv4.m_sockaddr_in, sizeof(struct sockaddr_in)));
		m_protocol = ipv4.m_protocol;
		m_type = ipv4.m_type;
	}

	SocketAddress(const SocketAddressInfo & info, u16 port = 0){
		m_sockaddr = info.m_sockaddr;
		m_protocol = info.m_addrinfo.ai_protocol;
		m_type = info.m_addrinfo.ai_socktype;
		set_port(port);
	}

	SocketAddress(const sockaddr_in & ipv4,
					  int protocol = SocketAddressInfo::PROTOCOL_TCP,
					  int type = SocketAddressInfo::TYPE_STREAM){
		m_sockaddr.copy_contents(var::Data((void*)&ipv4, sizeof(ipv4)));
		m_protocol = protocol;
		m_type = type;
	}

	SocketAddress(const sockaddr_in6 & ipv6){
		m_sockaddr.copy_contents(var::Data((void*)&ipv6, sizeof(ipv6)));
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

protected:
	friend class Socket;
	var::Data m_sockaddr;
	int m_protocol;
	int m_type;

};

class SocketOption : public api::InetInfoObject {
public:

	enum {
		LEVEL_SOCKET = SOL_SOCKET,
	};

	SocketOption(int level = LEVEL_SOCKET){
		m_level = level;
	}

	enum {
		DEBUG = SO_DEBUG,
		BROADCAST = SO_BROADCAST,
		REUSE_ADDRESS = SO_REUSEADDR,
		REUSEADDR = SO_REUSEADDR,
#if !defined __win32
		REUSEPORT = SO_REUSEPORT,
		REUSE_PORT = SO_REUSEPORT,
#endif
		SET_SEND_SIZE = SO_SNDBUF,
		SNDBUF = SO_SNDBUF,
		SET_RECEIVE_SIZE = SO_RCVBUF,
		RCVBUF = SO_RCVBUF,
		DONT_ROUTE = SO_DONTROUTE,
		KEEP_ALIVE = SO_KEEPALIVE,
		LINGER = SO_LINGER,
		OOBINLINE = SO_OOBINLINE,
		RCVLOWAT = SO_RCVLOWAT,
		SET_RECEIVE_MINIMUM_SIZE = SO_RCVLOWAT,
		RECEIVE_TIMEOUT = SO_RCVTIMEO,
		RCVTIMEO = SO_RCVTIMEO,
		SNDLOWAT = SO_SNDLOWAT,
		SET_SEND_MINIMUM_SIZE = SO_SNDLOWAT,
		SEND_TIMEOUT = SO_SNDTIMEO,
		SNDTIMEO = SO_SNDTIMEO
	};

	SocketOption & broadcast(bool value = true){
		return set_integer_value(BROADCAST, value);
	}

	SocketOption & reuse_address(bool value = true){
		return set_integer_value(REUSE_ADDRESS, value);
	}

#if !defined __win32
	SocketOption & reuse_port(bool value = true){
		return set_integer_value(REUSE_PORT, value);
	}
#endif

	SocketOption & dont_route(bool value = true){
		return set_integer_value(DONT_ROUTE, value);
	}

	SocketOption & keep_alive(bool value = true){
		return set_integer_value(KEEP_ALIVE, value);
	}

	SocketOption & set_send_size(int value){
		return set_integer_value(SET_SEND_SIZE, value);
	}

	SocketOption & set_send_minimum_size(int value){
		return set_integer_value(SET_SEND_MINIMUM_SIZE, value);
	}

	SocketOption & set_receive_size(int value){
		return set_integer_value(SET_RECEIVE_SIZE, value);
	}

	SocketOption & set_receive_minimum_size(int value){
		return set_integer_value(SET_RECEIVE_MINIMUM_SIZE, value);
	}

	SocketOption & set_send_timeout(const chrono::ClockTime & timeout){
		return set_timeout(SEND_TIMEOUT, timeout);
	}

	SocketOption & set_receive_timeout(const chrono::ClockTime & timeout){
		return set_timeout(RECEIVE_TIMEOUT, timeout);
	}

private:

	friend class Socket;
	SocketOption & set_integer_value(int name, int value){
		m_name = name;
		m_option_value.alloc(sizeof(int));
		*m_option_value.to<int>() = value;
		return *this;
	}

	SocketOption & set_timeout(int name, const chrono::ClockTime & timeout){
		m_name = name;
		m_option_value.alloc(sizeof(struct timeval));
		m_option_value.to<struct timeval>()->tv_sec = timeout.seconds();
		m_option_value.to<struct timeval>()->tv_usec = timeout.nanoseconds() / 1000UL;
		return *this;
	}

	int m_level;
	int m_name;
	var::Data m_option_value;

};


class Socket : public sys::File {
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
	virtual int create(const SocketAddress & address);

	/*!
	  * \details Connects to the server using the SocketAddress
	  * object passed to create() method.
	  * @return Less than zero on error or zero on success
	  */
	virtual int connect(const SocketAddress & address);


	/*!
	  * \details Binds and listens to the port for which the socket was created.
	  * @return Less than zero on error and zero on success
	  *
	  * This method will always bind but it will only listen
	  * when using TCP sockets where listen is applicable.
	  *
	  */
	virtual int bind_and_listen(const SocketAddress & address, int backlog = 4) const;


	/*!
	  * \details Accepts a socket connection on a socket that is listening.
	  *
	  * @return A valid Socket if the operation is successful.
	  */
	Socket accept(SocketAddress & address) const;

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
	virtual int shutdown(int how = 0) const;

	//already documented in sys::File
	using File::write;
	virtual int write(const void * buf, int nbyte) const;

	//already documented in sys::File
	using File::read;
	virtual int read(void * buf, int nbyte) const;

	//already documented in sys::File
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

protected:

	/*!
	  * \details Initializes the system socket API.
	  *
	  * @return Zero on success
	  */
	int initialize();

	int decode_socket_return(int value) const;

#if defined __win32
	::SOCKET m_socket;
#else
	//socket on all other platforms is a file handler
	int m_socket;
#endif

private:
	static bool m_is_initialized;
};

}

#endif // SAPI_SOCKET_HPP
