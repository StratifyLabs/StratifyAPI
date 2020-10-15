/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef SAPI_INET_SOCKET_HPP_
#define SAPI_INET_SOCKET_HPP_

#include <mcu/types.h>

#include "IpAddress.hpp"
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
  using Family = Domain;

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


  typedef struct {
    size_t size;
    union {
      struct sockaddr sockaddr;
      struct sockaddr_in sockaddr_in;
      struct sockaddr_in6 sockaddr_in6;
    };
  } socketaddr_t;
};

class SocketAddress : public SocketFlags {
public:
  SocketAddress() = default;
  explicit SocketAddress(
    const socketaddr_t sockaddr,
    var::StringView canon_name)
    : m_sockaddr(sockaddr), m_canon_name(canon_name) {}

  const var::String &canon_name() const { return m_canon_name; }
  size_t length() const { return m_sockaddr.size; }
  bool is_valid() const { return m_sockaddr.size > 0; }

  const struct sockaddr *to_sockaddr() const { return &m_sockaddr.sockaddr; }

private:
  friend class Socket;
  friend class SocketAddress4;
  friend class SocketAddress6;
  socketaddr_t m_sockaddr = {0};
  var::String m_canon_name;
};

class SocketAddress4 : public SocketAddress {
public:
  /*!
   * \details Constructor to set the sockaddr structure to 0.
   */
  SocketAddress4() { m_sockaddr.size = sizeof(struct sockaddr_in); }

  // length, family, port, address

  /*! \details Sets the family for getting address info. */
  SocketAddress4 &set_family(Family value) {
    m_sockaddr.sockaddr_in.sin_family = static_cast<int>(value);
    return *this;
  }

  /*! \details Sets the protocol for getting address info. */
  SocketAddress4 &set_port(u16 port) {
    m_sockaddr.sockaddr_in.sin_port = htons(port);
    return *this;
  }

  SocketAddress4 &set_address(IpAddress4 address) {
    m_sockaddr.sockaddr_in.sin_addr.s_addr = address.address();
    return *this;
  }

  /*! \details Accesses the family. */
  Family family() const {
    return static_cast<Family>(m_sockaddr.sockaddr_in.sin_family);
  }

  u16 port() const { return ntohs(m_sockaddr.sockaddr_in.sin_port); }

protected:
};

class SocketAddress6 : public SocketAddress {
public:
  SocketAddress6() { m_sockaddr.size = sizeof(struct sockaddr_in6); }

  // length, family, port, address

  /*! \details Sets the family for getting address info. */
  SocketAddress6 &set_family(Family value) {
    m_sockaddr.sockaddr_in6.sin6_family = static_cast<int>(value);
    return *this;
  }

  /*! \details Sets the protocol for getting address info. */
  SocketAddress6 &set_port(u16 port) {
    m_sockaddr.sockaddr_in6.sin6_port = htons(port);
    return *this;
  }

  SocketAddress6 &set_address(IpAddress6 address) {
    var::View(m_sockaddr.sockaddr_in6.sin6_addr)
      .copy(var::View(address.address()));
    return *this;
  }

  /*! \details Accesses the family. */
  Family family() const {
    return static_cast<Family>(m_sockaddr.sockaddr_in6.sin6_family);
  }

  u16 port() const { return ntohs(m_sockaddr.sockaddr_in6.sin6_port); }
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
class SocketAddressInfo : public api::ExecutionContext, public SocketFlags {
public:
  /*! \details Constructs a new socket address infomation object.
   *
   * @param family Socket family (defautl is FAMILY_INET)
   * @param type Socket type (default is TYPE_STREAM)
   * @param protocol Socket protocol (default is PROTOCOL_TCP)
   * @param flags Socket flags (default is none)
   *
   */
  SocketAddressInfo() = default;

  bool is_valid() { return m_sockaddr.size > 0; }

  /*! \details Sets the flags used for getting address info.
   *
   */
  SocketAddressInfo &set_flags(int value) {
    m_addrinfo.ai_flags = value;
    return *this;
  }

  /*! \details Sets the family for getting address info. */
  SocketAddressInfo &set_family(Family value) {
    m_addrinfo.ai_family = static_cast<int>(value);
    return *this;
  }
  /*! \details Sets the type for getting address info. */
  SocketAddressInfo &set_type(Type value) {
    m_addrinfo.ai_socktype = static_cast<int>(value);
    return *this;
  }
  /*! \details Sets the protocol for getting address info. */
  SocketAddressInfo &set_protocol(Protocol value) {
    m_addrinfo.ai_protocol = static_cast<int>(value);
    return *this;
  }

  /*! \details Accesses the flags. */
  int flags() const { return m_addrinfo.ai_flags; }
  /*! \details Accesses the family. */
  Family family() const { return static_cast<Family>(m_addrinfo.ai_family); }
  /*! \details Accesses the type. */
  Type type() const { return static_cast<Type>(m_addrinfo.ai_socktype); }
  /*! \details Accesses the protocol. */
  Protocol protocol() const {
    return static_cast<Protocol>(m_addrinfo.ai_protocol);
  }

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
    API_AF(Fetch, u16, port, 0);
  };

