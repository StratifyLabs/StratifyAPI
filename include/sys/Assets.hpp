/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#ifndef SYS_ASSETS_HPP_
#define SYS_ASSETS_HPP_

#include "../api/WorkObject.hpp"
#include "../sgfx/FileFont.hpp"
#include "../sgfx/Vector.hpp"
#include "../fmt/Svic.hpp"
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

	/*! \details Returns a read-only reference to the font information list.
	 *
	 * This list contains a list of the fonts that are available in the system assets.
	 *
	 */
	static var::Vector<sgfx::FontInfo> & font_info_list(){
		initialize();
		return m_font_info_list;
	}

	static const sgfx::FontInfo * find_font(u8 point_size, u8 weight = sgfx::FontInfo::REGULAR, const var::ConstString & name = "", bool is_exact_match = false);

	/*! \details Returns a read-only reference to the vector path list.
	 *
	 * This list contains a list of the vector path icons that are available in the system assets.
	 *
	 */
	static const var::Vector<fmt::Svic> & vector_path_list(){
		initialize();
		return m_vector_path_list;
	}

	static sgfx::VectorPath find_vector_path(const var::ConstString & name);

private:

	static bool m_is_initialized;
	static var::Vector<sgfx::FontInfo> m_font_info_list;
	static var::Vector<fmt::Svic> m_vector_path_list;
	static void find_fonts_in_directory(const var::ConstString & path);
	static void find_icons_in_directory(const var::ConstString & path);


};

}

#endif /* SYS_ASSETS_HPP_ */
