/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#ifndef SAPI_UX_LIST_HPP
#define SAPI_UX_LIST_HPP

#include "Layout.hpp"
#include "TouchGesture.hpp"

namespace ux {

class ListItem : public ComponentAccess<ListItem> {
public:
  class Event : public EventObject<ListItem, EVENT_TYPE('_', 'l', 's', 't')> {
  public:
    enum ids { id_none, id_select };

    explicit Event(ListItem &value) : EventObject(id_select, &value) {}
  };

  COMPONENT_PREFIX(ListItem)

  ListItem(const var::String &name) : ComponentAccess(prefix() + name) {
    set_left_padding(5);
    set_right_padding(5);
  }

  void draw(const DrawingScaledAttributes &attributes);
  void handle_event(const ux::Event &event);

protected:
  ListItem(const var::String &prefix, const var::String &name)
    : ComponentAccess(prefix + name) {
    set_left_padding(5);
    set_right_padding(5);
  }

  virtual var::String get_model_data() const { return value(); }

private:
  API_ACCESS_COMPOUND(ListItem, var::String, key);
  API_ACCESS_COMPOUND(ListItem, var::String, value);
  API_ACCESS_BOOL(ListItem, interactive, true);
};

template <class T> class ListItemAccess : public ListItem {
public:
  ListItemAccess(const var::String &name) : ListItem("", name) {}

  API_ACCESS_DERIVED_COMPOUND(ListItem, T, var::String, key)
  API_ACCESS_DERIVED_COMPOUND(ListItem, T, var::String, value)
  API_ACCESS_DERIVED_BOOL(ListItem, T, interactive)

  COMPONENT_ACCESS_DERIVED(T)
  COMPONENT_ACCESS_CREATE()
};

class List : public LayoutAccess<List> {
public:
  COMPONENT_PREFIX(List)

  List(const var::String &name, EventLoop *event_loop)
    : LayoutAccess(prefix() + name, event_loop) {
    set_flow(flow_vertical);
  }

  List &add_component(Component &component);

  List &
  add_filler(enum sgfx::Theme::styles style = sgfx::Theme::style_outline_dark);

private:
  API_ACCESS_FUNDAMENTAL(List, drawing_size_t, item_height, 250);
  API_ACCESS_FUNDAMENTAL(List, u8, border_size, 1);
};

} // namespace ux

#endif // SAPI_UX_LIST_HPP
