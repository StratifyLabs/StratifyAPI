#ifndef VARAPI_VAR_STACKSTRING_HPP
#define VARAPI_VAR_STACKSTRING_HPP

#include <sys/syslimits.h>

#include "StringView.hpp"

namespace var {

template <class Derived, int Size> class StackStringObject {
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

  bool operator==(const char *a) { return strncmp(m_buffer, a, Size - 1) == 0; }
  bool operator!=(const char *a) { return strncmp(m_buffer, a, Size - 1) != 0; }

  // explicit conversion
  const char *cstring() const { return m_buffer; }
  const StringView string_view() const { return StringView(m_buffer); }

  // implicit conversion
  operator const char *() const { return m_buffer; }
  operator StringView() { return StringView(m_buffer); }

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
  StackStringObject() { m_buffer[0] = 0; }
  StackStringObject(const StringView a) {
    m_buffer[Size - 1] = 0;
    const size_t s = a.length() > Size - 1 ? Size - 1 : a.length();
    m_buffer[s] = 0;
    memcpy(m_buffer, a.data(), s);
  }

  StackStringObject(const char *a) {
    m_buffer[Size - 1] = 0;
    strncpy(m_buffer, a, Size - 1);
  }

  constexpr size_t capacity() const { return Size - 1; }

  char m_buffer[Size];
};

class KeyString : public StackStringObject<KeyString, 32> {
public:
  KeyString() {}
  KeyString(const StringView a) : StackStringObject(a) {}
  KeyString(const char *a) : StackStringObject(a) {}
};

class StackString128 : public StackStringObject<StackString128, 128> {
public:
  StackString128() {}
  StackString128(const StringView a) : StackStringObject(a) {}
  StackString128(const char *a) : StackStringObject(a) {}
};

class StackString256 : public StackStringObject<StackString256, 256> {
public:
  StackString256() {}
  StackString256(const StringView a) : StackStringObject(a) {}
  StackString256(const char *a) : StackStringObject(a) {}
};

class NumberString : public StackStringObject<NumberString, 64> {
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
  NumberString(const StringView a) : StackStringObject(a) {}
  NumberString(const char *a) : StackStringObject(a) {}

  int to_integer() const;
  float to_float() const;
  long to_long(Base base = Base::decimal) const;
  unsigned long to_unsigned_long(Base base = Base::decimal) const;
};

using StackString64 = NumberString;
using StackString32 = KeyString;
using Ntos = NumberString;

} // namespace var

#endif // VARAPI_VAR_STACKSTRING_HPP
