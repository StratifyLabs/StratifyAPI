#ifndef FSAPI_PATH_HPP
#define FSAPI_PATH_HPP

#include "VarAPI/String.hpp"
#include "VarAPI/StringView.hpp"

namespace fs {

class Path {
public:
  Path(var::StringView view) : m_path(view) {}

  var::StringView path() const { return m_path; }

  /*! \details Returns the file suffix ('txt' for '/home/test.txt'). */
  var::String suffix();
  /*! \details Returns the file name ('test.txt' for '/home/test.txt'). */
  var::String name();
  /*! \details Returns the path to a file ('/home' for '/home/test.txt'). */
  var::String parent_directory();
  /*! \details Returns the base name of a file path('test' for
   * '/home/test.txt'). */
  var::String base_name();
  /*! \details Returns the path without the suffix ('/home/test' for
   * '/home/test.txt). */
  var::String no_suffix();

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
  var::StringView m_path;
};
} // namespace fs

#endif // FSAPI_PATH_HPP
