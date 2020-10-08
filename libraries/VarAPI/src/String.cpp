/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
// Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#include <cctype>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <errno.h>

#include "var/Data.hpp"
#include "var/String.hpp"
#include "var/Tokenizer.hpp"

using namespace var;

#if USE_PRINTER
sys::Printer &sys::operator<<(sys::Printer &printer, const var::String &a) {
  return printer.key(nullptr, a.cstring());
}

Printer &sys::operator<<(Printer &printer, const var::StringList &a) {
  for (u32 i = 0; i < a.count(); i++) {
    printer.key(nullptr, a.at(i).cstring());
  }
  return printer;
}
#endif

String String::m_empty_string;

String operator+(StringView lhs, const char *rhs) {
  return String(lhs) + String(rhs);
}

String operator+(const char *lhs, StringView rhs) {
  return String(lhs) + String(rhs);
}

String operator+(StringView lhs, const String &rhs) {
  return String(lhs) + rhs;
}

String operator+(StringView lhs, String &&rhs) {
  return String(lhs) + rhs;
}

String operator+(StringView lhs, StringView rhs) {
  return String(lhs) + String(rhs);
}

String operator+(const String &lhs, StringView rhs) {
  return lhs + String(rhs);
}

String::String() {}

String::String(const View &item) {
  m_string.assign(item.to_const_char(), item.size());
}

String &String::format(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vformat(format, args);
  va_end(args);
  return *this;
}

String &String::vformat(const char *fmt, va_list list) {

  if (fmt == nullptr) {
    return *this;
  }

  va_list list_copy;
  va_copy(list_copy, list);
  const int buffer_size = 64;
  char buffer[buffer_size];
  int size = vsnprintf(buffer, buffer_size - 1, fmt, list);
  if (size <= buffer_size - 1) {
    *this = String(buffer);
  } else {
    Data tmp(size + 1);
    View tmp_item(tmp);
    size = vsnprintf(tmp_item.to_char(), tmp_item.size(), fmt, list_copy);

    if (size > 0) {
      *this = String(tmp_item.to_const_char());
    } else {
      clear();
    }
  }
  va_end(list_copy);

  return *this;
}

String &String::erase(StringView string_to_erase, size_t position) {
  size_t erase_pos;
  const size_t len = string_to_erase.length();
  while ((erase_pos = find(string_to_erase, position)) != npos) {
    erase(Erase().set_position(erase_pos).set_length(len));
    position = erase_pos;
  }
  return *this;
}

String &String::replace(const Replace &options) {
  size_t pos = options.position();
  const size_t old_length = options.old_string().length();
  const size_t new_length = options.new_string().length();
  size_t replaced_count = 0;
  while (((pos = find(options.old_string(), pos)) != String::npos)
         && (options.count() ? replaced_count < options.count() : 1)) {

    erase(Erase().set_position(pos).set_length(old_length));

    m_string.insert(pos, options.new_string().string_view());
    // insert(pos, options.new_string());
    pos += new_length;
    replaced_count++;
  }
  return *this;
}

String &String::to_upper() {
  for (auto &c : m_string) {
    c = std::toupper(c);
  }
  return *this;
}

String &String::to_lower() {
  for (auto &c : m_string) {
    c = std::tolower(c);
  }
  return *this;
}

#if 0
String & String::operator << (
		var::View reference
		){

	for(
			u32 i=0;
			i < reference.size();
			i++){

		append(String().format(
						 "%02x", reference.to_const_u8()[i]
						 )
					 );
	}

	return *this;
}
#endif

float String::to_float() const {
#ifndef __link
  return ::atoff(cstring());
#else
  return ::atof(cstring());
#endif
}

Vector<String> String::split(StringView delimiter) const {
  Tokenizer tokens = Tokenizer().parse(
    *this,
    Tokenizer::Parse().set_delimeters(delimiter));
  return tokens.list();
}

size_t String::count(var::StringView occurance) const {
  size_t pos = 0;
  u32 item_count = 0;
  while ((pos < length()) && (pos != String::npos)) {
    pos = find(occurance, pos);
    if (pos != String::npos) {
      item_count++;
      pos += occurance.length();
    }
  }
  return item_count;
}
