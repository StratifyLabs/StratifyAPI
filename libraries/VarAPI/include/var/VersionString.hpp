/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#ifndef VAR_API_VERSIONSTRING_HPP_
#define VAR_API_VERSIONSTRING_HPP_

#include "String.hpp"

namespace var {

class VersionString {
public:
  VersionString() {}

  u32 to_bcd() const;

  u16 to_bcd16() const { return to_bcd() >> 8; }

  VersionString from_triple(u16 major, u8 minor, u8 patch);
  VersionString from_u16(u16 major_minor);

  VersionString &set_string(StringView value) {
    m_version = String(value);
    return *this;
  }

  bool operator==(const VersionString &a) const {
    return compare(*this, a) == 0;
  }

  bool operator>(const VersionString &a) const { return compare(*this, a) > 0; }

  bool operator<(const VersionString &a) const { return compare(*this, a) < 0; }

  bool operator<=(const VersionString &a) const {
    return compare(*this, a) <= 0;
  }

  /*! \details Returns true if this version string
   * is greater than or equal to a.
   *
   * @param a version compared against.
   *
   * See compare() for more details.
   *
   */
  bool operator>=(const VersionString &a) const {
    return compare(*this, a) >= 0;
  }

  String &string() { return m_version; }
  const String &string() const { return m_version; }

  /*! \details Compares two version strings.
   *
   * @param a first string
   * @param b second string
   * @return 1 if a > b, 0 if a == b or -1 if a < b
   *
   * If a is formatted as x.y.z and b is formatted
   * as x.y then a is greater than b and 1 will be returned. If b
   * uses x.y.z and a uses x.y then -1 will be returned.
   *
   * Generally, a project uses one format or the other
   * and the two formats should not need to be compared.
   *
   * Also, an empty version is always less than a version that
   * is populated.
   *
   */
  static int compare(const VersionString &a, const VersionString &b);

private:
  String m_version;
};
} // namespace var

#endif // VAR_API_VERSIONSTRING_HPP_
