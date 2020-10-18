#ifndef VARAPI_VAR_STACKSTRING_HPP
#define VARAPI_VAR_STACKSTRING_HPP

#include <sys/syslimits.h>

#include "StringView.hpp"

namespace var {

template <class Derived, int Size> class StackStringObject {
public:
  const char *cstring() const { return m_buffer; }
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

private:
  char m_buffer[Size];
};

class StackString32 : public StackStringObject<StackString32, 32> {
public:
  StackString32(const StringView a) : StackStringObject(a) {}
  StackString32(const char *a) : StackStringObject(a) {}
};

class StackString64 : public StackStringObject<StackString64, 64> {
public:
  StackString64(const StringView a) : StackStringObject(a) {}
  StackString64(const char *a) : StackStringObject(a) {}
};

class StackString128 : public StackStringObject<StackString128, 128> {
public:
  StackString128(const StringView a) : StackStringObject(a) {}
};

class StackString256 : public StackStringObject<StackString256, 256> {
public:
  StackString256(const StringView a) : StackStringObject(a) {}
  StackString256(const char *a) : StackStringObject(a) {}
};

} // namespace var

#endif // VARAPI_VAR_STACKSTRING_HPP
