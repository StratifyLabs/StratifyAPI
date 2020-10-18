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
  explicit Path(const char *a) : var::StackStringObject<Path, PATH_MAX>(a) {}

  bool operator==(const Path &a) const {
    return strncmp(cstring(), a.cstring(), PATH_MAX) == 0;
  }

  bool operator!=(const Path &a) const {
    return strncmp(cstring(), a.cstring(), PATH_MAX) != 0;
  }

  var::StringView path() { return cstring(); }

  /*! \details Returns the file suffix ('txt' for '/home/test.txt'). */
  var::StringView suffix();
  /*! \details Returns the file name ('test.txt' for '/home/test.txt'). */
  var::StringView name();
  /*! \details Returns the path to a file ('/home' for '/home/test.txt'). */
  var::StringView parent_directory();
  /*! \details Returns the base name of a file path('test' for
   * '/home/test.txt'). */
  var::StringView base_name();
  /*! \details Returns the path without the suffix ('/home/test' for
   * '/home/test.txt). */
  var::StringView no_suffix();

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
  bool is_hidden();

private:
};
} // namespace fs

#endif // FSAPI_PATH_HPP
