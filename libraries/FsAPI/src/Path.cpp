#include "fs/Path.hpp"

using namespace fs;
using namespace var;

var::StringView Path::suffix() {
  size_t pos = path().reverse_find(".");

  if (pos == var::String::npos) {
    return var::String();
  }

  return StringView(cstring() + pos + 1);
}

var::StringView Path::name() {
  size_t pos = path().reverse_find('/');

  if (pos == var::String::npos) {
    return String(path());
  }

  return StringView(cstring() + pos + 1);
}

var::StringView Path::parent_directory() {
  size_t pos = path().reverse_find('/');

  if (pos == var::String::npos) {
    return var::String();
  }

  return StringView(cstring(), pos);
}

var::StringView Path::base_name() { return Path(name()).no_suffix(); }

var::StringView Path::no_suffix() {
  size_t pos = path().reverse_find('.');

  if (pos == StringView::npos) {
    return StringView(cstring());
  }

  return StringView(cstring(), pos);
}

bool Path::is_hidden() {
  if (name().find(".") == 0) {
    return true;
  }

  var::StringView parent = parent_directory();
  if (parent != path()) {
    return Path(parent).is_hidden();
  }

  return false;
}
