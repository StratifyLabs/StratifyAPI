/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef SAPI_CRYPTO_SHA256_HPP_
#define SAPI_CRYPTO_SHA256_HPP_

#include <sos/api/crypt_api.h>

#if defined __link && !defined CRYPT_SHA256_API_REQUEST
#include <mbedtls_api.h>
#define CRYPT_SHA256_API_REQUEST &mbedtls_crypt_sha256_api
#endif

#include "api/api.hpp"
#include "fs/File.hpp"
#include "var/Array.hpp"
#include "var/View.hpp"

namespace crypto {

/*! \brief SHA256 Class
 * \details This class provides
 * access to system hardware or software
 * routines that calculate SHA256
 * hash digest values.
 *
 * The system must implmenent the
 * CRYPT_SHA256_API_REQUEST in kernel_request_api().
 *
 * ```
 * #include <sys/crypt.hpp>
 * #include <sys/var.hpp>
 *
 * Sha256 hash;
 * hash.initialize(); //call once per object
 * hash.start(); //start a new digest
 * Data some_data(128);
 * some_data.fill((u8)0xaa);
 * hash << some_data; //update the digest with some data
 * hash.finalize();
 * printf("Hash is %s\n", hash.stringify());
 * ```
 *
 *
 */
class Sha256 : public api::ExecutionContext, public var::Transformer {
public:
  Sha256();
  ~Sha256();

  Sha256(const Sha256 &) = delete;
  Sha256 &operator=(const Sha256 &) = delete;
  Sha256(Sha256 &&) = default;
  Sha256 &operator=(Sha256 &&) = default;

  const Sha256 &update(const var::View &data) const;
  const var::Array<u8, 32> &output() const {
    finish();
    return m_output;
  }

  int transform(const Transform &options) const override {
    update(options.input());
    var::View(options.output()).copy(options.input());
    return options.input().size();
  }


  static constexpr size_t page_size() {
#if defined __link
    return 4096;
#else
    return 256;
#endif
  }

private:
  using Api = api::Api<crypt_hash_api_t, CRYPT_SHA256_API_REQUEST>;
  static Api m_api;

  void *m_context = nullptr;
  mutable bool m_is_finished = false;
  var::Array<u8, 32> m_output;

  void finish() const;
  static Api &api() { return m_api; }
};

} // namespace crypto

#endif // SAPI_CRYPTO_SHA256_HPP_
