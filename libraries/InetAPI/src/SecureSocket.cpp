/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#include "inet/SecureSocket.hpp"

using namespace inet;

SecureSocketApi SecureSocket::m_api;

SecureSocket::SecureSocket(Domain domain, Type type, Protocol protocol) {
  API_RETURN_IF_ERROR();

  API_SYSTEM_CALL(
    "secure socket",
    api()->socket(
      &m_context,
      static_cast<int>(domain),
      static_cast<int>(type),
      static_cast<int>(protocol)));
}

SecureSocket::SecureSocket(u32 ticket_lifetime) {
  m_ticket_lifetime = ticket_lifetime;
  m_ticket.resize(0);
}

SecureSocket::~SecureSocket() { close(); }

// already documented in inet::Socket
SecureSocket &SecureSocket::connect(const SocketAddress &address) {
  int result;

  if (m_ticket.size() > 0) {
    result = api()->parse_ticket(m_context, m_ticket.data(), m_ticket.size());
    if (result < 0) {
      printf("Ticket parse failed: 0x%X\n", result * -1);
    }
  }

  result = api()->connect(
    m_context,
    address.to_sockaddr(),
    address.length(),
    address.canon_name().cstring());

  if (m_ticket_lifetime && result == 0) {
    m_ticket.resize(2619);
    do {
      result = api()->write_ticket(
        m_context,
        m_ticket.data(),
        m_ticket.size(),
        m_ticket_lifetime);
#if 0
			if( result == MBEDTLS_ERR_SSL_BUFFER_TOO_SMALL ){
				m_ticket.resize(m_ticket.size() + 64);
			}
#endif
    } while (
#if 0
						(result == MBEDTLS_ERR_SSL_BUFFER_TOO_SMALL) &&
#endif
      (m_ticket.size() < 4096));

    if (result > 0) {
      m_ticket.resize(result);
    }
  }
  return result;
}

// already documented in inet::Socket
int SecureSocket::interface_bind_and_listen(
  const SocketAddress &address,
  int backlog) const {
  return -1;
}

// already documented in inet::Socket
Socket SecureSocket::accept(SocketAddress &address) const { return Socket(); }

// already documented in inet::Socket
int SecureSocket::shutdown(int how) const { return -1; }

int SecureSocket::interface_write(int fd, const void *buf, int nbyte) const {
  int bytes_written = 0;
  int result;
  do {
    result = api()->write(
      m_context,
      (const u8 *)buf + bytes_written,
      nbyte - bytes_written);
    if (result > 0) {
      bytes_written += result;
    }
  } while (result > 0);
  if (result < 0 && bytes_written == 0) {
    return result;
  }
  return bytes_written;
}

int SecureSocket::interface_read(int fd, void *buf, int nbyte) const {
  return api()->read(m_context, buf, nbyte);
}

int SecureSocket::interface_close(int fd) const {
  int result = 0;
  if (m_context) {
    result = api()->close(&m_context);
    m_context = nullptr;
  }
  return result;
}
