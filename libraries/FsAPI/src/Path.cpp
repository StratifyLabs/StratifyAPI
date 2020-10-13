#include "fs/Path.hpp"

using namespace fs;
using namespace var;

var::String Path::suffix() {
  size_t pos = path().reverse_find(".");

  if (pos == var::String::npos) {
    return var::String();
  }

  return std::move(
    String(path()).get_substring(String::GetSubstring().set_position(pos + 1)));
}

var::String Path::name() {
  size_t pos = path().reverse_find('/');

  if (pos == var::String::npos) {
    return String(path());
  }

  return std::move(
    String(path()).get_substring(String::GetSubstring().set_position(pos + 1)));
}

var::String Path::parent_directory() {
  size_t pos = path().reverse_find('/');

  if (pos == var::String::npos) {
    return var::String();
  }

  return std::move(String(path()).get_substring(
    String::GetSubstring().set_position(0).set_length(pos)));
}

var::String Path::base_name() { return std::move(Path(name()).no_suffix()); }

var::String Path::no_suffix() {
  size_t pos = path().reverse_find('.');

  return std::move(String(path()).get_substring(
    String::GetSubstring().set_position(0).set_length(pos)));
}

bool Path::is_hidden() {
  if (name().find(".") == 0) {
    return true;
  }

  var::String parent = parent_directory();
  if (parent != path()) {
    return Path(parent.cstring()).is_hidden();
  }

  return false;
}
