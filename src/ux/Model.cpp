
#include "ux/Model.hpp"
#include "ux/Component.hpp"

sys::Printer &operator<<(sys::Printer &printer, const ux::Model &a) {
  for (const ux::Model::Entry &entry : a.entry_list()) {
    printer.key(entry.key(), entry.value());
  }
  return printer;
}

using namespace ux;

Model::Model() {}

bool Model::Entry::operator==(const Entry &a) const {
  return (a.key() == key());
}

void Model::update(const Entry &value) {
  for (Entry &entry : entry_list()) {
    if (entry == value) {
      printf(
        "update model:%s -> %s\n",
        value.key().cstring(),
        value.value().cstring());
      entry.set_value(value.value());
      return;
    }
  }

  printf(
    "add model:%s -> %s\n",
    value.key().cstring(),
    value.value().cstring());
  entry_list().push_back(value);
}

const var::String &Model::lookup(const var::String &key) const {
  for (const Entry &entry : entry_list()) {
    if (key == entry.key()) {
      return entry.value();
    }
  }
  return var::String::empty_string();
}
