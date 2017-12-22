/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#ifndef SYS_ASSETS_HPP_
#define SYS_ASSETS_HPP_

#include "../sgfx/Font.hpp"
#include "../sgfx/Vector.hpp"

namespace sys {

class Assets {
public:

	/*! \details Initializes system assets.
	 *
	 * @return Zero
	 */
	static int init();


	/*! \details Load the system font that most closely matches the
	 * specified characteristics
	 *
	 * @param h The target font height.  This is a maximum height unless \a h is smaller than all system fonts
	 * @param bold Whether or not to use a bold font
	 * @return
	 */
	static sgfx::Font * get_font(sg_size_t h, bool bold);


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
	 * for(i = 0; i < Assets::font_count(); i++){
	 * 	sgfx::Font * font = Assets::font(i);
	 * 	font->set_space_size(4);
	 * }
	 * \endcode
	 *
	 */
	static sgfx::Font * font(int num){
		return num < m_font_count ? m_font_array[num] : 0;
	}


	/*! \details Returns the number of system fonts available */
	static int font_count(){ return m_font_count; }


	/*! \details Load the system icon specified by the location in the icon table.
	 *
	 * @param icon The icon to load
	 * @return
	 */
	static const sg_vector_icon_t * get_vector_icon(u16 icon);

	/*! \details Accesses the number of vector icons available from the system. */
	static int vector_icon_count(){ return m_vector_icon_count; }

	/*! \details Accesses the specified system bitmap icon.
	 *
	 * @param icon The icon number (offset) to access
	 * @return A pointer to the bitmap header or null \a icon is not valid
	 */
	static const sg_bmap_header_t * get_bmap_icon(u16 icon);

	/*! \details Accesses the number of bitmap icons available from the system.*/
	static int bmap_icon_count(){ return m_bmap_icon_count; }

private:

	static void check_initialized();

	static bool load_bmap_icons(const sg_bmap_header_t ** icons, u16 count);
	static bool load_vector_icons(const sg_vector_icon_t * icons, u16 count);
	static bool load_fonts(const sg_font_ref_t * fonts, int count);

	static const sg_font_ref_t * m_system_fonts;
	static sgfx::Font ** m_font_array;
	static u16 m_font_count;

	static const sg_vector_icon_t * m_vector_icons;
	static u16 m_vector_icon_count;

	static const sg_bmap_header_t ** m_bmap_icons;
	static u16 m_bmap_icon_count;
};

};

#endif /* SYS_ASSETS_HPP_ */
