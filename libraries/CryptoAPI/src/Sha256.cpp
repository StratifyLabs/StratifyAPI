/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#include "crypto/Sha256.hpp"

using namespace crypto;

Sha256::Api Sha256::m_api;

Sha256::Sha256() {
  if (api().is_valid() == false) {
    exit_fatal("api api missing");
  }
  API_RETURN_IF_ERROR();
  API_SYSTEM_CALL("", api()->init(&m_context));
}

Sha256::~Sha256() {
  if (m_context != nullptr) {
    api()->deinit(&m_context);
  }
}

Sha256 &Sha256::operator<<(const var::View &a) { return update(a); }

var::String Sha256::to_string() {
  API_RETURN_VALUE_IF_ERROR(var::String());
  var::String result = var::String().reserve(64);
  const var::Array<u8, 32> &out = output();
  for (u32 i = 0; i < out.count(); i++) {
    result += var::NumberToString(out.at(i), "%02x");
  }
  return result;
}

const var::Array<u8, 32> &Sha256::output() {
  finish();
  return m_output;
}

Sha256 &Sha256::start() {
  API_RETURN_VALUE_IF_ERROR(*this);
  m_is_finished = false;
  API_SYSTEM_CALL("", api()->start(m_context));
  return *this;
}

Sha256 &Sha256::update(const var::View &input) {
  if (m_is_finished) {
    start();
  }

  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL(
    "",
    api()->update(m_context, input.to_const_u8(), input.size()));
  return *this;
}

Sha256 &Sha256::update(const fs::File &file) {
  for (u32 i = 0; i < 32; i++) {
    char buffer[32];
    int bytes_read = file.read(var::View(buffer)).return_value();
    if (bytes_read > 0) {
      update(var::View(buffer, bytes_read));
      if (is_error()) {
        return *this;
      }
    }
  }
  return *this;
}

var::String Sha256::get_checksum(const fs::File &file) {
  return Sha256().start().update(file).to_string();
}

Sha256 &Sha256::finish() {
  API_RETURN_VALUE_IF_ERROR(*this);
  if (m_is_finished == false) {
    m_is_finished = true;
    API_SYSTEM_CALL(
      "",
      api()->finish(
        m_context,
        (unsigned char *)m_output.data(),
        m_output.count()));
  }
  return *this;
}
