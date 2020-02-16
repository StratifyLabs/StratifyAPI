#ifndef SAPI_SGFX_ICONFONT_HPP
#define SAPI_SGFX_ICONFONT_HPP
#include <sapi/sg_font_types.h>

#include "../fs/File.hpp"
#include "Font.hpp"
#include "Bitmap.hpp"

namespace sgfx {

class IconFont;

class IconFontInfo : public FontFlags {
public:
	IconFontInfo(
			PointSize point_size = PointSize(0),
			IconFont * icon_font = nullptr
			);

	~IconFontInfo();

	IconFontInfo(const var::String & path);

	const var::String & name() const {
		return m_name;
	}

	const var::String & path() const {
		return m_path;
	}

	u8 point_size() const {
		return m_point_size;
	}

	/*! \details Returns a pointer to the Font that can
	 * be used to draw characters and strings on a bitmap.
	 *
	 */
	IconFont * icon_font() const { return m_icon_font; }

	/*! \details Assigns a pointer to the icon font.
	 */
	IconFontInfo & set_icon_font(IconFont * icon_font){
		m_icon_font = icon_font;
		return *this;
	}

	IconFontInfo & create_icon_font();


	const fs::File & icon_font_file() const {
		return m_file;
	}

	fs::File & icon_font_file(){
		return m_file;
	}

	static bool ascending_point_size(
			const IconFontInfo & a,
			const IconFontInfo & b
			);

private:
	var::String m_name;
	var::String m_path;
	u8 m_point_size;
	IconFont * m_icon_font;
	fs::File m_file;
};

/*! \brief IconInfo Class
 * \details The IconInfo class
 * contains details of an icon that
 * is stored within an icon font.
 *
 */
class IconInfo {
public:
	IconInfo(const sg_font_icon_t & icon){
		m_icon = icon;
	}

	Area area() const {
		return Area(m_icon.width, m_icon.height);
	}

	const var::String name() const {
		return var::String(m_icon.name);
	}

	u32 canvas_idx() const {
		return m_icon.canvas_idx;
	}

	Point canvas_point() const {
		return Point(
					m_icon.canvas_x, m_icon.canvas_y
					);
	}

private:
	sg_font_icon_t m_icon;

};

class IconFont {
public:
	IconFont(const fs::File & file);

	int refresh();

	sg_size_t point_size() const {
		return m_header.max_height;
	}

	u8 bits_per_pixel() const {
		return m_header.bits_per_pixel;
	}

	sg_size_t width() const {
		return m_header.max_width;
	}

	sg_size_t height() const {
		return m_header.max_height;
	}

	Area area() const {
		return Area(m_header.max_width, m_header.max_height);
	}

	const IconInfo get_info(size_t offset) const {
		return  IconInfo(m_list.at(offset % m_list.count()));
	}

	size_t find(const var::String name) const;

	size_t count() const {
		return m_list.count();
	}

	int draw(
			const var::String & name,
			Bitmap & dest,
			const Point & point
			) const;

	int draw(
			size_t offset,
			Bitmap & dest,
			const Point & point
			) const;

private:
	mutable s32 m_master_canvas_idx = -1;
	Bitmap m_master_canvas;
	sg_font_icon_header_t m_header;
	var::Vector<sg_font_icon_t> m_list;
	const fs::File & m_file;
};

}

#endif // SAPI_SGFX_ICONFONT_HPP
