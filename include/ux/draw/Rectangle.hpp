/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef SAPI_UX_DRAW_RECTANGLE_HPP_
#define SAPI_UX_DRAW_RECTANGLE_HPP_

#include "../Drawing.hpp"

namespace ux::draw {

/*! \brief Rectangle Class
 * \details The Rect class draws a rectangle on the display.  It can be used for
 * both horizontal and vertical separators as well.
 *
 * The following code snippet will draw a rectangle using color 1 in the
 * middle of the bitmap specified by attributes.
 *
 * ```
 * #include <sapi/draw.hpp>
 * DrawingAttributes attributes;
 *
 * //construct attributes
 *
 * Rectangle()
 *   .set_color(1)
 *   .draw(attributes, DrawingPoint(250,250), DrawingArea(500,500));
 * ```
 */
class Rectangle : public Drawing {
public:
  Rectangle();
  virtual void draw(const DrawingScaledAttributes &attr);

  Rectangle &set_color(sg_color_t value) {
    m_color = value;
    return *this;
  }

private:
  sg_color_t m_color;
};

/*! \brief Rect Class
 * \ingroup element
 * \details The Rect class draws a rectangle on the display.  It can be used for
 * both horizontal and vertical separators as well.
 */
class RoundedRectangle : public Drawing {
public:
  RoundedRectangle();
  virtual void draw(const DrawingScaledAttributes &attr);

  enum corners {
    corner_top_left_rounded = 0,
    corner_top_left_square = (1 << 0),
    corner_top_right_rounded = 0,
    corner_top_right_square = (1 << 1),
    corner_bottom_left_rounded = 0,
    corner_bottom_left_square = (1 << 2),
    corner_bottom_right_rounded = 0,
    corner_bottom_right_square = (1 << 3),
  };

  RoundedRectangle &set_corners(u8 corners) {
    m_corners = corners;
    return *this;
  }

  RoundedRectangle &set_radius(sg_size_t radius) {
    m_radius = radius;
    return *this;
  }

  RoundedRectangle &set_color(sg_color_t value) {
    m_color = value;
    return *this;
  }

private:
  /*! \cond */
  u8 m_corners = 0; // all rounded corners
  sg_color_t m_color;
  drawing_size_t m_radius = 25;
  /*! \endcond */
};

} // namespace ux::draw

#endif /* SAPI_UX_DRAW_RECTANGLE_HPP_ */
