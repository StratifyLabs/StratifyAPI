/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#ifndef SYS_ASSETS_HPP_
#define SYS_ASSETS_HPP_

#include "../api/WorkObject.hpp"
#include "../sgfx/FileFont.hpp"
#include "../sgfx/Vector.hpp"
#include "../api/SysObject.hpp"

namespace sys {

/*! \brief Assets Class
 * \details The Assets class is a static class
 * that allows the application to access kernel assets
 * such as fonts, bitmaps, and vector graphics.
 */
class Assets : public api::SysInfoObject {
public:

	/*! \details Initializes system assets.
	 *
	 * @return Zero
	 */
	static int initialize();

	static var::Vector<sgfx::FontInfo> & font_info_list(){ return m_font_info_list; }

	static const sgfx::FontInfo * find_font(u8 point_size, u8 weight = sgfx::FontInfo::REGULAR, const var::ConstString & name = "", bool is_exact_match = false);

private:

	static bool m_is_initialized;
	static var::Vector<sgfx::FontInfo> m_font_info_list;
	static void find_fonts_in_directory(const var::ConstString & path);


};

}

#endif /* SYS_ASSETS_HPP_ */
