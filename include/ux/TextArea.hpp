#ifndef SAPI_UX_TEXTAREA_HPP
#define SAPI_UX_TEXTAREA_HPP

#include "Component.hpp"
#include "TouchGesture.hpp"
#include "draw/TextBox.hpp"

namespace ux {

class TextArea : public ComponentAccess<TextArea> {
public:
  TextArea(const var::String &name);

  void draw(const DrawingScaledAttributes &attributes);
  void handle_event(const ux::Event &event);

  TextArea &append(const var::String &input) {
    m_value += input;
    return *this;
  }

private:
  API_ACCESS_COMPOUND(TextArea, var::String, value);
  API_ACCESS_FUNDAMENTAL(TextArea, drawing_size_t, text_height, 0);
  API_ACCESS_FUNDAMENTAL(TextArea, drawing_size_t, offset, 0);

  sg_int_t m_scroll_accumulator = 0;
  sg_size_t m_line_height = 0;
  draw::TextBox m_text_box;
  var::String get_display_text();
};

} // namespace ux

#endif // SAPI_UX_TEXTAREA_HPP
