/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#ifndef AUTH_HPP
#define AUTH_HPP

#include <sos/dev/auth.h>
#include <sos/link.h>

#include "fs/File.hpp"
#include "var/String.hpp"

namespace sys {

/*! \brief Auth Class
 * \details This class is used to authenticate
 * the calling thread to the OS and gain root privileges.
 *
 * This is similar to using `sudo`.
 *
 *
 * ```msc
 * Caller->System: Sends 128-bit Random Number
 * Note right of System: Appends 128-bits
 * System->Caller: 256-bit Random Number
 * Caller->System: SHA256(Secret Key, Random Number)
 * Note right of System: Validates token
 * System->Caller: SHA256(Random Number, Secret Key)
 * ```
 *
 */
class Auth : public fs::FileAccess<Auth> {
public:
  class Token {
  public:
    Token() { m_auth_token = {0}; }
    explicit Token(const auth_token_t &auth_token) : m_auth_token(auth_token) {}
    explicit Token(const var::String &token);
    explicit Token(var::View token);

    bool is_valid() const {
      for (u8 i = 0; i < sizeof(auth_token_t); i++) {
        if (m_auth_token.data[i] != 0) {
          return true;
        }
      }
      return false;
    }

    bool operator==(const Token &a) const {
      return memcmp(&a.m_auth_token, &m_auth_token, sizeof(auth_token_t)) == 0;
    }

    var::String to_string() const {
      var::String result;
      for (u8 i = 0; i < sizeof(auth_token_t); i++) {
        result << var::String().format("%02x", m_auth_token.data[i]);
      }
      return result;
    }

    const auth_token_t &auth_token() const { return m_auth_token; }

    static u32 size() { return sizeof(auth_token_t); }

  private:
    void populate(var::View data);
    auth_token_t m_auth_token;
  };

  Auth(FSAPI_LINK_DECLARE_DRIVER_NULLPTR);

  bool authenticate(var::View key);

  Token start(const Token &token);
  Token finish(const Token &token);

  bool is_valid() const { return status().is_success(); }

private:
#if defined __link
  API_AF(Auth, link_transport_mdriver_t *, driver, nullptr);
#endif
};

} // namespace sys

#endif // AUTH_HPP
