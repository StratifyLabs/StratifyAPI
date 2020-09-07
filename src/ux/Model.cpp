
#include "ux/Model.hpp"
#include "ux/Component.hpp"

sys::Printer &operator<<(sys::Printer &printer, const ux::Model &a) {
  for (const ux::Model::Entry &entry : a.entry_list()) {
    printer.key(entry.name(), entry.value());
  }
  return printer;
}

using namespace ux;

Model::Model() {}

bool Model::Entry::operator==(const Entry &a) const {
  return (a.name() == name());
}

void Model::update(const Entry &value) {
  for (Entry &entry : entry_list()) {
    if (entry == value) {
      entry.set_value(value.value());
      return;
    }
  }

  entry_list().push_back(value);
}

const var::String &Model::lookup(const var::String &name) const {
  for (const Entry &entry : entry_list()) {
    if (name == entry.name()) {
      return entry.value();
    }
  }
  return var::String::empty_string();
}
