/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#include "inet/Socket.hpp"
#include "var.hpp"

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

var::Vector<SocketAddressInfo> SocketAddressInfo::fetch(const Fetch &options) {
  var::Vector<SocketAddressInfo> result;
  int result_int;

  struct addrinfo *info_start;
  struct addrinfo *info;

  const char *server_cstring;
  const char *node_cstring;

  server_cstring
    = options.service().is_empty() ? nullptr : options.service().cstring();

  node_cstring = options.node().is_empty() ? nullptr : options.node().cstring();

  Socket::initialize();

  m_addrinfo.ai_flags |= AI_CANONNAME;

  if (
    (result_int = getaddrinfo(node_cstring, server_cstring, &m_addrinfo, &info))
    != 0) {
    return result;
  }

  info_start = info;
  do {
    SocketAddressInfo value;
    value.m_addrinfo = *info;
    if (info->ai_addr) {
      View(value.m_sockaddr).copy(View(info->ai_addr, info->ai_addrlen));
      value.m_sockaddr.size = info->ai_addrlen;
      if (value.m_sockaddr.size == sizeof(struct sockaddr_in)) {
        value.m_sockaddr.sockaddr_in.sin_port = htons(options.port());
      } else {
        value.m_sockaddr.sockaddr_in6.sin6_port = htons(options.port());
      }
    }

    if (info->ai_canonname) {
      value.m_canon_name = info->ai_canonname;
    }

    result.push_back(value);

    info = info->ai_next;

  } while (info);

  freeaddrinfo(info_start);
  Socket::deinitialize();

  return result;
}

Socket::Socket() {
  m_socket = SOCKET_INVALID;
  initialize();
}

Socket::Socket(Domain domain, Type type, Protocol protocol){
  initialize();
  API_RETURN_IF_ERROR();

  m_socket = API_SYSTEM_CALL(
    "socket",
    ::socket(
      static_cast<int>(domain),
      static_cast<int>(type),
      static_cast<int>(protocol)));
}

int Socket::decode_socket_return(int value) const {
#if defined __win32
  switch (value) {
  case INVALID_SOCKET:
    // set error number
    return -1;
    // case SOCKET_ERROR:
    // set error number
    // return -1;
  default:
    return value;
  }
#else
  return value;
#endif
}

int Socket::initialize() {
  if (m_is_initialized == 0) {

#if defined __win32
    WSADATA wsadata;
    int result;

    // Initialize Winsock
    WSAStartup(MAKEWORD(1, 0), &wsadata);
    result = WSAStartup(wsadata.wHighVersion, &wsadata);
    if (result != 0) {
      // set_error_number(EPIPE);
      return -1;
    }

#endif
  }

  m_is_initialized++;
  return 0;
}

int Socket::deinitialize() {
  if (m_is_initialized > 0) {
    m_is_initialized--;
    if (m_is_initialized == 0) {
#if defined __win32
      WSACleanup();
#endif
    }
  }
  return 0;
}

Socket::~Socket() { close(); }

int Socket::bind(const SocketAddress &addr) const {
  return decode_socket_return(
    ::bind(m_socket, addr.to_sockaddr(), static_cast<int>(addr.length())));
}

const Socket &
Socket::bind_and_listen(const SocketAddress &addr, int backlog) const {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL("", interface_bind_and_listen(addr, backlog));
  return *this;
}

int Socket::interface_bind_and_listen(const SocketAddress &address, int backlog)
  const {
  int result = bind(address);
  if (result < 0) {
    return result;
  }

  return decode_socket_return(::listen(m_socket, backlog));
}

Socket Socket::accept(SocketAddress &address) const {
  Socket result;
  socklen_t len = sizeof(struct sockaddr_in6);
  address.m_sockaddr.size = len;

  result.m_socket = decode_socket_return(
    ::accept(m_socket, &address.m_sockaddr.sockaddr, &len));

  address.m_sockaddr.size = len;
  return Socket();
  // return std::move(result);
}

Socket &Socket::connect(const SocketAddress &address) {
  // Connect to server.
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL("", interface_connect(address));
  return *this;
}

int Socket::interface_connect(const SocketAddress &address) const {
  return decode_socket_return(::connect(
    m_socket,
    address.to_sockaddr(),
    static_cast<int>(address.length())));
}

int Socket::interface_close(int fd) const {
  MCU_UNUSED_ARGUMENT(fd);
  int result = 0;
  if (m_socket != SOCKET_INVALID) {
#if defined __win32
    result = decode_socket_return(closesocket(m_socket));
#else
    result = decode_socket_return(::close(m_socket));
#endif
  }
  return result;
}

int Socket::interface_read(int fd, void *buf, int nbyte) const {
  MCU_UNUSED_ARGUMENT(fd);
  return decode_socket_return(::recv(m_socket, buf, nbyte, 0));
}

int Socket::interface_write(int fd, const void *buf, int nbyte) const {
  MCU_UNUSED_ARGUMENT(fd);
  return decode_socket_return(::send(m_socket, buf, nbyte, 0));
}

const Socket &Socket::send_to(
  const SocketAddress &socket_address,
  const void *buf,
  int nbyte) const {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL(
    "",
    decode_socket_return(::sendto(
      m_socket,
      (const char *)buf,
      nbyte,
      0,
      var::View(socket_address.m_sockaddr).to<struct sockaddr>(),
      socket_address.length())));
  return *this;
}

const Socket &
Socket::receive_from(const SocketAddress &address, void *buf, int nbyte) const {
  API_RETURN_VALUE_IF_ERROR(*this);
  socklen_t address_len = address.m_sockaddr.size;
  API_SYSTEM_CALL(
    "",
    decode_socket_return(::recvfrom(
      m_socket,
      (char *)buf,
      nbyte,
      0,
      var::View(address.m_sockaddr).to<struct sockaddr>(),
      &address_len)));
  return *this;
}

const Socket &Socket::shutdown(const fs::OpenMode how) const {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL("", interface_shutdown(m_socket, how));
  return *this;
}

int Socket::interface_shutdown(SOCKET_T fd, const fs::OpenMode how) const {
  int socket_how = SHUT_RDWR;
  if (how.is_read_only()) {
    socket_how = SHUT_RD;
  } else if (how.is_write_only()) {
    socket_how = SHUT_WR;
  }
  return decode_socket_return(::shutdown(m_socket, socket_how));
}

Socket &Socket::set_option(const SocketOption &option) {
  decode_socket_return(::setsockopt(
    m_socket,
    option.m_level,
    option.m_name,
#if defined __win32
    var::View(option.m_option_value).to_const_char(),
#else
    var::View(option.m_option_value).to_const_void(),
#endif
    option.m_option_value.size()));
  return *this;
}
