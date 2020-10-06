/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "VarAPI/StringView.hpp"
#include "VarAPI/String.hpp"

using namespace var;

StringView::StringView(const String &value)
  : m_string_view(value.cstring(), value.length()) {}
