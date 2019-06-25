/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


#ifndef DRAW_IMAGE_HPP_
#define DRAW_IMAGE_HPP_

#include "Drawing.hpp"


namespace draw {

/*! \brief Image Class
 * \details The Image class draws a bitmap image
 * on the drawing bitmap.
 */
class Image : public Drawing {
public:
	Image();
	Image(const sgfx::Bitmap & bitmap);

	Image & set_align_left(){ Drawing::set_align_left(); return *this; }
	Image & set_align_right(){ Drawing::set_align_right(); return *this; }
	Image & set_align_center(){ Drawing::set_align_center(); return *this; }
	Image & set_align_top(){ Drawing::set_align_top(); return *this; }
	Image & set_align_middle(){ Drawing::set_align_middle(); return *this; }
	Image & set_align_bottom(){ Drawing::set_align_bottom(); return *this; }

	void draw_to_scale(const DrawingScaledAttr & attr);

	drawing_area_t calc_area();

	/*! \details Sets the bitmap that is to be drawn as the image.
	 *
	 * @param bitmap A pointer to the bitmap to draw
	 */
	Image & set_bitmap(const sgfx::Bitmap * bitmap){
		m_bitmap = bitmap;
		return *this;
	}

private:
	const sgfx::Bitmap * m_bitmap;
};

} /* namespace draw */

#endif /* DRAW_IMAGE_HPP_ */
