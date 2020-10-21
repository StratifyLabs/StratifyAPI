#ifndef FSAPI_PATH_HPP
#define FSAPI_PATH_HPP

#include "var/String.hpp"
#include "var/StringView.hpp"

namespace fs {

class Path : public var::StackStringObject<Path, PATH_MAX> {
public:
  Path() {}
  explicit Path(var::StringView view)
    : var::StackStringObject<Path, PATH_MAX>(view) {}
  explicit Path(const var::String &str)
    : var::StackStringObject<Path, PATH_MAX>(str.string_view()) {}
  explicit Path(const char *a) : var::StackStringObject<Path, PATH_MAX>(a) {}

  Path &operator/(const char *a) { return append("/").append(a); }
  Path &operator/(const Path &a) { return append("/").append(a.cstring()); }
  Path &operator/(const var::StringView a) { return append("/").append(a); }
  Path &operator/(const var::String &a) {
    return append("/").append(a.cstring());
  }

  // implicit conversion
  operator const char *() const { return m_buffer; }
  operator const var::StringView() {
    return std::move(var::StringView(m_buffer));
  }

  /*! \details Returns the file suffix ('txt' for '/home/test.txt'). */
  var::StringView suffix() const;
  /*! \details Returns the file name ('test.txt' for '/home/test.txt'). */
  var::StringView name() const;
  /*! \details Returns the path to a file ('/home' for '/home/test.txt'). */
  var::StringView parent_directory() const;
  /*! \details Returns the base name of a file path('test' for
   * '/home/test.txt'). */
  var::StringView base_name() const;
  /*! \details Returns the path without the suffix ('/home/test' for
   * '/home/test.txt). */
  var::StringView no_suffix() const;

  /*! \details Returns true if the first character in any directory or file name
   * starts with `.`
   *
   * Examples:
   * - `/home/.DS_Store` returns true
   * - `/home/.library/data.txt` returns true
   * - `.DS_Store` returns true
   *
   *
   */
  bool is_hidden() const;

private:
};
} // namespace fs

#endif // FSAPI_PATH_HPP
