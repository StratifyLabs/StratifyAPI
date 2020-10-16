/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef SAPI_INET_SECURESOCKET_HPP_
#define SAPI_INET_SECURESOCKET_HPP_

#include <mbedtls_api.h>

#include "api/api.hpp"
#include "chrono/ClockTime.hpp"

#include "Socket.hpp"

namespace inet {

typedef api::Api<mbedtls_api_t, MBEDTLS_API_REQUEST> SecureSocketApi;

class SecureSocket : public Socket {
public:
  explicit SecureSocket(
    Domain domain,
    Type type = Type::stream,
    Protocol protocol = Protocol::tcp);

  explicit SecureSocket(const SocketAddress &address);
  ~SecureSocket();

  SecureSocket(SecureSocket &&a) { std::swap(m_context, a.m_context); }
  SecureSocket &operator=(SecureSocket &&a) {
    std::swap(m_context, a.m_context);
    return *this;
  }

  SecureSocket &set_ticket_lifetime(u32 seconds) {
    m_ticket_lifetime_seconds = seconds;
    return *this;
  }

  // already documented in inet::Socket
  virtual int shutdown(int how = 0) const;

  const var::Data &ticket() const { return m_ticket; }

  SecureSocket &set_ticket(var::View ticket) {
    m_ticket.copy(ticket);
    return *this;
  }

private:
  static SecureSocketApi api() { return m_api; }
  static SecureSocketApi m_api;
  u32 m_ticket_lifetime_seconds = 60 * 60 * 24UL; // one day

  mutable var::Data m_ticket;
  mutable void *m_context = nullptr;

  int interface_connect(const SocketAddress &address) const override final;

  int interface_bind_and_listen(const SocketAddress &address, int backlog)
    const override final;

  virtual int
  interface_shutdown(int fd, const fs::OpenMode how) const override final;

  int interface_close(int fd) const;

  int interface_read(int fd, void *buf, int nbyte) const override final;
  int interface_write(int fd, const void *buf, int nbyte) const override final;
};

} // namespace inet

#endif // SAPI_INET_SECURESOCKET_HPP_
