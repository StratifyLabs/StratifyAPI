/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
// LICENSE.md for rights.
#include "crypto/Random.hpp"
#include "chrono/ClockTime.hpp"
#include "chrono/MicroTime.hpp"

using namespace crypto;

Random::Api Random::m_api;

Random::Random() {
  API_RETURN_IF_ERROR();
  if (api().is_valid() == false) {
    API_RETURN_ASSIGN_ERROR("missing api", ENOTSUP);
  } else {
    API_RETURN_IF_ERROR();
    API_SYSTEM_CALL("random", api()->init(&m_context));
    seed(var::View());
  }
}

Random::~Random() {
  if (m_context != nullptr) {
    api()->deinit(&m_context);
    m_context = nullptr;
  }
}

Random &Random::seed() {
  var::Array<u32, 64> list;
  for (u32 &item : list) {
    item = ~chrono::ClockTime::get_system_time().nanoseconds();
    chrono::wait(chrono::MicroTime(item % 1000));
  }
  return seed(list);
}

Random &Random::seed(const var::View source_data) {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL(
    "",
    api()->seed(m_context, source_data.to_const_u8(), source_data.size()));
  return *this;
}

const Random &Random::randomize(var::View destination_data) const {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL(
    "",
    api()
      ->random(m_context, destination_data.to_u8(), destination_data.size()));
  return *this;
}

var::String Random::get_string(size_t length) const {
  return get_data(length / 2).to_string();
}

var::Data Random::get_data(u32 size) const {
  var::Data result(size);
  randomize(result);
  return std::move(result);
}
