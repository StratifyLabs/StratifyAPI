/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#include "var/VersionString.hpp"
#include "var/Tokenizer.hpp"

using namespace var;

VersionString VersionString::from_triple(u16 major, u8 minor, u8 patch) {
  VersionString result;
  return std::move(result.set_string(
    String::number(major) + StringView(".") + String::number(minor)
    + StringView(".") + String::number(patch)));
}

VersionString VersionString::from_u16(u16 major_minor) {
  VersionString result;
  return std::move(result.set_string(
    String::number(major_minor >> 8) + StringView(".")
    + String::number(major_minor & 0xff)));
}

u32 VersionString::to_bcd() const {
  StringList tokens = m_version.split(".");
  u32 result = 0;
  u32 token_max = tokens.count() < 3 ? tokens.count() : 3;
  for (u32 i = 0; i < token_max; i++) {
    result |= (String(tokens.at(i)).to_integer() & 0xff) << (8 * (2 - i));
  }
  return result;
}

int VersionString::compare(const VersionString &a, const VersionString &b) {
  StringList a_tokens = a.m_version.split(".");
  StringList b_tokens = b.m_version.split(".");

  if (a_tokens.count() > b_tokens.count()) {
    return 1;
  }

  if (a_tokens.count() < b_tokens.count()) {
    return -1;
  }

  // count is equal -- check the numbers
  for (u32 i = 0; i < a_tokens.count(); i++) {
    if (
      String(a_tokens.at(i)).to_integer()
      > String(b_tokens.at(i)).to_integer()) {
      return 1;
    }
    if (
      String(a_tokens.at(i)).to_integer()
      < String(b_tokens.at(i)).to_integer()) {
      return -1;
    }
  }

  // versions are equal
  return 0;
}
