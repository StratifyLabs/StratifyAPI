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
  if (event.type() == ux::TouchEvent::event_type()) {

    if (theme_state() != Theme::state_disabled) {

      const ux::TouchEvent &touch_event = event.reinterpret<ux::TouchEvent>();

      if (touch_event.id() == ux::TouchEvent::id_dragged) {
        if (theme_state() == Theme::state_highlighted) {
          set_theme_state(Theme::state_default);
          set_refresh_drawing_pending();
          m_hold_timer.stop();
        }
      }

      if (touch_event.id() == ux::TouchEvent::id_released) {

        if (
          contains(touch_event.point())
          && (theme_state() == Theme::state_highlighted)) {
          toggle();
          m_hold_timer.stop();
          event_loop()->trigger_event(
            ButtonEvent(ButtonEvent::id_released, *this));
        }

        if (theme_state() == Theme::state_highlighted) {
          m_hold_timer.stop();
          set_theme_state(Theme::state_default);
          set_refresh_drawing_pending();
        }
      }

      if (
        (touch_event.id() == ux::TouchEvent::id_pressed)
        && contains(touch_event.point())) {
        toggle();
        event_loop()->trigger_event(
          ButtonEvent(ButtonEvent::id_pressed, *this));

        m_hold_timer.restart();

        set_theme_state(Theme::state_highlighted);
        refresh_drawing();
      }
    }
  } else if (event.type() == SystemEvent::event_type()) {
    if (event.id() == SystemEvent::id_exit) {
      set_theme_state(Theme::state_default);
      set_refresh_drawing_pending();
      m_hold_timer.reset();
    } else if (event.id() == SystemEvent::id_update) {
      if (
        m_hold_timer.is_running()
        && (m_hold_timer > theme()->button_hold_duration())) {
        m_hold_timer.stop();
        event_loop()->trigger_event(ButtonEvent(ButtonEvent::id_held, *this));
      }
    }
  }
}
