/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#include "crypto/Sha256.hpp"

using namespace crypto;

Sha256::Api Sha256::m_api;

Sha256::Sha256() {
  if (api().is_valid() == false) {
    exit_fatal("api api missing");
  }
  m_context = 0;
  m_is_finished = true;
}

Sha256::~Sha256() { finalize(); }

Sha256 &Sha256::operator<<(const var::View &a) { return update(a); }

int Sha256::initialize() {
  finalize();
  return api()->init(&m_context);
}

var::String Sha256::to_string() {
  var::String result;
  const var::Array<u8, 32> &out = output();
  for (u32 i = 0; i < out.count(); i++) {
    result << var::String::number(out.at(i), "%02x");
  }
  return result;
}

const var::Array<u8, 32> &Sha256::output() {
  finish();
  return m_output;
}

int Sha256::finalize() {
  if (m_context != 0) {
    api()->deinit(&m_context);
  }
  return 0;
}

Sha256 &Sha256::start() {
  m_is_finished = false;
  API_ASSIGN_ERROR_CODE(api::ErrorCode::io_error, api()->start(m_context));
  return *this;
}

Sha256 &Sha256::update(const var::View &input) {
  if (is_initialized() == false) {
    initialize();
  }

  if (m_is_finished) {
    start();
  }

  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    api()->update(m_context, input.to_const_u8(), input.size()));
  return *this;
}

Sha256 &Sha256::update(fs::File &file) {
  for (u32 i = 0; i < 32; i++) {
    char buffer[32];
    int bytes_read = file.read(var::View(buffer)).status().value();
    if (bytes_read > 0) {
      update(var::View(buffer, bytes_read));
      if (status().is_error()) {
        return *this;
      }
    }
  }
  return *this;
}

var::String Sha256::get_checksum(fs::File &file) {
  return Sha256().start().update(file).to_string();
}

Sha256 &Sha256::finish() {
  if (m_is_finished == false) {
    m_is_finished = true;
    API_ASSIGN_ERROR_CODE(
      api::ErrorCode::io_error,
      api()->finish(
        m_context,
        (unsigned char *)m_output.data(),
        m_output.count()));
  }
  return *this;
}
