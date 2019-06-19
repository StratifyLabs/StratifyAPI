/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_PROGRESS_HPP_
#define DRAW_PROGRESS_HPP_

#include "Drawing.hpp"

namespace draw {

typedef struct MCU_PACK {
	u16 value;
	u16 maximum;
} progress_t;

/*! \brief Progress Class
 * \ingroup element
 * \details The Progress Class defines the top level object for progress bars etc.
 */
class Progress : public Drawing {
public:
	Progress(){
		m_border_thickness = 25;
	}

	/*! \details Sets the progress.
	 *
	 * @param value value from 0 to maximum
	 * @param maximum maximum possible for value
	 * @return A reference to this object
	 *
	 */
	Progress & set_progress(u16 value, u16 maximum){
		m_progress.value = value; m_progress.maximum = maximum; return *this;
	}

	/*! \details Returns the value of the progress. */
	u16 value() const { return m_progress.value; }

	/*! \details Returns the maximum value of the progress. */
	u16 maximum() const { return m_progress.maximum; }

	/*! \details Sets the border thickness.
	 *
	 * @param border_thickness value from 0 to 100 representing a percentage of height/2.
	 *
	 * If border_thickness is 50, the border will take up 50% of the top half and 50% of
	 * the bottom half of the progress bar.
	 *
	 */
	Progress & set_border_thickness(drawing_size_t border_thickness){
		m_border_thickness = border_thickness; return *this;
	}

	/*! \details Sets the color of the progress. */
	Progress & set_color(sg_color_t value){
		m_color = value; return *this;
	}

	/*! \details Sets the background color.
	 *
	 * @param value The background color value.
	 *
	 * If the value is draw::color_transparent(),
	 * the background is not drawn.
	 *
	 */
	Progress & set_background_color(sg_color_t value){
		m_background_color = value; return *this;
	}

	/*! \details Returns the border thickness. */
	drawing_size_t border_thickness() const { return m_border_thickness; }

	/*! \details Returns the color. */
	sg_color_t color() const { return m_color; }

	/*! \details Returns the background color. */
	sg_color_t background_color() const { return m_background_color; }

private:
	progress_t m_progress;
	drawing_size_t m_border_thickness;
	sg_color_t m_color;
	sg_color_t m_background_color;

};

}

#endif /* DRAW_PROGRESS_HPP_ */
