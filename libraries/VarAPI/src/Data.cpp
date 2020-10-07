/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
// Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <errno.h>

#include "var/Data.hpp"
#include "var/View.hpp"

#if USE_PRINTER
#include "sys/Printer.hpp"
#endif

#if !defined __link
#include <mcu/arch.h>
#include <reent.h>

#if USE_PRINTER
sys::Printer &sys::operator<<(sys::Printer &printer, const var::DataInfo &a) {
  printer.key("arena", F32U, a.arena());
  printer.key("freeBlockCount", F32U, a.free_block_count());
  printer.key("freeSize", F32U, a.free_size());
  printer.key("usedSize", F32U, a.used_size());
  return printer;
}
#endif
#endif

using namespace var;

// this value corresponds to the malloc chunk size used in Stratify OS
// this may be something that could be determined through a system call

u32 Data::minimum_capacity() { return api::ApiInfo::malloc_start_chunk_size(); }

u32 Data::block_size() {
  return api::ApiInfo::malloc_chunk_size();
  ;
}

Data::Data() {
  // var::Refernence sets size to zero
}

Data::Data(size_t size) { resize(size); }

Data Data::from_string(StringView value) {
  Data result(value.length() / 2);
  for (u32 i = 0; i < value.length() / 2; i++) {
    View(result).to_u8()[i]
      = value
          .create_sub_string(
            String::CreateSubStringOptions().set_position(i * 2).set_length(2))
          .to_unsigned_long(String::Base::hexidecimal);
  }
  return result;
}

Data &Data::resize(size_t s) {
  m_data.resize(s);
  if (m_data.size() < s) {
    // set memory error
    API_ASSIGN_ERROR_CODE(api::ErrorCode::no_memory, -1);
  }
  return *this;
}

Data &Data::copy_contents(const View &item) {
  return copy_contents(item, CopyContentsOptions().set_size(item.size()));
}

Data &Data::copy_contents(const View &item, size_t size) {
  return copy_contents(item, CopyContentsOptions().set_size(size));
}

Data &Data::copy_contents(const View &a, const CopyContentsOptions &options) {
  // Position destination_position,
  // Size size) {

  u32 bytes_to_copy = options.size() < a.size() ? options.size() : a.size();

  u32 bytes_needed = bytes_to_copy + options.destination_position();

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

Data &Data::append(const View &item) {
  m_data.reserve(size() + item.size());

  for (u32 i = 0; i < item.size(); i++) {
    m_data.push_back(item.to_const_u8()[i]);
  }

  return *this;
}

Data &Data::operator<<(const View &item) { return append(item); }
