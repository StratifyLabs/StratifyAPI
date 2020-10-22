#include "var/StackString.hpp"

using namespace var;

u32 m_stack_string_unused = 0;

int NumberString::to_integer() const { return ::atoi(cstring()); }
float NumberString::to_float() const {
#ifndef __link
  return ::atoff(cstring());
#else
  return ::atof(cstring());
#endif
}

long NumberString::to_long(Base base) const {
  return ::strtol(cstring(), nullptr, static_cast<int>(base));
}

unsigned long NumberString::to_unsigned_long(Base base) const {
  return ::strtoul(cstring(), nullptr, static_cast<int>(base));
}

var::StringView PathString::suffix() const { return suffix(string_view()); }

var::StringView PathString::name() const { return name(string_view()); }

var::StringView PathString::parent_directory() const {
  return parent_directory(string_view());
}

var::StringView PathString::base_name() const {
  return base_name(string_view());
}

var::StringView PathString::no_suffix() const {
  return no_suffix(string_view());
}

var::StringView PathString::suffix(const StringView path) {
  size_t pos = path.reverse_find(".");

  if (pos == var::String::npos) {
    return var::StringView();
  }

  return StringView(path.data() + pos + 1);
}

var::StringView PathString::name(const StringView path) {
  size_t pos = path.reverse_find('/');

  if (pos == var::String::npos) {
    return path;
  }

  return StringView(path.data() + pos + 1);
}

var::StringView PathString::parent_directory(const StringView path) {
  size_t pos = path.reverse_find('/');

  if (pos == var::String::npos) {
    return var::StringView();
  }

  return StringView(path.data(), pos);
}

var::StringView PathString::base_name(const StringView path) {
  return no_suffix(name(path));
}

var::StringView PathString::no_suffix(const StringView path) {
  size_t pos = path.reverse_find('.');

  if (pos == StringView::npos) {
    return path;
  }

  return StringView(path.data(), pos);
}

bool PathString::is_hidden(const StringView path) {
  if (name(path).find(".") == 0) {
    return true;
  }

  var::StringView parent_directory_path = parent_directory(path);
  if (parent_directory_path != path) {
    return is_hidden(parent_directory(parent_directory_path));
  }

  return false;
}
