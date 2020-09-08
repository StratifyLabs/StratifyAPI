
#include "ux/TextArea.hpp"
#include "sgfx.hpp"
#include "ux/TouchGesture.hpp"
#include "ux/draw/TextBox.hpp"

using namespace ux;

TextArea::TextArea(const var::String &name) : ComponentAccess(name) {}

void TextArea::draw(const DrawingScaledAttributes &attributes) {
  draw_base_properties(attributes.bitmap(), attributes.region(), theme());

  Region region_inside_padding
    = calculate_region_inside_padding(attributes.region());

  update_model(value());

  // calculate the font point size scaled from drawing size to sgfx size
  m_line_height = text_height() * region_inside_padding.height() * 9 / 10000;

  // if the label is available, draw it

  m_text_box.set_font(nullptr)
    .set_string(get_display_text())
    .set_font_name(theme()->primary_font_name())
    .set_font_point_size(m_line_height)
    .set_color(theme()->text_color())
    .set_alignment(alignment())
    .draw(
      attributes + region_inside_padding.point()
      + region_inside_padding.area());

  apply_antialias_filter(attributes);
}

TextArea &TextArea::set_value(const var::String &value) {

  // if the value is too big --truncate it
  if (value.length() > max_length()) {
    update_model(value.create_sub_string(
      var::String::Position(value.length() - max_length())));
  } else {
    update_model(value);
  }
  return *this;
}

var::String TextArea::get_display_text() {
  // convert text to display text
  var::String result;

  // may want to convert to HEX format
  result = value();

  return result;
}

void TextArea::handle_event(const ux::Event &event) {
  TouchContext *touch_context = event.is_trigger<TouchContext>();
  if (touch_context) {
    if (event.id() == TouchContext::event_id_dragged_point) {
      if (contains(touch_context->point())) {

        sg_size_t line_compare = m_line_height / 2;
        m_scroll_accumulator += touch_context->drag().y();

        sg_size_t scroll = m_text_box.scroll();
        while (m_scroll_accumulator > line_compare) {
          m_scroll_accumulator -= line_compare;
          m_text_box.decrement_scroll();
        }

        while (m_scroll_accumulator < -1 * line_compare) {
          m_scroll_accumulator += line_compare;
          m_text_box.increment_scroll();
        }

        if (scroll != m_text_box.scroll()) {
          redraw();
        }
      }

    } else if (event.id() == TouchContext::event_id_released) {
      m_scroll_accumulator = 0;
    }
  }
}
