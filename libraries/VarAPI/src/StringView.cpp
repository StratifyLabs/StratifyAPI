/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "var/StringView.hpp"
#include "var/String.hpp"

using namespace var;

String
StringView::get_substring(const GetSubstring &options) const {
  return String(*this).get_substring(options);
}

String StringView::get_substring_at_position(size_t position) const {
  return String(*this).get_substring_at_position(position);
}

String StringView::get_substring_with_length(size_t length) const {
  return String(*this).get_substring_with_length(length);
}

var::Vector<String> StringView::split(StringView delimeters) const {
  return String(*this).split(delimeters);
}
