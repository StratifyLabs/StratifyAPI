/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef SAPI_INET_SOCKET_HPP_
#define SAPI_INET_SOCKET_HPP_

#if defined __win32
#define _BSD_SOURCE
#include <stdint.h>
#include <winsock2.h>
#include <ws2tcpip.h>
typedef uint32_t in_addr_t;

#define SOCKET_T ::SOCKET

#else
#define SOCKET_T int
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include <mcu/types.h>

#include "chrono/ClockTime.hpp"
#include "chrono/DateTime.hpp"
#include "fs/File.hpp"
#include "var/String.hpp"
#include "var/Vector.hpp"

namespace inet {

class Socket;
class SocketAddress;

class SocketFlags {
public:
  enum class Domain { inet = AF_INET, inet6 = AF_INET6 };

  enum class Type {
    raw /*! Raw socket data */ = SOCK_RAW,
    stream /*! Streaming socket data */ = SOCK_STREAM,
    datagram /*! Datagram socket data */ = SOCK_DGRAM
  };

  enum class Protocol {
    raw /*! Raw protocol */ = IPPROTO_RAW,
    tcp /*! TCP Protocol */ = IPPROTO_TCP,
    udp /*! UDP Procotol */ = IPPROTO_UDP,
    icmp /*! ICMP Procotol */ = IPPROTO_ICMP,
    icmpv6 = IPPROTO_ICMPV6,
    ip /*! IP Protocol */ = IPPROTO_IP,
  };

  /*! \details Enumerates the socket address family options. */
  enum family {
    family_none = 0,
    family_inet /*! IPV4 */ = AF_INET,
    family_inet6 /*! IPV6 */ = AF_INET6
  };

  /*! \details Enumerates the socket address type options. */
  enum type {
    type_none = 0,
    type_raw /*! Raw socket data */ = SOCK_RAW,
    type_stream /*! Streaming socket data */ = SOCK_STREAM,
    type_dgram /*! Datagram socket data */ = SOCK_DGRAM
  };

  /*! \details Enumerates the socket address protocol options. */
  enum protocol {
    protocol_raw /*! Raw protocol */ = IPPROTO_RAW,
    protocol_tcp /*! TCP Protocol */ = IPPROTO_TCP,
    protocol_udp /*! UDP Procotol */ = IPPROTO_UDP,
    protocol_icmp /*! ICMP Procotol */ = IPPROTO_ICMP,
    protocol_icmpv6 = IPPROTO_ICMPV6,
    protocol_ip /*! IP Protocol */ = IPPROTO_IP,
  };

