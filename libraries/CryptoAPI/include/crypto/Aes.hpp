/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#ifndef CRYPTOAPI_CRYPTO_AES_HPP_
#define CRYPTOAPI_CRYPTO_AES_HPP_

#include <sos/api/crypt_api.h>

#include "api/api.hpp"

#include "fs/File.hpp"

namespace crypto {

typedef api::Api<crypt_hash_api_t, CRYPT_SHA256_API_REQUEST> Sha256Api;
typedef api::Api<crypt_hash_api_t, CRYPT_SHA512_API_REQUEST> Sha512Api;
typedef api::Api<crypt_random_api_t, CRYPT_RANDOM_API_REQUEST> RandomApi;

class Aes : public api::Object {
public:
  using InitializationVector = var::Array<u8, 16>;
  using Iv = InitializationVector;

  Aes();
  ~Aes();

  bool is_valid() const { return m_context != nullptr; }

  Aes &set_key(const var::View &key);

  Aes &set_initialization_vector(const var::View &value);

  const InitializationVector &initialization_vector() const {
    return m_initialization_vector;
  }

  class CryptOptions {
    API_AF(CryptOptions, fs::File *, plain, nullptr);
    API_AF(CryptOptions, fs::File *, cipher, nullptr);
  };

  Aes &encrypt_ecb(const CryptOptions &options);
  Aes &decrypt_ecb(const CryptOptions &options);

  Aes &encrypt_cbc(const CryptOptions &options);
  Aes &decrypt_cbc(const CryptOptions &options);

#if 0 // not yet implemented
  Aes &encrypt_ctr(const CryptOptions &options);
  Aes &decrypt_ctr(const CryptOptions &options);
#endif

private:
  using Api = api::Api<crypt_aes_api_t, CRYPT_AES_API_REQUEST>;
  static Api m_api;

  void *m_context = nullptr;
  InitializationVector m_initialization_vector;

  static Api &api() { return m_api; }
  int initialize();
  int finalize();
};

} // namespace crypto

#endif // CRYPTOAPI_CRYPTO_AES_HPP_
