/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#ifndef CRYPTOAPI_CRYTPO_RANDOM_HPP
#define CRYPTOAPI_CRYTPO_RANDOM_HPP

#include <sos/api/crypt_api.h>

#include "api/api.hpp"

#include "var/Data.hpp"
#include "var/String.hpp"

namespace crypto {

class Random : public api::Object {
public:
  Random();
  ~Random();

  Random &seed();
  Random &seed(const var::View source_data);
  Random &randomize(const var::View destination_data);

  var::String get_string(size_t length);
  var::Data get_data(u32 size);

private:
  using Api = api::Api<crypt_random_api_t, CRYPT_RANDOM_API_REQUEST>;
  static Api m_api;

  static Api &api() { return m_api; }
  int initialize();
  void finalize();
  void *m_context = nullptr;
};

} // namespace crypto

#endif // CRYPTOAPI_CRYTPO_RANDOM_HPP
