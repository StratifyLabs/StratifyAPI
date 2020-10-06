#include "FsAPI/Path.hpp"

using namespace fs;
using namespace var;

var::String Path::suffix() {
  size_t pos = path().reverse_find(".");

  if (pos == var::String::npos) {
    return var::String();
  }

  return String(path()).create_sub_string(
    String::CreateSubStringOptions().set_position(pos + 1));
}

var::String Path::name() {
  size_t pos = path().reverse_find('/');

  if (pos == var::String::npos) {
    return String(path());
  }

  return String(path()).create_sub_string(
    String::CreateSubStringOptions().set_position(pos + 1));
}

var::String Path::parent_directory() {
  size_t pos = path().reverse_find('/');

  if (pos == var::String::npos) {
    return var::String();
  }

  return String(path()).create_sub_string(
    String::CreateSubStringOptions().set_position(0).set_length(pos));
}

var::String Path::base_name() {
  var::String result = name();
  size_t pos = result.reverse_find(".");
  if (pos == String::npos) {
    return result;
  }

  return result.create_sub_string(
    String::CreateSubStringOptions().set_position(0).set_length(pos));
}

var::String Path::no_suffix() {
  size_t pos = path().reverse_find('.');

  if (pos == var::String::npos) {
    return var::String();
  }

  return String(path()).create_sub_string(
    String::CreateSubStringOptions().set_position(0).set_length(pos));
}

bool Path::is_hidden() {
  if (name().find(".") == 0) {
    return true;
  }

  var::String parent = parent_directory();
  if (parent != path()) {
    return Path(parent).is_hidden();
  }

  return false;
}
