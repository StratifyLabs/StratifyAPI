//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SGFX_FONTSYSTEM_HPP_
#define SGFX_FONTSYSTEM_HPP_

#include <sgfx/sg_font_types.h>

#include "sgfx/FontFile.hpp"
#include "sgfx/FontMemory.hpp"

namespace sgfx {

class FontSystem {
public:

	/*! \details This sets the system font pointer.  This function is needed
	 * in order for load_system_font() to function.
	 *
	 * @param fonts A pointer to a table that holds the system fonts
	 * @return True if all fonts loaded successfully
	 */
	static bool load_fonts(const sg_font_ref_t * fonts);


	/*! \details Load the system font that most closely matches the
	 * specified characteristics
	 *
	 * @param h The target font height.  This is a maximum height unless \a h is small than all system fonts
	 * @param bold Whether or not to use a bold font
	 * @return
	 */
	static FontObject * get_font(sg_size_t h, bool bold = false);


private:
	static const sg_font_ref_t * m_system_fonts;
	static FontObject ** m_font_array;
};

};

#endif /* SGFX_FONTSYSTEM_HPP_ */
