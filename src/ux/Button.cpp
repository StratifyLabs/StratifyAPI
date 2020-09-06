/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#include "ux/Button.hpp"
#include "sys/Printer.hpp"
#include "ux/EventLoop.hpp"
#include "ux/draw/Rectangle.hpp"
#include "ux/draw/RichText.hpp"

using namespace sgfx;
using namespace ux;

void Button::draw(const DrawingScaledAttributes &attributes) {

  // draw the Border
  draw_base_properties(attributes.bitmap(), attributes.region(), theme());

  Region region_inside_padding
    = calculate_region_inside_padding(attributes.region());

  draw::RichText()
    .set_icon_font_name(theme()->primary_icon_font_name())
    .set_text_font_name(theme()->primary_font_name())
    .set_value(value())
    .set_color(theme()->text_color())
    .set_alignment(alignment())
    .draw(
      attributes + region_inside_padding.point()
      + region_inside_padding.area());

  apply_antialias_filter(attributes);
}

void Button::handle_event(const ux::Event &event) {
  // change the state when an event happens in the component
  TouchContext *touch_context = TouchContext::match_component(event);
  if (touch_context) {

    if (theme_state() != Theme::state_disabled) {

      if (event.id() == TouchContext::event_id_dragged) {
        if (theme_state() == Theme::state_highlighted) {
          set_theme_state(Theme::state_default);
          set_refresh_drawing_pending();
          m_hold_timer.stop();
        }
      }

      if (event.id() == TouchContext::event_id_released) {

        if (
          contains(touch_context->point())
          && (theme_state() == Theme::state_highlighted)) {
          toggle();
          m_hold_timer.stop();
          trigger_event(event_id_released);
        }

        if (theme_state() == Theme::state_highlighted) {
          m_hold_timer.stop();
          set_theme_state(Theme::state_default);
          set_refresh_drawing_pending();
        }
      }

      if (
        (event.id() == TouchContext::event_id_pressed)
        && contains(touch_context->point())) {
        toggle();
        trigger_event(event_id_pressed);
        m_hold_timer.restart();

        set_theme_state(Theme::state_highlighted);
        refresh_drawing();
      }
    }
  } else if (event.type() == SystemEvent::event_type()) {
    if (event.id() == SystemEvent::event_id_exiteded) {
      set_theme_state(Theme::state_default);
      set_refresh_drawing_pending();
      m_hold_timer.reset();
    } else if (event.id() == SystemEvent::event_id_updated) {
      if (
        m_hold_timer.is_running()
        && (m_hold_timer > theme()->button_hold_duration())) {
        m_hold_timer.stop();
        trigger_event(event_id_held);
      }
    }
  }
}
