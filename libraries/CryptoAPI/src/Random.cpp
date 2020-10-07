/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
// LICENSE.md for rights.
#include "crypto/Random.hpp"
#include "chrono/Clock.hpp"

using namespace crypto;

Random::Api Random::m_api;

Random::Random() {
  if (api().is_valid() == false) {
    API_ASSIGN_ERROR_CODE(api::ErrorCode::missing_system_api, -1);
  } else {
    API_ASSIGN_ERROR_CODE(api::ErrorCode::io_error, initialize());
  }
}

Random::~Random() { finalize(); }

int Random::initialize() {
  if (api()->init(&m_context) < 0) {
    return -1;
  }
  seed(var::View());
  return 0;
}

void Random::finalize() {
  if (m_context != nullptr) {
    api()->deinit(&m_context);
    m_context = nullptr;
  }
}

Random &Random::seed() {
  var::Array<u32, 64> list;
  for (u32 &item : list) {
    item = ~chrono::Clock::get_time().nanoseconds();
    chrono::wait(chrono::Microseconds(item % 1000));
  }
  return seed(list);
}

Random &Random::seed(const var::View source_data) {
  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    api()->seed(m_context, source_data.to_const_u8(), source_data.size()));
  return *this;
}

Random &Random::randomize(var::View destination_data) {
  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    api()
      ->random(m_context, destination_data.to_u8(), destination_data.size()));
  return *this;
}

var::String Random::get_string(size_t length) {
  return get_data(length / 2).to_string();
}

var::Data Random::get_data(u32 size) {
  var::Data result(size);
  var::View result_view(result);
  randomize(result_view);
  return result;
}
