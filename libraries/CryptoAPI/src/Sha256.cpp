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
  API_RETURN_IF_ERROR();
  API_SYSTEM_CALL("", api()->start(m_context));
  m_is_finished = false;
}

Sha256::~Sha256() {
  if (m_context != nullptr) {
    api()->deinit(&m_context);
  }
}

const Sha256 &Sha256::update(const var::View &input) const {
  if (m_is_finished) {
    API_RETURN_VALUE_ASSIGN_ERROR(*this, "", EINVAL);
  }
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL(
    "",
    api()->update(m_context, input.to_const_u8(), input.size()));
  return *this;
}

void Sha256::finish() const {
  if (m_is_finished == false) {
    API_RETURN_VALUE_IF_ERROR();
    m_is_finished = true;
    API_SYSTEM_CALL(
      "",
      api()->finish(
        m_context,
        (unsigned char *)m_output.data(),
        m_output.count()));
  }
}
