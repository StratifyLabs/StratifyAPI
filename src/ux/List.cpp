/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#include "ux/List.hpp"
#include "ux/EventLoop.hpp"
#include "ux/Separator.hpp"
#include "ux/draw/Rectangle.hpp"
#include "ux/draw/RichText.hpp"

using namespace sgfx;
using namespace ux;

void ListItem::draw(const DrawingScaledAttributes &attributes) {

  draw_base_properties(attributes.bitmap(), attributes.region(), theme());

  Region region_inside_padding
    = calculate_region_inside_padding(attributes.region());

  draw::RichText()
    .set_icon_font_name(theme()->primary_icon_font_name())
    .set_text_font_name(theme()->primary_font_name())
    .set_value(key())
    .set_color(theme()->text_color())
    .set_align_left()
    .set_align_middle()
    .draw(
      attributes + region_inside_padding.point()
      + region_inside_padding.area());

  draw::RichText()
    .set_icon_font_name(theme()->primary_icon_font_name())
    .set_text_font_name(theme()->primary_font_name())
    .set_value(value())
    .set_color(theme()->text_color())
    .set_align_right()
    .set_align_middle()
    .draw(
      attributes + region_inside_padding.point()
      + region_inside_padding.area());
}

void ListItem::handle_event(const ux::Event &event) {
  // change the state when an event happens in the component
  if (event.type() == TouchGesture::Event::event_type()) {
    const TouchGesture::Event &touch_event
      = event.reinterpret<TouchGesture::Event>();

    if (touch_event.id() == TouchGesture::Event::id_dragged) {
      if (theme_state() == Theme::state_highlighted) {
        set_theme_state(Theme::state_default);
        set_refresh_drawing_pending();
      }
    }

    if (is_interactive()) {
      if (touch_event.id() == TouchGesture::Event::id_released) {
        if (theme_state() == Theme::state_highlighted) {
          if (contains(touch_event.point())) {
            event_loop()->trigger_event(ListItem::Event(*this));
          }
          set_theme_state(Theme::state_default);
          set_refresh_drawing_pending();
        }
      }

      if (touch_event.id() == TouchGesture::Event::id_pressed) {
        if (contains(touch_event.point())) {
          set_theme_state(Theme::state_highlighted);
          set_refresh_drawing_pending();
        }
      }
    }
  }
  Component::handle_event(event);
}

List &List::add_component(Component &component) {

  // to ensure items overlap a tiny bit, we add a little fudge factor
  component.set_drawing_area(
    DrawingArea(1000, m_item_height + m_item_height / 25));
  // component.set_theme_style( theme_style() );
  return LayoutAccess<List>::add_component(component);
}

List &List::add_filler(enum sgfx::Theme::styles style) {

  drawing_size_t height = component_list().count()
                          * component_list()
                              .back()
                              .component()
                              ->reference_drawing_attributes()
                              .area()
                              .height();

  if (height < 1010) {
    Separator *list_filler = &(Separator::create(var::String(name()) + "Filler")
                                 .set_drawing_area(1000, 1010 - height)
                                 .set_theme_style(style)
                                 .set_left_border(0)
                                 .set_right_border(0));
    // not scrolling -- we don't fill the whole screen
    set_vertical_scroll_enabled(false);
    return LayoutAccess<List>::add_component(*list_filler);
  }

  return *this;
}
