//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SGFX_FONTSYSTEM_HPP_
#define SGFX_FONTSYSTEM_HPP_

#include <sapi/sg_font_types.h>

#include "FontFile.hpp"
#include "FontMemory.hpp"

namespace sgfx {

/*! \brief Font System Class
 * \details This class is used to manage fonts that are installed
 * on the system.
 */
class FontSystem {
public:

	/*! \details This sets the system font pointer.  This function is needed
	 * in order for load_system_font() to function.
	 *
	 * @param fonts A pointer to a table that holds the system fonts
	 * @return True if all fonts loaded successfully
	 */
	static bool load_fonts(const sg_font_ref_t * fonts, int count);


	/*! \details Load the system font that most closely matches the
	 * specified characteristics
	 *
	 * @param h The target font height.  This is a maximum height unless \a h is small than all system fonts
	 * @param bold Whether or not to use a bold font
	 * @return
	 */
	static Font * get_font(sg_size_t h, bool bold);


	/*! \details Load the specified font
	 *
	 * @param num The offset of the system font
	 * @return A pointer to the font
	 *
	 * Here is an example of how to adjust the spacing of
	 * the system fonts.
	 *
	 * \code
	 * int i;
	 * for(i = 0; i < FontSystem::count(); i++){
	 * 	Font * font = FontSystem::font(i);
	 * 	font->set_space_size(4);
	 * }
	 * \endcode
	 *
	 */
	static Font * font(int num){
		return num < m_font_count ? m_font_array[num] : 0;
	}


	/*! \details Returns the number of system fonts available */
	static int count(){ return m_font_count; }

private:
	static const sg_font_ref_t * m_system_fonts;
	static Font ** m_font_array;
	static int m_font_count;
};

};

#endif /* SGFX_FONTSYSTEM_HPP_ */
