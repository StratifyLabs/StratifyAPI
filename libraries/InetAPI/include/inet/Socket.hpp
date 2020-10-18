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


class SocketFlags {
public:
  enum class Domain {
    unspecified = AF_UNSPEC,
    inet = AF_INET,
    inet6 = AF_INET6
  };
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

  enum class Level {
    socket = SOL_SOCKET,
    ip = IPPROTO_IP,
    ipv6 = IPPROTO_IPV6,
    tcp = IPPROTO_TCP
  };

  enum class AddressInfoFlags {
    null = 0,
    passive = AI_PASSIVE,
    canon_name = AI_CANONNAME,
    numeric_host = AI_NUMERICHOST,
    numeric_service = AI_NUMERICSERV
  };

  enum class MessageFlags {
    null = 0,
    peek = MSG_PEEK,
    out_of_band = MSG_OOB,
#if defined MSG_EOR
    end_of_record = MSG_EOR,
#endif
    truncated = MSG_TRUNC,
    control_truncatd = MSG_CTRUNC,
    wait_all = MSG_WAITALL,
    dont_wait = MSG_DONTWAIT,
#if defined MSG_DONTROUTE
    dont_route = MSG_DONTROUTE,
    end_of_file = MSG_EOF,
    wait_stream = MSG_WAITSTREAM,
    flush = MSG_FLUSH,
    hold = MSG_HOLD,
    send = MSG_SEND,
    have_more = MSG_HAVEMORE,
    receive_more = MSG_RCVMORE,
    need_socket_address = MSG_NEEDSA
#endif

  };

  enum class NameFlags {
    socket_debug = SO_DEBUG,
    socket_broadcast = SO_BROADCAST,
    socket_reuse_address = SO_REUSEADDR,
#if !defined __win32
    socket_reuse_port = SO_REUSEPORT,
#endif
    socket_set_send_size = SO_SNDBUF,
    socket_set_receive_size = SO_RCVBUF,
    socket_dont_route = SO_DONTROUTE,
    socket_keep_alive = SO_KEEPALIVE,
    socket_linger = SO_LINGER,
    socket_oobinline = SO_OOBINLINE,
    socket_set_receive_minimum_size = SO_RCVLOWAT,
    socket_receive_timeout = SO_RCVTIMEO,
    socket_set_send_minimum_size = SO_SNDLOWAT,
    socket_send_timeout = SO_SNDTIMEO,

    ip_type_of_service = IP_TOS,
    ip_time_to_live = IP_TTL,
    ip_packet_info = IP_PKTINFO,

#if !defined __link
    tcp_no_delay = TCP_NODELAY,
    tcp_keep_alive = TCP_KEEPALIVE,
    tcp_keep_idle = TCP_KEEPIDLE,
    tcp_keep_interval = TCP_KEEPINTVL,
    tcp_keep_count = TCP_KEEPCNT
#endif
  };

  typedef struct {
    size_t size;
    union {
      struct sockaddr sockaddr;
      struct sockaddr_in sockaddr_in;
      struct sockaddr_in6 sockaddr_in6;
    };
  } socket_address_union_t;
};

API_OR_NAMED_FLAGS_OPERATOR(SocketFlags, NameFlags)
API_OR_NAMED_FLAGS_OPERATOR(SocketFlags, AddressInfoFlags)
API_OR_NAMED_FLAGS_OPERATOR(SocketFlags, MessageFlags)

class SocketAddress : public SocketFlags {
public:
  SocketAddress() = default;
  explicit SocketAddress(
    const socket_address_union_t sockaddr,
    var::StringView canon_name = "")
    : m_sockaddr(sockaddr), m_canon_name(canon_name) {}

  const var::String &canon_name() const { return m_canon_name; }
  size_t length() const { return m_sockaddr.size; }
  bool is_valid() const { return m_sockaddr.size > 0; }

  /*! \details Accesses the family. */
  Family family() const {
    if (m_sockaddr.size == sizeof(struct sockaddr_in)) {
      return Family::inet;
    }

    return Family::inet6;
  }

