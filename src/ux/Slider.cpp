/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#include "ux/Slider.hpp"
#include "ux/EventLoop.hpp"
#include "ux/TouchGesture.hpp"
#include "ux/draw/Rectangle.hpp"

using namespace ux;
using namespace sgfx;

Slider::Slider(const var::String &name) : ComponentAccess(name) {
  var::StringList list = Model::to_list(lookup_model_value());
  set_value(list.count() ? list.at(0).to_integer() : 0);
  set_maximum(list.count() > 1 ? list.at(1).to_integer() : 100);
}

void Slider::draw(const DrawingScaledAttributes &attributes) {

  draw_base_properties(attributes.bitmap(), attributes.region(), theme());

  // draw the background
  attributes.bitmap() << sgfx::Pen().set_color(theme()->background_color());
  attributes.bitmap().draw_rectangle(attributes.region());

  Region region_inside_margin
    = calculate_region_inside_margin(attributes.region());

  Region slider_region
    = calculate_region_inside(region_inside_margin, {80, 5, 80, 5});

  Area indicator_area(
    region_inside_margin.width() * 100 / 1000,
    region_inside_margin.height());

  // draw the slider bar
  attributes.bitmap() << Pen().set_color(theme()->border_color());
  attributes.bitmap().draw_rectangle(slider_region);

  u16 indicator_range = region_inside_margin.width() - indicator_area.width();

  sg_int_t indicator_position = m_value * indicator_range / m_maximum;

  // draw the position indicator
  attributes.bitmap() << Pen().set_color(theme()->color());
  attributes.bitmap().draw_rectangle(
    region_inside_margin.point() + Point::X(indicator_position),
    indicator_area);

  apply_antialias_filter(attributes);
}

void Slider::handle_event(const ux::Event &event) {
  // change the state when an event happens in the component
  TouchContext *touch_context = TouchContext::match_component(event);
  if (touch_context) {

    if (
      (event.id() == TouchContext::event_id_pressed)
      && contains(touch_context->point())) {
      m_is_touched = true;
      update_touch_point(touch_context->point());
    }

    if (m_is_touched) {
      if (event.id() == TouchContext::event_id_released) {
        m_is_touched = false;
        update_touch_point(touch_context->point());
        trigger_event(event_id_released);
      } else if (event.id() == TouchContext::event_id_active) {
        // need to check for dragging
        update_touch_point(touch_context->point());
        trigger_event(event_id_active);
      }
    }

    if (
      (event.id() == TouchContext::event_id_pressed)
      && contains(touch_context->point())) {
      m_is_touched = true;
      update_touch_point(touch_context->point());
      trigger_event(event_id_pressed);
    }
  }

  Component::handle_event(event);
}

void Slider::update_touch_point(const sgfx::Point display_point) {
  DrawingPoint point = translate_point(display_point);

  if (point.x() < 50) {
    m_value = 0;
  } else if (point.x() > 950) {
    m_value = m_maximum;
  } else {
    m_value = (point.x() - 50) * m_maximum / 900;
  }

  update_model(Model::from_list<u16>({value(), maximum()}));
  redraw();
}
