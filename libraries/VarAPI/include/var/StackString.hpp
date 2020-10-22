#ifndef VARAPI_VAR_STACKSTRING_HPP
#define VARAPI_VAR_STACKSTRING_HPP

#include <sys/syslimits.h>

#include "String.hpp"
#include "StringView.hpp"

namespace var {

#if 0
class StackStringObject {
public:
private:
  char *m_buffer_pointer;
  size_t m_size;
};
#endif

template <class Derived, int Size> class StackString {
public:
  using Base = StringView::Base;
  Derived &clear() {
    m_buffer[0] = 0;
    return static_cast<Derived &>(*this);
  }

  bool is_empty() const { return m_buffer[0] == 0; }

  Derived &append(const char *a) {
    strncat(m_buffer, a, Size - 1);
    return static_cast<Derived &>(*this);
  }

  Derived &append(const StringView a) {
    const size_t length = strlen(m_buffer);
    const size_t s
      = a.length() > Size - 1 - length ? Size - 1 - length : a.length();
    m_buffer[s] = 0;
    memcpy(m_buffer + length, a.data(), s);
    return static_cast<Derived &>(*this);
  }

  Derived &operator+=(const char *a) { return append(a); }
  Derived &operator+=(const StringView a) { return append(a); }
  Derived &operator+=(const StackString &a) { return append(a.string_view()); }

  bool operator==(const char *a) { return strncmp(m_buffer, a, Size - 1) == 0; }
  bool operator!=(const char *a) { return strncmp(m_buffer, a, Size - 1) != 0; }

  bool operator==(const StackString &a) const {
    return string_view() == a.string_view();
  }

  bool operator!=(const StackString &a) const {
    return string_view() != a.string_view();
  }

  // explicit conversion
  const char *cstring() const { return m_buffer; }
  const StringView string_view() const { return StringView(m_buffer); }

  // implicit conversion
  operator const char *() const { return m_buffer; }
  operator const StringView() { return StringView(m_buffer); }

  template <typename... Args>
  Derived &format(const char *format, Args... args) {
    ::snprintf(m_buffer, capacity(), format, args...);
    return static_cast<Derived &>(*this);
  }

  Derived &to_upper() {
    for (size_t i = 0; i < capacity(); i++) {
      m_buffer[i] = std::toupper(m_buffer[i]);
    }
    return static_cast<Derived &>(*this);
  }

  Derived &to_lower() {
    for (size_t i = 0; i < capacity(); i++) {
      m_buffer[i] = std::tolower(m_buffer[i]);
    }
    return static_cast<Derived &>(*this);
  }

  class Replace {
    API_AF(Replace, char, old_character, 0);
    API_AF(Replace, char, new_character, 0);
  };

  Derived &replace(const Replace &options) {
    for (size_t i = 0; i < capacity(); i++) {
      if (m_buffer[i] == options.old_character()) {
        m_buffer[i] = options.new_character();
      }
    }
    return static_cast<Derived &>(*this);
  }

  inline Derived &operator()(const Replace &options) {
    return replace(options);
  }

protected:
  StackString() { m_buffer[0] = 0; }
  StackString(const StringView a) {
    m_buffer[Size - 1] = 0;
    const size_t s = a.length() > Size - 1 ? Size - 1 : a.length();
    m_buffer[s] = 0;
    memcpy(m_buffer, a.data(), s);
  }

  StackString(const char *a) {
    m_buffer[Size - 1] = 0;
    strncpy(m_buffer, a, Size - 1);
  }

  constexpr size_t capacity() const { return Size - 1; }

  char m_buffer[Size];
};

class KeyString : public StackString<KeyString, 48> {
public:
  KeyString() {}
  KeyString(const StringView a) : StackString(a) {}
  KeyString(const char *a) : StackString(a) {}
  // implicit conversion
  operator const char *() const { return m_buffer; }
  operator const StringView() { return StringView(m_buffer); }
};

class NameString : public StackString<NameString, NAME_MAX + 1> {
public:
  NameString() {}
  NameString(const StringView a) : StackString(a) {}
  NameString(const char *a) : StackString(a) {}

  // implicit conversion
  operator const char *() const { return m_buffer; }
  operator const StringView() { return StringView(m_buffer); }
};

class PathString : public StackString<PathString, PATH_MAX> {
public:
  PathString() {}
  PathString(const StringView a) : StackString(a) {}
  PathString(const char *a) : StackString(a) {}

  PathString &operator/(const char *a) { return append("/").append(a); }

  PathString &operator/(const PathString &a) {
    append("/").append(a.cstring());
    return *this;
  }

  PathString &operator/(const NameString &a) {
    append("/").append(a.cstring());
    return *this;
  }

  PathString &operator/(const var::StringView a) {
    append("/").append(a);
    return *this;
  }

  var::StringView suffix() const;
  var::StringView name() const;
  var::StringView parent_directory() const;
  var::StringView base_name() const;
  var::StringView no_suffix() const;
  bool is_hidden() const;

  static var::StringView suffix(const StringView path);
  static var::StringView name(const StringView path);
  static var::StringView parent_directory(const StringView path);
  static var::StringView base_name(const StringView path);
  static var::StringView no_suffix(const StringView path);
  static bool is_hidden(const StringView path);

  // implicit conversion
  operator const char *() const { return m_buffer; }
  operator const StringView() { return StringView(m_buffer); }
};

class GeneralString : public StackString<GeneralString, 256> {
public:
  GeneralString() {}
  GeneralString(const StringView a) : StackString(a) {}
  GeneralString(const char *a) : StackString(a) {}
  // implicit conversion
  operator const char *() const { return m_buffer; }
  operator const StringView() { return StringView(m_buffer); }
};

class NumberString : public StackString<NumberString, 64> {
public:
  template <typename T> NumberString(T value) {
    // guarantee null termination
    m_buffer[capacity()] = 0;
    constexpr const char *fmt
      = (std::is_same<T, int>::value || std::is_same<T, short>::value
         || std::is_same<T, char>::value)
          ? "%d"
          : std::is_same<T, long>::value
              ? "%ld"
              : std::is_same<T, long long>::value
                  ? "%lld"
                  : (std::is_same<T, unsigned>::value
                     || std::is_same<T, unsigned short>::value
                     || std::is_same<T, unsigned char>::value)
                      ? "%u"
                      : std::is_same<T, unsigned long>::value
                          ? "%lu"
                          : std::is_same<T, unsigned long long>::value
                              ? "%lld"
                              : std::is_same<T, float>::value ? "%f" : nullptr;
    static_assert(fmt != nullptr, "NumberString can't handle type");

    snprintf(m_buffer, capacity(), fmt, value);
  }

  template <typename T> NumberString(T value, const char *format) {
    m_buffer[capacity()] = 0;
    ::snprintf(m_buffer, capacity(), format, value);
  }

  NumberString() {}
  NumberString(const StringView a) : StackString(a) {}
  NumberString(const char *a) : StackString(a) {}

  int to_integer() const;
  float to_float() const;
  long to_long(Base base = Base::decimal) const;
  unsigned long to_unsigned_long(Base base = Base::decimal) const;

  // implicit conversion
  operator const char *() const { return m_buffer; }
  operator const StringView() { return StringView(m_buffer); }
};

using StackString64 = NumberString;

} // namespace var

#endif // VARAPI_VAR_STACKSTRING_HPP
