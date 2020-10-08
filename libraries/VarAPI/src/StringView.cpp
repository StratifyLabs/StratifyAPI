/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "var/StringView.hpp"
#include "var/String.hpp"

using namespace var;

StringView::StringView(const String &value)
  : m_string_view(value.cstring(), value.length()) {}

String
StringView::create_sub_string(const CreateSubStringOptions &options) const {
  return String(*this).get_substring(options);
}
