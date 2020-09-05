/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#include "ux/ToggleSwitch.hpp"
#include "ux/EventLoop.hpp"
#include "ux/draw/Rectangle.hpp"

using namespace ux;
using namespace sgfx;

void ToggleSwitch::draw(const DrawingScaledAttributes &attributes) {

  const sg_size_t border_offset
    = attributes.height() / 100 ? attributes.height() / 100 : 1;
  const sg_size_t active_area_offset = attributes.height() / 10;
  const sg_size_t toggle_area_offset
    = active_area_offset + attributes.height() / 10;
  const sg_size_t toggle_area_total_width
    = attributes.width() - toggle_area_offset * 2;
  const sg_size_t toggle_area_active_width = toggle_area_total_width * 60 / 100;
  const sg_size_t toggle_travel
    = toggle_area_total_width - toggle_area_active_width;

  const Area toggle_area(
    toggle_area_active_width,
    attributes.height() - toggle_area_offset * 2);

  // border
  attributes.bitmap() << Pen().set_color(theme()->border_color());
  attributes.bitmap().draw_rectangle(attributes.region());

  // default
  attributes.bitmap() << Pen().set_color(theme()->color());
  attributes.bitmap().draw_rectangle(
    attributes.point() + Point(border_offset, border_offset),
    attributes.area() - Area(border_offset * 2, border_offset * 2));

  // active background
  if (state() == true) {
    attributes.bitmap() << Pen().set_color(theme()->text_color());
  } else {
    attributes.bitmap() << Pen().set_color(theme()->border_color());
  }
  attributes.bitmap().draw_rectangle(
    attributes.point() + Point(active_area_offset, active_area_offset),
    attributes.area() - Area(active_area_offset * 2, active_area_offset * 2));

  sg_size_t toggle_point_location = 0;
  if (state() == true) {
    toggle_point_location = toggle_travel;
  }

  // default
  attributes.bitmap() << Pen().set_color(theme()->color());
  attributes.bitmap().draw_rectangle(
    attributes.point()
      + Point(toggle_area_offset + toggle_point_location, toggle_area_offset),
    toggle_area);

  apply_antialias_filter(attributes);
}

void ToggleSwitch::handle_event(const ux::Event &event) {
  // change the state when an event happens in the component
  if (event.type() == TouchGesture::Event::event_type()) {
    const TouchGesture::Event &touch_event
      = event.reinterpret<ux::TouchGesture::Event>();

    if (
      (touch_event.id() == TouchGesture::Event::id_released)
      && contains(touch_event.point())) {
      toggle();

      if (event_loop()) {
        event_loop()->trigger_event(Event(*this));
      }

      redraw();
    }
  }

  Component::handle_event(event);
}
