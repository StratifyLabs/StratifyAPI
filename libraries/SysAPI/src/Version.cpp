/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#include "sys/Version.hpp"
#include "var/Tokenizer.hpp"

using namespace var;

Version Version::from_triple(u16 major, u8 minor, u8 patch) {
  Version result;
  return std::move(result.set_string(
    String::number(major) + StringView(".") + String::number(minor)
    + StringView(".") + String::number(patch)));
}

Version Version::from_u16(u16 major_minor) {
  Version result;
  return std::move(result.set_string(
    String::number(major_minor >> 8) + StringView(".")
    + String::number(major_minor & 0xff)));
}

u32 Version::to_bcd() const {
  StringViewList tokens = m_version.split(".");
  u32 result = 0;
  u32 token_max = tokens.count() < 3 ? tokens.count() : 3;
  for (u32 i = 0; i < token_max; i++) {
    result |= (String(tokens.at(i)).to_integer() & 0xff) << (8 * (2 - i));
  }
  return result;
}

int Version::compare(const Version &a, const Version &b) {
  StringViewList a_tokens = a.m_version.split(".");
  StringViewList b_tokens = b.m_version.split(".");

  if (a_tokens.count() > b_tokens.count()) {
    return 1;
  }

  if (a_tokens.count() < b_tokens.count()) {
    return -1;
  }

  // count is equal -- check the numbers
  for (u32 i = 0; i < a_tokens.count(); i++) {
    if (
      StringView(a_tokens.at(i)).to_unsigned_long()
      > StringView(b_tokens.at(i)).to_unsigned_long()) {
      return 1;
    }
    if (
      StringView(a_tokens.at(i)).to_unsigned_long()
      < StringView(b_tokens.at(i)).to_unsigned_long()) {
      return -1;
    }
  }

  // versions are equal
  return 0;
}
