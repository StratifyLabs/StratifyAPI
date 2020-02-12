/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.


#ifndef SAPI_SYS_ASSETS_HPP_
#define SAPI_SYS_ASSETS_HPP_

#include "../api/WorkObject.hpp"
#include "../sgfx/FileFont.hpp"
#include "../sgfx/IconFont.hpp"
#include "../sgfx/Vector.hpp"
#include "../fmt/Svic.hpp"
#include "../api/SysObject.hpp"

namespace sys {


/*! \brief Assets Class
 * \details The Assets class is a static class
 * that allows the application to access kernel assets
 * such as fonts, bitmaps, and vector graphics.
 *
 * This class will search the following locations
 * for fonts and graphics files:
 *
 * - /assets
 * - /home
 * - /home/assets
 *
 * Several other classes within the StratifyAPI use
 * this class without the user ever needed to reference it.
 *
 * - draw::Text will lookup fonts using this class
 * - draw::Icon will lookup icons files installed as assets
 *
 *
 */
class Assets : public api::InfoObject {
public:

	/*! \details Initializes system assets.
	 *
	 * @return Zero
	 *
	 * This method can be called explicitly, but will
	 * be called whenever as needed if not.
	 *
	 */
	static int initialize();

	/*! \details Returns a read-only reference to the font information list.
	 *
	 * This list contains a list of the fonts that are available in the system assets.
	 *
	 */
	static var::Vector<sgfx::FontInfo> & font_info_list(){
		initialize();
		return m_font_info_list;
	}

	static const sgfx::FontInfo * find_font(
			const sgfx::FontInfo::Name name,
			const sgfx::FontInfo::PointSize point_size,
			const sgfx::FontInfo::Style style = sgfx::FontInfo::Style(sgfx::FontInfo::REGULAR),
			const sgfx::FontInfo::IsExactMatch is_exact_match = sgfx::FontInfo::IsExactMatch(false)
			);

	static var::Vector<sgfx::IconFontInfo> & icon_font_info_list(){
		initialize();
		return m_icon_font_info_list;
	}

	static const sgfx::IconFontInfo * find_icon_font(
			const sgfx::IconFontInfo::Name name,
			const sgfx::IconFontInfo::PointSize point_size,
			const sgfx::IconFontInfo::IsExactMatch is_exact_match = sgfx::IconFontInfo::IsExactMatch(false)
			);


	static void find_fonts_in_directory(const var::String & path);
	static void find_icons_in_directory(const var::String & path);

private:
	static bool m_is_initialized;
	static var::Vector<sgfx::FontInfo> m_font_info_list;
	static var::Vector<sgfx::IconFontInfo> m_icon_font_info_list;

};

}

#endif /* SAPI_SYS_ASSETS_HPP_ */
