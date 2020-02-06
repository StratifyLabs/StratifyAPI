/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.


#ifndef SAPI_DRAW_IMAGE_HPP_
#define SAPI_DRAW_IMAGE_HPP_

#include "Drawing.hpp"


namespace draw {

/*! \brief Image Class
 * \details The Image class draws a bitmap image
 * on the drawing bitmap.
 *
 * The Image is not scaled to the drawing attributes region.
 * It can be aligned within the drawing region using the methods
 * defined in this class.
 *
 *
 */
class Image : public Drawing {
public:
	Image();
	Image(const sgfx::Bitmap & bitmap);

	/*! \details Aligns the image to the left side of the drawing region. */
	Image & set_align_left(){ Drawing::set_align_left(); return *this; }
	/*! \details Aligns the image to the right side of the drawing region. */
	Image & set_align_right(){ Drawing::set_align_right(); return *this; }
	/*! \details Aligns the image in the center (horizontally) of the drawing region. */
	Image & set_align_center(){ Drawing::set_align_center(); return *this; }
	/*! \details Aligns the image to the top side of the drawing region. */
	Image & set_align_top(){ Drawing::set_align_top(); return *this; }
	/*! \details Aligns the image to the middle (vertically) of the drawing region. */
	Image & set_align_middle(){ Drawing::set_align_middle(); return *this; }
	/*! \details Aligns the image to the bottom side of the drawing region. */
	Image & set_align_bottom(){ Drawing::set_align_bottom(); return *this; }

	/*! \details Sets the bitmap that is to be drawn as the image.
	 *
	 * @param bitmap A pointer to the bitmap to draw
	 * @return A reference to this object
	 */
	Image & set_bitmap(const sgfx::Bitmap * bitmap){
		m_bitmap = bitmap;
		return *this;
	}

	void draw_to_scale(const DrawingScaledAttr & attr);


private:
	/*! \cond */
	const sgfx::Bitmap * m_bitmap;
	/*! \endcond */

};

} /* namespace draw */

#endif /* SAPI_DRAW_IMAGE_HPP_ */
