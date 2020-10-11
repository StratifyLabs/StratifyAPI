/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#ifndef CRYPTOAPI_CRYPTO_AES_HPP_
#define CRYPTOAPI_CRYPTO_AES_HPP_

#include <sos/api/crypt_api.h>

#include "api/api.hpp"

#include "fs/File.hpp"

#if defined __link && !defined CRYPT_AES_API_REQUEST
#include <mbedtls_api.h>
#define CRYPT_AES_API_REQUEST &mbedtls_crypt_aes_api
#endif

namespace crypto {

class Aes : public api::Object {
public:
  using InitializationVector = var::Array<u8, 16>;
  using Iv = InitializationVector;

  Aes();
  ~Aes();

  Aes &set_key(const var::View &key);

  Aes &set_initialization_vector(const var::View &value);

  const InitializationVector &initialization_vector() const {
    return m_initialization_vector;
  }

  class Crypt {
    API_AC(Crypt, var::View, plain);
    API_AC(Crypt, var::View, cipher);
  };

  using EncryptEcb = Crypt;
  using DecryptEcb = Crypt;

  const Aes &encrypt_ecb(const EncryptEcb &options) const;
  const Aes &decrypt_ecb(const DecryptEcb &options) const;

  using EncryptCbc = Crypt;
  using DecryptCbc = Crypt;

  const Aes &encrypt_cbc(const EncryptCbc &options) const;
  const Aes &decrypt_cbc(const DecryptCbc &options) const;

#if 0 // not yet implemented
  Aes &encrypt_ctr(const Crypt &options);
  Aes &decrypt_ctr(const Crypt &options);
#endif

private:
  using Api = api::Api<crypt_aes_api_t, CRYPT_AES_API_REQUEST>;
  static Api m_api;

  void *m_context = nullptr;
  mutable InitializationVector m_initialization_vector;

  static Api &api() { return m_api; }
  void initialize();
  void finalize();
};

template <class Derived> class AesAccess : public Aes {
public:
  Derived &set_key(const var::View &key) {
    return static_cast<Derived &>(Aes::set_key(key));
  }

  Derived &set_initialization_vector(const var::View &value) {
    return static_cast<Derived &>(Aes::set_initialization_vector(value));
  }
};

class AesCbcEncrypter : public var::Transformer,
                        public AesAccess<AesCbcEncrypter> {
public:
  int transform(
    const var::Transformer::Transform &options) const override final;
};

class AesCbcDecrypter : public var::Transformer,
                        public AesAccess<AesCbcEncrypter> {
public:
  int transform(
    const var::Transformer::Transform &options) const override final;
};

} // namespace crypto

#endif // CRYPTOAPI_CRYPTO_AES_HPP_