  typedef struct {
    size_t size;
    union {
      struct sockaddr sockaddr;
      struct sockaddr_in sockaddr_in;
      struct sockaddr_in6 sockaddr_in6;
    };
  } socketaddr_t;
};

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
 * Vector<SocketAddressInfo> result =
 *socket_address_info.fetch_node("stratifylabs.co");
 *
 * if( result.count() > 0 ){
 *	 SocketAddress socket_address(result.at(0));
 *  Socket socket;
 *  socket.create(socket_address); //create a socket that can connect to
 *stratifylabs.co
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
class SocketAddressInfo : public api::Object, public SocketFlags {
public:
  /*! \details Constructs a new socket address infomation object.
   *
   * @param family Socket family (defautl is FAMILY_INET)
   * @param type Socket type (default is TYPE_STREAM)
   * @param protocol Socket protocol (default is PROTOCOL_TCP)
   * @param flags Socket flags (default is none)
   *
   */
  SocketAddressInfo(
    int family = family_inet,
    int type = type_stream,
    int protocol = protocol_tcp,
    int flags = 0);

  bool is_valid() { return m_sockaddr.size > 0; }

  /*! \details Sets the flags used for getting address info.
   *
   */
  void set_flags(int value) { m_addrinfo.ai_flags = value; }

  /*! \details Sets the family for getting address info. */
  void set_family(int value) { m_addrinfo.ai_family = value; }
  /*! \details Sets the type for getting address info. */
  void set_type(int value) { m_addrinfo.ai_socktype = value; }
  /*! \details Sets the protocol for getting address info. */
  void set_protocol(int value) { m_addrinfo.ai_protocol = value; }

  /*! \details Accesses the flags. */
  int flags() const { return m_addrinfo.ai_flags; }
  /*! \details Accesses the family. */
  int family() const { return m_addrinfo.ai_family; }
  /*! \details Accesses the type. */
  int type() const { return m_addrinfo.ai_socktype; }
  /*! \details Accesses the protocol. */
  int protocol() const { return m_addrinfo.ai_protocol; }

  const var::String &canon_name() const { return m_canon_name; }

  /*! \details Fetches the socket address information from
   * DNS servers based on the node and service specified.
   *
   * ```
   * #include <sapi/var.hpp>
   * #include <sapi/inet.hpp>
   *
   * SocketAddressInfo address_info;
   * Vector<SocketAddressInfo> address_info.fetch_node("stratifylabs.co"); //get
   * IP address and other info for stratifylabs.co
   * ```
   */

  class Fetch {
    API_AC(Fetch, var::StringView, node);
    API_AC(Fetch, var::StringView, service);
  };

  var::Vector<SocketAddressInfo> fetch(const Fetch &options);

private:
  friend class SocketAddress;
  struct addrinfo m_addrinfo = {0};

  socketaddr_t m_sockaddr = {0};
  var::String m_canon_name;
};

class SocketAddressIpv4 {
public:
  static in_addr_t address(u8 a, u8 b, u8 c, u8 d) {
    return a << 24 | b << 16 | c << 8 | d;
  }

  SocketAddressIpv4() {
    m_sockaddr_in = {0};
    m_protocol = SocketAddressInfo::protocol_tcp;
    m_type = SocketAddressInfo::type_stream;
  }

  SocketAddressIpv4(
    in_addr_t address,
    u16 port,
    int protocol = SocketAddressInfo::protocol_tcp,
    int type = SocketAddressInfo::type_stream) {
    m_sockaddr_in.sin_family = AF_INET;
#if !defined __win32 && !defined __linux
    m_sockaddr_in.sin_len = sizeof(struct sockaddr_in);
#endif
    m_sockaddr_in.sin_addr.s_addr = htonl(address);
    m_sockaddr_in.sin_port = htons(port);
    memset(m_sockaddr_in.sin_zero, 0, sizeof(m_sockaddr_in.sin_zero));
    m_protocol = protocol;
    m_type = type;
  }

  static SocketAddressIpv4 from_string(var::StringView value);

  SocketAddressIpv4 &set_address(u8 a, u8 b, u8 c, u8 d) {
    m_sockaddr_in.sin_addr.s_addr = address(a, b, c, d);
    return *this;
  }

  SocketAddressIpv4 &set_address(var::StringView addr);

private:
  friend class SocketAddress;
  struct sockaddr_in m_sockaddr_in;
  int m_protocol;
  int m_type;
};

class SocketAddress : public SocketFlags {
public:
  /*!
   * \details Constructor to set the sockaddr structure to 0.
   */
  SocketAddress() {

  }

  bool is_valid() const { return m_sockaddr.size > 0; }

  explicit SocketAddress(const SocketAddressIpv4 &ipv4) {
    m_sockaddr.sockaddr_in = ipv4.m_sockaddr_in;
  }

  explicit SocketAddress(const SocketAddressInfo &info, u16 port = 0) {
    m_sockaddr = info.m_sockaddr;
    m_canon_name = info.m_canon_name;
    set_port(port);
  }

  static SocketAddress any(u16 port) {
    return SocketAddress(SocketAddressIpv4(INADDR_ANY, port));
  }

  explicit SocketAddress(const sockaddr_in &ipv4) {
    m_sockaddr.sockaddr_in = ipv4;
    m_sockaddr.size = sizeof(ipv4);
  }

  explicit SocketAddress(const sockaddr_in6 &ipv6) {
    m_sockaddr.sockaddr_in6 = ipv6;
    m_sockaddr.size = sizeof(ipv6);
  }

  SocketAddress &set_port(u16 port);

  u32 length() const { return m_sockaddr.size; }

  u16 family() const {
    return var::View(m_sockaddr).to<const sockaddr>()->sa_family;
  }

  bool is_ipv4() const { return family() == SocketAddressInfo::family_inet; }

  bool is_ipv6() const { return family() == SocketAddressInfo::family_inet6; }

  u16 port() const;

  in_addr_t address_ipv4() const {
    return var::View(m_sockaddr).to<const sockaddr_in>()->sin_addr.s_addr;
  }

  var::String to_string() const;
  var::String address_to_string() const { return to_string(); }
  // struct in_addr6 address_ipv6() const { return m_sockaddr.to<const
  // sockaddr_in6>()->sin6_addr.un; }

  const struct sockaddr *to_sockaddr() const { return &m_sockaddr.sockaddr; }

  const var::String &canon_name() const { return m_canon_name; }

protected:
  friend class Socket;
  socketaddr_t m_sockaddr;
  var::String m_canon_name;
};

class SocketOption : public SocketFlags {
public:
  enum levels {
    // uppercase is deprecated
    LEVEL_SOCKET = SOL_SOCKET,
    LEVEL_IP = IPPROTO_IP,
    LEVEL_IPV6 = IPPROTO_IPV6,
    LEVEL_TCP = IPPROTO_TCP,

    level_socket = SOL_SOCKET,
    level_ip = IPPROTO_IP,
    level_ipv6 = IPPROTO_IPV6,
    level_tcp = IPPROTO_TCP
  };

  explicit SocketOption(int level = level_socket) : m_level(level) {}

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

  SocketOption &socket_broadcast(bool value = true) {
    m_level = level_socket;
    return set_integer_value(SOCKET_BROADCAST, value);
  }

  SocketOption &socket_reuse_address(bool value = true) {
    m_level = level_socket;
    return set_integer_value(SOCKET_REUSE_ADDRESS, value);
  }

  SocketOption &socket_reuse_port(bool value = true) {
#if !defined __win32
    m_level = level_socket;
    return set_integer_value(SOCKET_REUSE_PORT, value);
#else
    // windows doesn't support this -- ignore it
    return *this;
#endif
  }

  SocketOption &socket_dont_route(bool value = true) {
    m_level = level_socket;
    return set_integer_value(SOCKET_DONT_ROUTE, value);
  }

  SocketOption &socket_keep_alive(bool value = true) {
    m_level = level_socket;
    return set_integer_value(SOCKET_KEEP_ALIVE, value);
  }

  SocketOption &socket_send_size(int value) {
    m_level = level_socket;
    return set_integer_value(SOCKET_SET_SEND_SIZE, value);
  }

  SocketOption &socket_send_minimum_size(int value) {
    m_level = level_socket;
    return set_integer_value(SOCKET_SET_SEND_MINIMUM_SIZE, value);
  }

  SocketOption &socket_receive_size(int value) {
    m_level = level_socket;
    return set_integer_value(SOCKET_SET_RECEIVE_SIZE, value);
  }

  SocketOption &socket_receive_minimum_size(int value) {
    m_level = level_socket;
    return set_integer_value(SOCKET_SET_RECEIVE_MINIMUM_SIZE, value);
  }

  SocketOption &socket_send_timeout(const chrono::ClockTime &timeout) {
    m_level = level_socket;
    return set_timeout(SOCKET_SEND_TIMEOUT, timeout);
  }

  SocketOption &socket_receive_timeout(const chrono::ClockTime &timeout) {
    m_level = level_socket;
    return set_timeout(SOCKET_RECEIVE_TIMEOUT, timeout);
  }

  SocketOption &ip_type_of_service(int service) {
    m_level = level_ip;
    return set_integer_value(IP_TYPE_OF_SERVICE, service);
  }

  SocketOption &ip_time_to_live(int ttl) {
    m_level = level_ip;
    return set_integer_value(IP_TIME_TO_LIVE, ttl);
  }

private:
  friend class Socket;
  SocketOption &set_integer_value(int name, int value) {
    m_name = name;
    m_option_value.resize(sizeof(int));
    var::View(m_option_value).to<int>()[0] = value;
    return *this;
  }

  SocketOption &set_timeout(int name, const chrono::ClockTime &timeout) {
    m_name = name;
    m_option_value.resize(sizeof(struct timeval));
    var::View(m_option_value).to<struct timeval>()->tv_sec = timeout.seconds();
    var::View(m_option_value).to<struct timeval>()->tv_usec
      = timeout.nanoseconds() / 1000UL;
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
class Socket : public fs::FileAccess<Socket>, public SocketFlags {
public:
  Socket(Domain domain, Type type, Protocol protocol);
  ~Socket();

  /*!
   * \details Connects to the server using the SocketAddress
   * object passed to create() method.
   * @return Less than zero on error or zero on success
   */
  virtual Socket &connect(const SocketAddress &address);

  /*!
   * \details Binds and listens to the port for which the socket was created.
   * @return Less than zero on error and zero on success
   *
   * This method will always bind but it will only listen
   * when using TCP sockets where listen is applicable.
   *
   */
  Socket &bind_and_listen(const SocketAddress &address, int backlog = 4) const;

  /*!
   * \details Accepts a socket connection on a socket that is listening.
   *
   * @return A valid Socket if the operation is successful.
   */
  Socket accept(SocketAddress &address) const;

  /*! \details Shuts down the socket.
   *
   * @param how Use READWRITE, READONLY, or WRITEONLY to disable the specified
   * operations
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
  virtual int
  shutdown(const fs::OpenMode how = fs::OpenMode::read_write()) const;

  using File::read;
  /*@brief use for get ip address from recved data in socket
   * necceserly set ai_addrlen before use "recvfrom"
   * getaddrinfo(use self port)->socket->bind-> read_from
   * @param ai_addr - will address info accept
   * @param ai_addrlen - write address ip len (IPv4 or IPv6) before use!!!
   * */
  const Socket &
  receive_from(const SocketAddress &address, var::View data) const {
    return receive_from(address, data.to_void(), data.size());
  }

  const Socket &
  receive_from(const SocketAddress &address, void *buf, int nbyte) const;

  /*! \brief Writes to the address specified.
   *
   * @param data The data to write
   * @param address The address to write to.
   *
   * This method implements the socket call sendto().
   *
   */
  const Socket &
  send_to(const SocketAddress &socket_address, var::View data) const {
    return send_to(socket_address, data.to_const_void(), data.size());
  }

  const Socket &send_to(
    const SocketAddress &socket_address,
    const void *buf,
    int nbyte) const;

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
   * socket << SocketOption().set_reuse_address() <<
   * SocketOption().set_reuse_port(); socket.bind(address);
   *
   * \endcode
   *
   */
  Socket &set_option(const SocketOption &option);

  static int initialize();
  static int deinitialize();

protected:
#if defined __win32
  enum {SOCKET_INVALID = INVALID_SOCKET};
#else
  enum { SOCKET_INVALID = -1 };
  // socket on all other platforms is a file handler
#endif

  SOCKET_T m_socket;

  Socket();

  int bind(const SocketAddress &address) const;

  virtual int
  interface_bind_and_listen(const SocketAddress &address, int backlog) const;

  int interface_open(const char *path, int flags, int mode)
    const override final {
    return 0;
  }
  int interface_lseek(int fd, int location, int whence) const override final {
    return 0;
  }
  int interface_fsync(int fd) const override final { return 0; }
  int interface_ioctl(int fd, int request, void *argument)
    const override final {
    return 0;
  }

  virtual int interface_close(int fd) const override;
  virtual int interface_read(int fd, void *buf, int nbyte) const override;
  virtual int
  interface_write(int fd, const void *buf, int nbyte) const override;

  /*! \cond */
  int decode_socket_return(int value) const;

private:
  static int m_is_initialized;
  /*! \endcond */
};

} // namespace inet

#endif // SAPISAPI_INET_SOCKET_HPP__SOCKET_HPP
