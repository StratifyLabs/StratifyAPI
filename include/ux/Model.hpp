#ifndef SAPI_UX_MODEL_HPP
#define SAPI_UX_MODEL_HPP

#include "../var/String.hpp"
#include "Event.hpp"

namespace ux {

class Model
{
public:
  Model();

  class Entry {
    API_AF(Entry, u32, event_type, 0);
    API_AC(Entry, var::String, key);
    API_AC(Entry, var::String, value);
  };

  void handle_event(const Event &event);

  const var::String &lookup(u32 event_type, const var::String &key) const;
  const var::String &lookup(const var::String &key) const;

private:
  API_AC(Model, var::Vector<Entry>, entry_list);
};

} // namespace ux

#endif // SAPI_UX_MODEL_HPP
