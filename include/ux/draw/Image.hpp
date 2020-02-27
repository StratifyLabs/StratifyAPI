/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.


#ifndef SAPI_UX_DRAW_IMAGE_HPP_
#define SAPI_UX_DRAW_IMAGE_HPP_

#include "../Drawing.hpp"


namespace ux::draw {

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
class Image : public Drawing, public DrawingAlignment<Image> {
public:
	Image();
	Image(const sgfx::Bitmap & bitmap);

	/*! \details Sets the bitmap that is to be drawn as the image.
	 *
	 * @param bitmap A pointer to the bitmap to draw
	 * @return A reference to this object
	 */
	Image & set_bitmap(const sgfx::Bitmap * bitmap){
		m_bitmap = bitmap;
		return *this;
	}

	void draw_to_scale(const DrawingScaledAttributes & attr);


private:
	/*! \cond */
	const sgfx::Bitmap * m_bitmap;
	/*! \endcond */

};

} /* namespace draw */

#endif /* SAPI_DRAW_DRAW_IMAGE_HPP_ */
