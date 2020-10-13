/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
// Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <errno.h>

#include "var/Data.hpp"
#include "var/View.hpp"

using namespace var;

// this value corresponds to the malloc chunk size used in Stratify OS
// this may be something that could be determined through a system call

u32 Data::minimum_capacity() { return api::ApiInfo::malloc_start_chunk_size(); }

u32 Data::block_size() { return api::ApiInfo::malloc_chunk_size(); }

Data::Data(size_t size) { resize(size); }

Data Data::from_string(StringView value) {
  Data result(value.length() / 2);
  for (u32 i = 0; i < value.length() / 2; i++) {
    result.data_u8()[i]
      = value
          .get_substring(
            String::GetSubstring().set_position(i * 2).set_length(2))
          .to_unsigned_long(String::Base::hexidecimal);
  }
  return result;
}

Data &Data::resize(size_t s) {
  API_RETURN_VALUE_IF_ERROR(*this);
  m_data.resize(s);
  if (m_data.size() < s) {
    // set memory error
    API_RETURN_VALUE_ASSIGN_ERROR(*this, "Data::resize", ENOMEM);
  }
  return *this;
}

Data &Data::copy(const View a, const Copy &options) {
  const u32 bytes_to_copy
    = options.size() < a.size() ? options.size() : a.size();

  const u32 bytes_needed = bytes_to_copy + options.destination_position();

  resize(bytes_needed);
  if (status().is_error()) {
    return *this;
  }

  memcpy(
    data_u8() + options.destination_position(),
    a.to_const_void(),
    bytes_to_copy);

  return *this;
}

Data &Data::append(const View item) {
  return copy(item, Copy().set_destination_position(size()));
}
