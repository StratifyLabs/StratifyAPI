/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#include <fcntl.h>
#include <unistd.h>

#include "crypto/Random.hpp"
#include "crypto/Sha256.hpp"
#include "sys/Auth.hpp"

#if defined __link
#define OPEN() ::link_open(m_driver, "/dev/auth", LINK_O_RDWR)
#define CLOSE() ::link_close(m_driver, m_fd)
#define IOCTL(REQUEST, CTL) ::link_ioctl(m_driver, m_fd, REQUEST, CTL)
#else
#define OPEN() ::open("/dev/auth", O_RDWR)
#define CLOSE() ::close(m_fd)
#define IOCTL(REQUEST, CTL) ::ioctl(m_fd, REQUEST, CTL)
#endif

using namespace sys;

AuthToken::AuthToken(const var::String &token) {
  populate(var::Data::from_string(token));
}

AuthToken::AuthToken(const var::View &token) { populate(token); }

void AuthToken::populate(const var::View &data) {
  memset(&m_auth_token, 0, sizeof(m_auth_token));
  u32 size
    = data.size() > sizeof(m_auth_token) ? sizeof(m_auth_token) : data.size();
  memcpy(&m_auth_token, data.to_const_void(), size);
}

Auth::Auth(FSAPI_LINK_DECLARE_DRIVER)
  : FileAccess(
    "/dev/auth",
    fs::OpenMode::read_write() FSAPI_LINK_INHERIT_DRIVER_LAST) {}

bool Auth::authenticate(const var::View &key) {
  crypto::Random random;
  crypto::Sha256 hash;

  var::Data random_data(AuthToken::size());
  random.randomize(random_data);

  AuthToken token = start(AuthToken(random_data));

  auth_key_token_t key_token;
  auth_key_token_t reverse_key_token;

  key_token.key = AuthToken(key).auth_token();
  key_token.token = token.auth_token();
  reverse_key_token.key = key_token.token;
  reverse_key_token.token = key_token.key;

  // do SHA256 calcs
  hash.start();
  hash << var::View(key_token);
  hash.finish();

  AuthToken validation_token = finish(AuthToken(hash.output()));

  hash.start();
  hash << var::View(reverse_key_token);
  hash.finish();

  // hash output should match validation token
  if (
    var::View(hash.output())
    == var::View(validation_token.auth_token())) {
    return true;
  }

  return false;
}

AuthToken Auth::start(const AuthToken &token) {
  auth_token_t result = token.auth_token();
  if (ioctl(I_AUTH_START, &result).status().is_error()) {
    return AuthToken();
  }
  return AuthToken(result);
}

AuthToken Auth::finish(const AuthToken &token) {
  auth_token_t result = token.auth_token();
  if (ioctl(I_AUTH_FINISH, &result).status().is_error()) {
    return AuthToken();
  }
  return AuthToken(result);
}
