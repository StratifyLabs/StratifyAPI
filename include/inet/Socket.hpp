#ifndef SAPI_SOCKET_HPP
#define SAPI_SOCKET_HPP

#if defined __win32
#define _BSD_SOURCE
#include <winsock2.h>
#include <ws2tcpip.h>
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

	SocketAddressInfo(int flags = 0, int family = 0, int socktype = 0, int protocol = 0);

	void set_flags(int value){ m_addrinfo.ai_flags = value; }
	void set_family(int value){ m_addrinfo.ai_family = value; }
	void set_socktype(int value){ m_addrinfo.ai_socktype = value; }
	void set_protocol(int value){ m_addrinfo.ai_protocol = value; }

	int flags() const { return m_addrinfo.ai_flags; }
	int family() const { return m_addrinfo.ai_family; }
	int type() const { return m_addrinfo.ai_socktype; }
	int protocol() const { return m_addrinfo.ai_protocol; }

	var::Vector<SocketAddressInfo> get_address_info(
			const var::ConstString & node,
			const var::ConstString & server);

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
		m_sockaddr_in.sin_len = sizeof(struct sockaddr_in);
		m_sockaddr_in.sin_addr.s_addr = address;
		m_sockaddr_in.sin_port = htons(port);
		memset(m_sockaddr_in.sin_zero, 0, sizeof(m_sockaddr_in.sin_zero));
		m_protocol = protocol;
		m_type = type;
	}

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
	SocketAddress();
	SocketAddress(const SocketAddressIpv4 & ipv4){
		m_sockaddr.copy_contents(var::Data((void*)&ipv4.m_sockaddr_in, sizeof(struct sockaddr_in)));
		m_protocol = ipv4.m_protocol;
		m_type = ipv4.m_type;
	}

	SocketAddress(const SocketAddressInfo & info){
		m_sockaddr = info.m_sockaddr;
		m_protocol = info.m_addrinfo.ai_protocol;
		m_type = info.m_addrinfo.ai_socktype;
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
	int create(const SocketAddress & address);

	/*!
	  * \details Connects to the server using the SocketAddress
	  * object passed to create() method.
	  * @return Less than zero on error or zero on success
	  */
	int connect();

	/*!
	  * \details Binds to the port for which the socket was created.
	  * @return Less than zero on error and zero on success
	  */
	int bind(const Socket & socket, const SocketAddress & address);

	/*!
	  * \details Listens on the port specified during create().
	  *
	  * @param backlog Limit the number of outstanding connections in the socket's listen queue
	  * @return Less than zero on error or zero on success
	  */
	int listen(int backlog = 4);

	/*!
	  * \details Accepts a socket connection on a socket that is listening.
	  *
	  * @return A valid Socket if the operation is successful.
	  */
	Socket accept();

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
	int shutdown(int how = 0);

	//already documented in sys::File
	int write(const void * buf, int nbyte);

	//already documented in sys::File
	int read(void * buf, int nbyte);

	//already documented in sys::File
	int close();

	bool is_valid() const;

private:
	SocketAddress m_address;


	/*!
	  * \details Initializes the system socket API.
	  *
	  * @return Zero on success
	  */
	int initialize();

	static int decode_socket_return(int value);

#if defined __win32
	::SOCKET m_socket;
#else
	//socket on all other platforms is a file handler
	int m_socket;
#endif

	static bool m_is_initialized;
};


/*
 * \tg The socket API above will eventually
 * be the same as the one below.  The only
 * place we should really need to say
 * #if defined __win32 is for declaring
 * the ::SOCKET member variable m_socket.
 *
 *
 */
//class Socket : public sys::File {
//public:
//    Socket();
//    ~Socket();

//    int init();

//    /*! \details Opens a new socket. */
//    int create(const SocketAttributes & attributes); //calls ::socket() - calls File::set_fileno();

//    int connect(); //calls ::connect()
//    int listen(); //calls ::listen()
//    Socket accept(); //calls ::accept(m_socket); and returns a new Socket
//    int bind(); //calls ::bind()

//    //read and write are inherited from sys::File and will work without any additional methods

//private:

//#if defined __win32
//    ::SOCKET m_socket;
//#endif

//};
//#endif

}

#endif // SAPI_SOCKET_HPP