  var::Vector<SocketAddressInfo> fetch(const Fetch &options);

  SocketAddress get_socket_address() {
    return std::move(SocketAddress(m_sockaddr, m_canon_name));
  }

private:
  friend class SocketAddress;
  struct addrinfo m_addrinfo = {0};
  socketaddr_t m_sockaddr = {0};
  var::String m_canon_name;
};

class SocketOption : public SocketFlags {
public:
  enum class Level {
    socket = SOL_SOCKET,
    ip = IPPROTO_IP,
    ipv6 = IPPROTO_IPV6,
    tcp = IPPROTO_TCP
  };

  explicit SocketOption(Level level) : m_level(level) {}

  enum class Name {
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
    m_level = Level::socket;
    return set_integer_value(Name::SOCKET_BROADCAST, value);
  }

  SocketOption &socket_reuse_address(bool value = true) {
    m_level = Level::socket;
    return set_integer_value(Name::SOCKET_REUSE_ADDRESS, value);
  }

  SocketOption &socket_reuse_port(bool value = true) {
#if !defined __win32
    m_level = Level::socket;
    return set_integer_value(Name::SOCKET_REUSE_PORT, value);
#else
    // windows doesn't support this -- ignore it
    return *this;
#endif
  }

  SocketOption &socket_dont_route(bool value = true) {
    m_level = Level::socket;
    return set_integer_value(Name::SOCKET_DONT_ROUTE, value);
  }

  SocketOption &socket_keep_alive(bool value = true) {
    m_level = Level::socket;
    return set_integer_value(Name::SOCKET_KEEP_ALIVE, value);
  }

  SocketOption &socket_send_size(int value) {
    m_level = Level::socket;
    return set_integer_value(Name::SOCKET_SET_SEND_SIZE, value);
  }

  SocketOption &socket_send_minimum_size(int value) {
    m_level = Level::socket;
    return set_integer_value(Name::SOCKET_SET_SEND_MINIMUM_SIZE, value);
  }

  SocketOption &socket_receive_size(int value) {
    m_level = Level::socket;
    return set_integer_value(Name::SOCKET_SET_RECEIVE_SIZE, value);
  }

  SocketOption &socket_receive_minimum_size(int value) {
    m_level = Level::socket;
    return set_integer_value(Name::SOCKET_SET_RECEIVE_MINIMUM_SIZE, value);
  }

  SocketOption &socket_send_timeout(const chrono::ClockTime &timeout) {
    m_level = Level::socket;
    return set_timeout(Name::SOCKET_SEND_TIMEOUT, timeout);
  }

  SocketOption &socket_receive_timeout(const chrono::ClockTime &timeout) {
    m_level = Level::socket;
    return set_timeout(Name::SOCKET_RECEIVE_TIMEOUT, timeout);
  }

  SocketOption &ip_type_of_service(int service) {
    m_level = Level::ip;
    return set_integer_value(Name::IP_TYPE_OF_SERVICE, service);
  }

  SocketOption &ip_time_to_live(int ttl) {
    m_level = Level::ip;
    return set_integer_value(Name::IP_TIME_TO_LIVE, ttl);
  }

private:
  friend class Socket;
  SocketOption &set_integer_value(Name name, int value) {
    m_name = name;
    m_value.integer = value;
    return *this;
  }

  SocketOption &set_timeout(Name name, const chrono::ClockTime &timeout) {
    m_name = name;
    m_value.timeval.tv_sec = timeout.seconds();
    m_value.timeval.tv_usec = timeout.nanoseconds() / 1000UL;
    m_size = sizeof(struct timeval);
    return *this;
  }

  Level m_level;
  Name m_name;
  union {
    struct timeval timeval;
    int integer;
  } m_value;

  size_t m_size;
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
  explicit Socket(
    Domain domain,
    Type type = Type::stream,
    Protocol protocol = Protocol::tcp);
  ~Socket();

  /*!
   * \details Connects to the server using the SocketAddress
   * object passed to create() method.
   * @return Less than zero on error or zero on success
   */
  Socket &connect(const SocketAddress &address);

  /*!
   * \details Binds and listens to the port for which the socket was created.
   * @return Less than zero on error and zero on success
   *
   * This method will always bind but it will only listen
   * when using TCP sockets where listen is applicable.
   *
   */
  const Socket &
  bind_and_listen(const SocketAddress &address, int backlog = 4) const;

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
  const Socket &
  shutdown(const fs::OpenMode how = fs::OpenMode::read_write()) const;

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

  virtual int interface_connect(const SocketAddress &address) const;

  virtual int
  interface_bind_and_listen(const SocketAddress &address, int backlog) const;

  virtual int interface_shutdown(SOCKET_T fd, const fs::OpenMode how) const;

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
