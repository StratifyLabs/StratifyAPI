#include "fs/Path.hpp"
#include "printer/Printer.hpp"

using namespace fs;
using namespace var;

var::StringView Path::suffix() const {
  size_t pos = string_view().reverse_find(".");

  if (pos == var::String::npos) {
    return var::StringView();
  }

  return StringView(cstring() + pos + 1);
}

var::StringView Path::name() const {
  size_t pos = string_view().reverse_find('/');

  if (pos == var::String::npos) {
    return string_view();
  }

  return StringView(cstring() + pos + 1);
}

var::StringView Path::parent_directory() const {
  size_t pos = string_view().reverse_find('/');

  if (pos == var::String::npos) {
    return var::StringView();
  }

  return StringView(cstring(), pos);
}

var::StringView Path::base_name() const { return Path(name()).no_suffix(); }

var::StringView Path::no_suffix() const {
  size_t pos = string_view().reverse_find('.');

  if (pos == StringView::npos) {
    return string_view();
  }

  return StringView(cstring(), pos);
}

bool Path::is_hidden() const {
  if (name().find(".") == 0) {
    return true;
  }

  var::StringView parent = parent_directory();
  if (parent != string_view()) {
    return Path(parent).is_hidden();
  }

  return false;
}
