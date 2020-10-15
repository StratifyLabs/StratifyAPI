/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#include "printer/Printer.hpp"

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

printer::Printer &
printer::operator<<(printer::Printer &printer, const inet::SocketAddress &a) {
  printer.key(
    "family",
    (a.family() == inet::Socket::Family::inet) ? StringView("inet")
                                               : StringView("inet6"));
  printer.key("port", Ntos(a.port()));
  printer.key("address", a.get_address_string());
  printer.key("canonName", a.canon_name());
  return printer;
}

printer::Printer &
printer::operator<<(printer::Printer &printer, const inet::AddressInfo &a) {
  u32 i = 0;
  for (const auto &entry : a.list()) {
    printer.object(Ntos(i++), entry);
  }
  return printer;
}

using namespace inet;

int Socket::m_is_initialized = 0;

AddressInfo::AddressInfo(const Construct &options) {
  API_RETURN_IF_ERROR();

  const char *server_cstring
    = options.service().is_empty() ? nullptr : options.service().cstring();

  const char *node_cstring
    = options.node().is_empty() ? nullptr : options.node().cstring();

  Socket::initialize();

  struct addrinfo address_info = {0};
  address_info.ai_family = static_cast<int>(options.family());
  address_info.ai_socktype = static_cast<int>(options.type());
  address_info.ai_flags = static_cast<int>(options.flags());

  struct addrinfo *info;
  API_SYSTEM_CALL(
    "",
    getaddrinfo(node_cstring, server_cstring, &address_info, &info));

  API_RETURN_IF_ERROR();

  struct addrinfo *info_start;
  info_start = info;
  do {
    m_list.push_back(SocketAddress(
                       info->ai_addr,
                       info->ai_addrlen,
                       info->ai_canonname,
                       options.port())
                       .set_protocol(static_cast<Protocol>(info->ai_protocol))
                       .set_type(static_cast<Type>(info->ai_socktype)));

    info = info->ai_next;

  } while (info);

  freeaddrinfo(info_start);
  Socket::deinitialize();
}

Socket::Socket() {
  initialize();
}

Socket::Socket(Domain domain, Type type, Protocol protocol) {
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

Socket::~Socket() { interface_close(0); }

const Socket &
Socket::bind_and_listen(const SocketAddress &addr, int backlog) const {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL("", interface_bind_and_listen(addr, backlog));
  return *this;
}

int Socket::interface_bind_and_listen(const SocketAddress &address, int backlog)
  const {
  int result = decode_socket_return(::bind(
    m_socket,
    address.to_sockaddr(),
    static_cast<int>(address.length())));
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
  return std::move(result);
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
    m_socket = SOCKET_INVALID;
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
  m_socket = SOCKET_INVALID;
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
    static_cast<int>(option.m_level),
    static_cast<int>(option.m_name),
    &option.m_value,
    option.m_size));
  return *this;
}
