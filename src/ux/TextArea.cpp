
#include "ux/TextArea.hpp"
#include "sgfx.hpp"
#include "ux/draw/TextBox.hpp"

using namespace ux;

void TextArea::draw(const DrawingScaledAttributes &attributes) {
  draw_base_properties(attributes.bitmap(), attributes.region(), theme());

  Region region_inside_padding
    = calculate_region_inside_padding(attributes.region());

  // calculate the font point size
  u32 line_height = text_height() * region_inside_padding.height() * 9 / 10000;

  // if the label is available, draw it
  draw::TextBox()
    .set_string(value())
    .set_font_name(theme()->primary_font_name())
    .set_font_point_size(line_height)
    .set_color(theme()->text_color())
    .set_alignment(alignment())
    .draw(
      attributes + region_inside_padding.point()
      + region_inside_padding.area());

  apply_antialias_filter(attributes);
}

void TextArea::handle_event(const ux::Event &event) {}
