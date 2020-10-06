/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
// Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#include "VarAPI/Tokenizer.hpp"
#include "VarAPI/Item.hpp"
#include <cstdio>
#include <cstring>

#if USE_PRINTER
#include "sys/Printer.hpp"

sys::Printer &operator<<(sys::Printer &printer, const var::Tokenizer &a) {
  printer.print_open_object(printer.verbose_level(), "tokens");
  for (u32 i = 0; i < a.count(); i++) {
    printer.print(
      printer.verbose_level(),
      var::String().format("[%04d]", i).cstring(),
      a.at(i).cstring());
  }
  printer.print_close_object();
  return printer;
}

#endif

using namespace var;

Tokenizer::Tokenizer() {}

Tokenizer &
Tokenizer::parse(const var::StringView &input, const ParseOptions &options) {

  const u32 max_delimeter_count = options.maximum_delimeter_count();
  const u32 length = input.length();
  m_token_list = StringList();
  u32 cursor = 0;
  u32 sub_position = 0;

  while (cursor < length) {
    if (options.delimeters().find(input.at(cursor)) != String::npos) {
      m_token_list.push_back(
        String(input).create_sub_string(String::CreateSubStringOptions()
                                          .set_position(sub_position)
                                          .set_length(cursor - sub_position)));

      sub_position = cursor + 1;
      if (
        max_delimeter_count && (m_token_list.count() == max_delimeter_count)) {
        cursor = length - 1;
      }
    } else if (
      options.ignore_between().find(input.at(cursor)) != String::npos) {
      // skip the space between specific characters
      char end = input.at(cursor);
      cursor++;
      while ((cursor < length) && (input.at(cursor) != end)) {
        cursor++;
      }
    }

    cursor++;
  }

  // push the last token
  m_token_list.push_back(
    String(input).create_sub_string(String::CreateSubStringOptions()
                                      .set_position(sub_position)
                                      .set_length(cursor - sub_position)));

  return *this;
}

const String &Tokenizer::at(u32 n) const {
  if (n < m_token_list.count()) {
    return m_token_list.at(n);
  } else {
    return String::empty_string();
  }
}

Tokenizer &Tokenizer::sort(SortBy sort_option) {
  switch (sort_option) {
  case SortBy::ascending:
    m_token_list.sort(StringList::ascending);
    break;
  case SortBy::descending:
    m_token_list.sort(StringList::descending);
    break;
  case SortBy::none:
    break;
  }
  return *this;
}
