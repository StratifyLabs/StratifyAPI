
#include "ux/Model.hpp"
#include "ux/Component.hpp"

using namespace ux;

Model::Model(){}

void Model::handle_event(const Event &event) {
  // update the entry if the event matches
  for (Entry &entry : entry_list()) {
    if (
      entry.event_type() == event.type() && event.component()
      && event.component()->name() == entry.key()) {
      entry.set_value(event.component()->get_model_value());
    }
  }
}

const var::String &Model::lookup(const var::String &key) const {
  return lookup(0, key);
}

const var::String &Model::lookup(u32 event_type, const var::String &key) const {
  for (const Entry &entry : entry_list()) {
    if (
      ((event_type == 0) || (entry.event_type() == event_type))
      && (key == entry.key())) {
      return entry.value();
    }
  }
  return var::String::empty_string();
}
