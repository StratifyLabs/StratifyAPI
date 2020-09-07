/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#ifndef SAPI_UX_LIST_HPP
#define SAPI_UX_LIST_HPP

#include "Layout.hpp"
#include "TouchGesture.hpp"

namespace ux {

class ListItem : public ComponentAccess<ListItem> {
public:
  enum event_ids { event_id_none, event_id_selected };

  ListItem(const var::String &name) : ComponentAccess(name) {
    set_left_padding(5);
    set_right_padding(5);
    set_value(lookup_model_value());
  }

  void draw(const DrawingScaledAttributes &attributes);
  void handle_event(const ux::Event &event);

private:
  API_ACCESS_COMPOUND(ListItem, var::String, label);
  API_ACCESS_COMPOUND(ListItem, var::String, value);
  API_ACCESS_BOOL(ListItem, interactive, true);
};

template <class T> class ListItemAccess : public ListItem {
public:
  ListItemAccess(const var::String &name) : ListItem(name) {}

  API_ACCESS_DERIVED_COMPOUND(ListItem, T, var::String, label)
  API_ACCESS_DERIVED_COMPOUND(ListItem, T, var::String, value)
  API_ACCESS_DERIVED_BOOL(ListItem, T, interactive)

  COMPONENT_ACCESS_DERIVED(Component, T)

  COMPONENT_ACCESS_CREATE()
};

class List : public LayoutAccess<List> {
public:
  List(const var::String &name, EventLoop *event_loop) : LayoutAccess(name) {
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
