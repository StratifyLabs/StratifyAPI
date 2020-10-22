#ifndef FSAPI_PATH_HPP
#define FSAPI_PATH_HPP

#include "var/StringView.hpp"

namespace fs {

class Path {
public:
  explicit Path(var::StringView view) : m_path(view) {}
  explicit Path(const char *a) : m_path(a) {}

  var::StringView suffix() const;
  var::StringView name() const;
  var::StringView parent_directory() const;
  var::StringView base_name() const;
  var::StringView no_suffix() const;

  bool is_hidden() const;

private:
  var::StringView m_path;
};
} // namespace fs

#endif // FSAPI_PATH_HPP
