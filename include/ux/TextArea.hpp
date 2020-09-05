#ifndef SAPI_UX_TEXTAREA_HPP
#define SAPI_UX_TEXTAREA_HPP

#include "Component.hpp"

namespace ux {

class TextArea : public ComponentAccess<TextArea> {
public:
  TextArea(const var::String &name) : ComponentAccess(name) {}

  void draw(const DrawingScaledAttributes &attributes);
  void handle_event(const ux::Event &event);

private:
  API_ACCESS_COMPOUND(TextArea, var::String, value);
  API_ACCESS_FUNDAMENTAL(TextArea, drawing_size_t, text_height, 0);
};

} // namespace ux

#endif // SAPI_UX_TEXTAREA_HPP
