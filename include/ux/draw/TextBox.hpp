/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef SAPI_UX_DRAW_TEXTBOX_HPP_
#define SAPI_UX_DRAW_TEXTBOX_HPP_

#include "../../var/Tokenizer.hpp"
#include "Text.hpp"

namespace ux::draw {

/*! \brief Text Box
 * \details This class is a scrolling text box that can be used to show
 * long text messages.
 *
 */
class TextBox : public Text {
public:
  /*! \details Construct a new text box */
  TextBox();

  static int
  count_lines(const sgfx::Font *font, const var::String &string, sg_size_t w);

  virtual void draw(const DrawingScaledAttributes &attr);

  static constexpr sg_size_t maximum_scroll() { return 65535; }

  TextBox &decrement_scroll() {
    if (m_scroll) {
      m_scroll--;
    }
    return *this;
  }

  TextBox &increment_scroll() {
    if (m_scroll < 65535) {
      m_scroll++;
    }
    return *this;
  }

private:
  API_AF(TextBox, sg_size_t, scroll, 0);
  /*! \cond */
  int count_lines(sg_size_t w);
  static void build_line(
    const sgfx::Font *font,
    u32 &i,
    var::String &line,
    const var::StringList &tokens,
    int &build_len,
    sg_size_t w);
  /*! \endcond */
};

} // namespace ux::draw

#endif /* SAPI_UX_DRAW_TEXTBOX_HPP_ */
