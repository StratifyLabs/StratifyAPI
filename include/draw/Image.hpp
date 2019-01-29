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
	void draw_to_scale(const DrawingScaledAttr & attr);

	drawing_area_t calc_area();

	/*! \details Sets the bitmap that is to be drawn as the image.
	 *
	 * @param bitmap A pointer to the bitmap to draw
	 */
	void set_bitmap(const sgfx::Bitmap * bitmap){
		m_bitmap = bitmap;
	}

private:
	const sgfx::Bitmap * m_bitmap;
};

} /* namespace draw */

#endif /* DRAW_IMAGE_HPP_ */