  u16 port() const {
    if (m_sockaddr.size == sizeof(struct sockaddr_in)) {
      return ntohs(m_sockaddr.sockaddr_in.sin_port);
    }
    return ntohs(m_sockaddr.sockaddr_in6.sin6_port);
  }

  var::String get_address_string() const {
    if (m_sockaddr.size == sizeof(struct sockaddr_in)) {
      return std::move(
        IpAddress4(m_sockaddr.sockaddr_in.sin_addr.s_addr).to_string());
    }
    return std::move(IpAddress6(m_sockaddr.sockaddr_in6.sin6_addr).to_string());
  }

  const struct sockaddr *to_sockaddr() const { return &m_sockaddr.sockaddr; }

private:
  friend class Socket;
  friend class SocketAddress4;
  friend class SocketAddress6;
  friend class AddressInfo;
  socket_address_union_t m_sockaddr = {0};
  var::String m_canon_name;
  API_AF(SocketAddress, Type, type, Type::raw);
  API_AF(SocketAddress, Protocol, protocol, Protocol::raw);

  explicit SocketAddress(
    const void *addr,
    size_t length,
    const char *canon_name) {
    memcpy(&m_sockaddr.sockaddr, addr, length);
    m_sockaddr.size = length;
    if (canon_name != nullptr) {
      m_canon_name = canon_name;
    }
  }
};

class SocketAddress4 : public SocketAddress {
public:
  SocketAddress4() {
    m_sockaddr.size = sizeof(struct sockaddr_in);
    m_sockaddr.sockaddr_in.sin_family = AF_INET;
  }

  SocketAddress4 &set_port(u16 port) {
    m_sockaddr.sockaddr_in.sin_port = htons(port);
    return *this;
  }

  SocketAddress4 &set_address(IpAddress4 address) {
    m_sockaddr.sockaddr_in.sin_addr.s_addr = address.address();
    return *this;
  }

protected:
};

class SocketAddress6 : public SocketAddress {
public:
  SocketAddress6() { m_sockaddr.size = sizeof(struct sockaddr_in6); }


  SocketAddress6 &set_family(Family value) {
    m_sockaddr.sockaddr_in6.sin6_family = static_cast<int>(value);
    return *this;
  }

  SocketAddress6 &set_port(u16 port) {
    m_sockaddr.sockaddr_in6.sin6_port = htons(port);
    return *this;
  }

  SocketAddress6 &set_address(IpAddress6 address) {
    var::View(m_sockaddr.sockaddr_in6.sin6_addr)
      .copy(var::View(address.address()));
    return *this;
  }
};

class AddressInfo : public api::ExecutionContext, public SocketFlags {
public:
  using Flags = AddressInfoFlags;

  class Construct {
    API_AC(Construct, var::StringView, node);
    API_AC(Construct, var::StringView, service);
    API_AF(Construct, Family, family, Family::unspecified);
    API_AF(Construct, Type, type, Type::stream);
    API_AF(Construct, Protocol, protocol, Protocol::ip);
    API_AF(Construct, Flags, flags, Flags::null);
  };

  AddressInfo(const Construct &options);

private:
  API_RAC(AddressInfo, var::Vector<SocketAddress>, list);
};

class SocketOption : public SocketFlags {
public:
  SocketOption(Level level, NameFlags name, int value = 0)
    : m_level(level), m_name(name) {
    m_size = sizeof(value);
  }

  SocketOption &set_value(const chrono::ClockTime &timeout) {
    m_value.timeval.tv_sec = timeout.seconds();
    m_value.timeval.tv_usec = timeout.nanoseconds() / 1000UL;
    m_size = sizeof(struct timeval);
    return *this;
  }

private:
  friend class Socket;

  Level m_level;
  NameFlags m_name;
  union {
    struct timeval timeval;
    int integer;
  } m_value;

  size_t m_size;
};

class Socket : public fs::FileAccess<Socket>, public SocketFlags {
public:
  Socket();

