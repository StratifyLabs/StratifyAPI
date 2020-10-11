/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#include <errno.h>

#include "crypto/Aes.hpp"
#include "crypto/Random.hpp"

using namespace crypto;
Aes::Api Aes::m_api;

Aes::Aes() {
  if (api().is_valid() == false) {
    API_SYSTEM_CALL("missing api", -1);
  } else {
    API_SYSTEM_CALL("", initialize());
  }
}

Aes::~Aes() {
  m_initialization_vector.fill(0);
  finalize();
}

int Aes::initialize() {
  finalize();
  return api()->init(&m_context);
}

int Aes::finalize() {
  if (m_context != nullptr) {
    api()->deinit(&m_context);
  }
  return 0;
}

Aes &Aes::set_initialization_vector(const var::View &value) {
  API_RETURN_VALUE_IF_ERROR(*this);

  if (value.size() != m_initialization_vector.count()) {
    API_SYSTEM_CALL("", -1);
    return *this;
  }

  for (u32 i = 0; i < m_initialization_vector.count(); i++) {
    m_initialization_vector.at(i) = value.to_const_u8()[i];
  }

  return *this;
}

Aes &Aes::set_key(const var::View &key) {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL(
    "",
    api()->set_key(m_context, key.to_const_u8(), key.size() * 8, 8));
  return *this;
}

Aes &Aes::encrypt_ecb(const Crypt &options) {
  API_RETURN_VALUE_IF_ERROR(*this);
  for (u32 i = 0; i < options.plain()->size(); i += 16) {
    unsigned char plain[16] = {0};
    unsigned char cipher[16];

    // if read is < 16 bytes -- plain is zero padded
    if (options.plain()->read(var::View(plain)).status().is_error()) {
      API_SYSTEM_CALL("", -1);
      return *this;
    }

    if (api()->encrypt_ecb(m_context, plain, cipher) < 0) {
      API_SYSTEM_CALL("", -1);
      return *this;
    }

    if (options.cipher()->write(var::View(cipher)).status().is_error()) {
      API_SYSTEM_CALL("", -1);
      return *this;
    }
  }

  return *this;
}

Aes &Aes::decrypt_ecb(const Crypt &options) {
  API_RETURN_VALUE_IF_ERROR(*this);

  if (options.cipher()->size() % 16 != 0) {
    API_SYSTEM_CALL("", -1);
    return *this;
  }

  for (u32 i = 0; i < options.cipher()->size(); i += 16) {
    unsigned char plain[16];
    unsigned char cipher[16];

    // if read is < 16 bytes -- plain is zero padded
    if (options.cipher()->read(var::View(cipher)).status().is_error()) {
      API_SYSTEM_CALL("", -1);
      return *this;
    }

    if (api()->decrypt_ecb(m_context, cipher, plain) < 0) {
      API_SYSTEM_CALL("", -1);
      return *this;
    }

    if (options.plain()->write(var::View(plain)).status().is_error()) {
      API_SYSTEM_CALL("", -1);
      return *this;
    }
  }

  return *this;
}

Aes &Aes::encrypt_cbc(const Crypt &options) {
  API_RETURN_VALUE_IF_ERROR(*this);

  u32 length = options.plain()->size();
  for (u32 i = 0; i < length; i += 16) {
    unsigned char plain[16] = {0};
    unsigned char cipher[16];

    // if read is < 16 bytes -- plain is zero padded
    if (options.plain()->read(var::View(plain)).status().is_error()) {
      API_SYSTEM_CALL("", -1);
      return *this;
    }

    if (
      api()->encrypt_cbc(
        m_context,
        16,
        m_initialization_vector.data(), // init vector
        plain,
        cipher)
      < 0) {
      API_SYSTEM_CALL("", -1);
      return *this;
    }

    if (options.cipher()->write(var::View(cipher)).status().is_error()) {
      API_SYSTEM_CALL("", -1);
      return *this;
    }
  }

  return *this;
}

Aes &Aes::decrypt_cbc(const Crypt &options) {
  API_RETURN_VALUE_IF_ERROR(*this);

  if (options.cipher()->size() % 16 != 0) {
    API_SYSTEM_CALL("", -1);
    return *this;
  }

  for (u32 i = 0; i < options.cipher()->size(); i += 16) {
    unsigned char plain[16];
    unsigned char cipher[16];

    // if read is < 16 bytes -- plain is zero padded
    if (options.cipher()->read(var::View(cipher)).status().is_error()) {
      API_SYSTEM_CALL("", -1);
      return *this;
    }

    if (
      api()->decrypt_cbc(
        m_context,
        16,
        m_initialization_vector.data(), // init vector
        cipher,
        plain)
      < 0) {
      API_SYSTEM_CALL("", -1);
      return *this;
    }

    if (options.plain()->write(var::View(plain)).status().is_error()) {
      API_SYSTEM_CALL("", -1);
      return *this;
    }
  }

  return *this;
}
