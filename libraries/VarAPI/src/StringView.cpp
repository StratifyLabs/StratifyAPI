/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "var/StringView.hpp"
#include "var/StackString.hpp"
#include "var/String.hpp"

using namespace var;

String
StringView::get_substring(const GetSubstring &options) const {
  return std::move(String(*this).get_substring(options));
}

String StringView::get_substring_at_position(size_t position) const {
  return std::move(String(*this).get_substring_at_position(position));
}

String StringView::get_substring_with_length(size_t length) const {
  return std::move(String(*this).get_substring_with_length(length));
}

var::Vector<String> StringView::split(StringView delimeters) const {
  return std::move(String(*this).split(delimeters));
}

float StringView::to_float() const {
#ifndef __link
  return ::atoff(cstring());
#else
  return ::atof(StackString64(*this).cstring());
#endif
}

long StringView::to_long(Base base) const {
  return ::strtol(
    StackString64(*this).cstring(),
    nullptr,
    static_cast<int>(base));
}

unsigned long StringView::to_unsigned_long(Base base) const {
  return ::strtoul(
    StackString64(*this).cstring(),
    nullptr,
    static_cast<int>(base));
}