  explicit Socket(
    Domain domain,
    Type type = Type::stream,
    Protocol protocol = Protocol::ip);

  explicit Socket(const SocketAddress &socket_address);

  Socket(Socket &&socket) { std::swap(m_socket, socket.m_socket); }
  Socket &operator=(Socket &&socket) {
    std::swap(m_socket, socket.m_socket);
    return *this;
  }

  virtual ~Socket();

  const Socket &connect(const SocketAddress &address) const;
  Socket &connect(const SocketAddress &address) {
    return API_CONST_CAST_SELF(Socket, connect, address);
  }

  const Socket &
  bind_and_listen(const SocketAddress &address, int backlog = 4) const;
  Socket &bind_and_listen(const SocketAddress &address, int backlog = 4) {
    return API_CONST_CAST_SELF(Socket, bind_and_listen, address, backlog);
  }

  const Socket &bind(const SocketAddress &address) const;
  Socket &bind(const SocketAddress &address) {
    return API_CONST_CAST_SELF(Socket, bind, address);
  }

  Socket accept(SocketAddress &address) const;
  Socket accept(SocketAddress &address) {
    return API_CONST_CAST(Socket, accept, address);
  }

  const Socket &
  shutdown(const fs::OpenMode how = fs::OpenMode::read_write()) const;

  Socket &shutdown(const fs::OpenMode how = fs::OpenMode::read_write()) {
    return API_CONST_CAST_SELF(Socket, shutdown, how);
  }

  SocketAddress receive_from(var::View data) const {
    return receive_from(data.to_void(), data.size());
  }

  SocketAddress receive_from(void *buf, int nbyte) const;

  const Socket &send_to(
    const SocketAddress &socket_address,
    const void *buf,
    int nbyte) const;

  Socket &
  send_to(const SocketAddress &socket_address, const void *buf, int nbyte) {
    return API_CONST_CAST_SELF(Socket, send_to, socket_address, buf, nbyte);
  }

  const Socket &
  send_to(const SocketAddress &socket_address, var::View data) const {
    return send_to(socket_address, data.to_const_void(), data.size());
  }

  Socket &send_to(const SocketAddress &socket_address, var::View data) {
    return send_to(socket_address, data.to_const_void(), data.size());
  }

  const Socket &set_option(const SocketOption &option) const;
  Socket &set_option(const SocketOption &option) {
    return API_CONST_CAST_SELF(Socket, set_option, option);
  }

  SocketAddress get_sock_name() const;

  static int initialize();
  static int finalize();

protected:
#if defined __win32
  enum {SOCKET_INVALID = INVALID_SOCKET};
#else
  enum { SOCKET_INVALID = -1 };
  // socket on all other platforms is a file handler
#endif


  virtual int interface_connect(const SocketAddress &address) const;

  virtual int
  interface_bind_and_listen(const SocketAddress &address, int backlog) const;

  virtual int interface_shutdown(const fs::OpenMode how) const;

  int interface_open(const char *path, int flags, int mode) const { return 0; }
  int interface_close(int fd) const;

  int interface_lseek(int location, int whence) const override final {
    return 0;
  }

  int interface_ioctl(int request, void *argument) const override final {
    return fake_ioctl(request, argument);
  }

  virtual int interface_read(void *buf, int nbyte) const override;
  virtual int
  interface_write(const void *buf, int nbyte) const override;

  int decode_socket_return(int value) const;

private:
  mutable SOCKET_T m_socket = SOCKET_INVALID;
  static int m_is_initialized;
  API_AF(Socket, MessageFlags, message_flags, MessageFlags::null);
  API_AF(Socket, Family, family, Family::unspecified);
};

} // namespace inet

namespace printer {
class Printer;
Printer &operator<<(Printer &printer, const inet::SocketAddress &a);
class Printer;
Printer &operator<<(Printer &printer, const inet::AddressInfo &a);
} // namespace printer

#endif // SAPISAPI_INET_SOCKET_HPP__SOCKET_HPP
