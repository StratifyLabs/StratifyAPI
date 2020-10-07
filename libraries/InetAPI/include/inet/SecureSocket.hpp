/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef SAPI_INET_SECURESOCKET_HPP_
#define SAPI_INET_SECURESOCKET_HPP_

#include <mbedtls_api.h>

#include "api/api.hpp"

#include "Socket.hpp"

namespace inet {

typedef api::Api<mbedtls_api_t, MBEDTLS_API_REQUEST> SecureSocketApi;

class SecureSocket : public Socket {
public:
  SecureSocket();
  explicit SecureSocket(u32 ticket_lifetime);
  ~SecureSocket();

  // already documented in inet::Socket
  virtual int create(const SocketAddress &address) override;

  // already documented in inet::Socket
  virtual int connect(const SocketAddress &address) override;

  // already documented in inet::Socket
  virtual int
  bind_and_listen(const SocketAddress &address, int backlog = 4) const override;

  /*! \details
   *
   */
  Socket accept(SocketAddress &address) const;

  // already documented in inet::Socket
  virtual int shutdown(int how = 0) const;

  const var::Data &ticket() const { return m_ticket; }
  var::Data &ticket() { return m_ticket; }
  static SecureSocketApi api() { return m_api; }

private:
  static SecureSocketApi m_api;
  mutable void *m_context = nullptr;
  u32 m_ticket_lifetime = 0;
  var::Data m_ticket;

  int interface_close(int fd) const override final;
  int interface_read(int fd, void *buf, int nbyte) const override final;
  int interface_write(int fd, const void *buf, int nbyte) const override final;
};

} // namespace inet

#endif // SAPI_INET_SECURESOCKET_HPP_
