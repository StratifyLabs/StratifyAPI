/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

/*! \file */ //Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#ifndef SAPI_SGFX_FONT_HPP_
#define SAPI_SGFX_FONT_HPP_

#include <sapi/sg_font_types.h>
#include "../api/WorkObject.hpp"
#include "Bitmap.hpp"
#include "../api/SgfxObject.hpp"
#include "../var/String.hpp"
#include "../fs/File.hpp"

namespace sgfx {

class Font;

class FontFlags {
public:
	using PointSize = arg::Argument< u8, struct FontPointSizeTag >;
	using Style = arg::Argument< u8, struct FontStyleTag >;
	using Name = arg::Argument< const var::String &, struct FontNameTag >;
	using FileLocation = fs::File::Location;
	using IsExactMatch = arg::Argument<bool, struct FontIsExactMatch>;

	/*! \details Lists the font styles that are available.
	 *
	 */
	enum styles {
		style_any,
		style_thin /*! Thin font (t) */,
		style_thin_italic /*! Thin italic (ti) */,
		style_extra_light /*! Extra light (el) */,
		style_extra_light_italic /*! Extra light italic (eli) */,
		style_light /*! Light (l) */,
		style_light_italic /*! Light italic (li) */,
		style_regular /*! Regular (r) */,
		style_regular_italic /*! Regular Italic (ri) */,
		style_medium /*! Medium (m) */,
		style_medium_italic /*! Medium italic (m) */,
		style_semi_bold /*! Semi bold (sb) */,
		style_semi_bold_italic /*! Semi bold italic (sbi) */,
		style_bold /*! Bold (b) */,
		style_bold_italic /*! Bold italic (bi) */,
		style_extra_bold /*! Extra bold (eb) */,
		style_extra_bold_italic /*! Extra bold italic (ebi) */,
		style_icons /*! Font is a collection of bitmap icons (ico) */,
	};

};

/*!
 * \brief The FontInfo class
 * \details This class contains
 * information that describes a font.
 *
 * The information can be used to allow various classes
 * to auto select a font installed in the system
 * to best match what the user specifies.
 *
 * See sys::Assets for font installation locations.
 *
 * Font names MUST follow the naming conventions for
 * this class to parse the font correctly:
 *
 * ```
 * <name>-<style>-<point size>.sbf
 * ```
 *
 * - name: the name of the font
 * - style: the font style (see enum FontInfo::style for details).
 * - point size: the pixel height of the tallest character in the font
 *
 *
 */
class FontInfo : public FontFlags {
public:

	FontInfo(
			PointSize point_size = PointSize(0),
			Style style = Style(0),
			Font * font = nullptr
			);
	~FontInfo();

	/*! \details Contsructs an object by parsing the path.
	 *
	 * @param path Path to the font. The font name needs to follow the rules below:
	 *
	 * The font name should be name-style-size.ext
	 *
	 * - name is the name of the font
	 * - style can be t, ti, el, eli, l, li, r, ri, m, mi, sb, sbi, b, bi, eb, ebi, ico (see enum style for details)
	 *
	 * Examples include:
	 *
	 * - roboto-l-28.sbf which is Roboto Light size 28 in Stratify bitmap font format.
	 * - opensans-b-32.sbf which is Opensans bold 32
	 *
	 */
	FontInfo(const var::String & path);

	/*! \details Returns true if the object is valid. */
	bool is_valid() const { return m_point_size && (m_font != nullptr); }


	/*! \details Returns the name of the font. */
	const var::String & name() const { return m_name; }

	/*! \details Returns the file path of the font. */
	const var::String & path() const { return m_path; }

	/*! \details Returns the font style (see enum FontInfo::style). */
	u8 style() const { return m_style; }
	/*! \details Returns the point size of the font. */
	u8 point_size() const { return m_point_size; }

	/*! \details Returns a pointer to the Font that can
	 * be used to draw characters and strings on a bitmap.
	 *
	 */
	Font * font() const { return m_font; }

	/*! \details Assigns a pointer to the font that is used for drawing.
	 */
	FontInfo & set_font(Font * font){ m_font = font; return *this; }

	FontInfo & create_font();
	void destroy_font();

	const fs::File & font_file() const {
		return m_file;
	}

	fs::File & font_file(){
		return m_file;
	}

	/*! \details Enables sorting FontInfo objects by point size. */
	static bool ascending_point_size(const FontInfo & a, const FontInfo & b);
	/*! \details Enables sorting FontInfo objects by style. */
	static bool ascending_style(const FontInfo & a, const FontInfo & b);

private:
	var::String m_name;
	var::String m_path;
	u8 m_style;
	u8 m_point_size;
	Font * m_font;
	fs::File m_file;
};

/*! \brief Font class
 *
 */
class Font : public api::SgfxWorkObject, public FontFlags {
public:

	Font(const fs::File & file);

	/*! \details Returns a string of the available character set */
	static const var::String & ascii_character_set();


	/*! \details Returns the maximum height of any character in the font. */
	sg_size_t get_height() const;

	/*! \details Returns the maximum width of any character in the font. */
	sg_size_t get_width() const;

	//Attribute access methods
	int offset_x() const { return m_char.offset_x; }
	int offset_y() const { return m_char.offset_y; }

	/*! \details Calculates the length (pixels on x-axis) of the specified string. */
	int calculate_length(const var::String & str) const;
	/*! \cond */
	int calc_len(const var::String & str) const { return calculate_length(str); }
	/*! \endcond */


	/*! \details Returns the number of characters in the font. */
	int size() const { return m_header.character_count; }

	/*! \details Sets the spacing between letters within a word. */
	void set_letter_spacing(sg_size_t spacing){ m_letter_spacing = spacing; }

	/*! \details Returns the spacing of the letters within a word. */
	sg_size_t letter_spacing() const { return m_letter_spacing; }

	/*! \details Sets the number of pixels in a space between words. */
	void set_space_size(int s){ m_space_size = s; }

	/*! \details Returns the number of pixels between words. */
	int space_size() const { return m_space_size; }

	/*! \details Draws the string on the specified bitmap.
	 *
	 * @param str The string to draw (or set)
	 * @param dest The bitmap to draw the string on
	 * @param point The top left corner to start drawing the string
	 * @return Zero on success
	 */
	int draw(
			const var::String & const_string,
			Bitmap & dest,
			const Point & point
			) const;

	/*! \details Draws a character on the bitmap.
	 *
	 * @param c The character to draw
	 * @param dest The destination bitmap
	 * @param point The top-left corner where to draw on \a dest
	 * @return Zero on success (-1 if character fails to load)
	 */
	int draw(
			char c,
			Bitmap & dest,
			const Point & point
			) const;

	const sg_font_char_t & character() const { return m_char; }

	/*! \details Accesses the number of characters in the font. */
	u16 character_count() const { return m_header.character_count; }
	/*! \details Accesses the number of bits per pixel in the font. */
	u16 bits_per_pixel() const { return m_header.bits_per_pixel; }
	/*! \details Accesses the number of kerning pairs in the font. */
	u16 kerning_pair_count() const { return m_header.kerning_pair_count; }

	/*! \details Enables (or disables) kerning. */
	Font & set_kerning_enabled(bool value = true){ m_is_kerning_enabled = value; return *this; }

	/*! \details Returns true if kerning is enabled. */
	bool is_kerning_enabled() const { return m_is_kerning_enabled; }

	sg_font_kerning_pair_t kerning_pair(u32 offset){ return load_kerning(offset); }
	sg_font_char_t character(u32 offset);
	Bitmap character_bitmap(u32 offset);

protected:

	/*! \cond */
	mutable sg_font_char_t m_char;
	bool m_is_kerning_enabled;
	sg_size_t m_letter_spacing;
	int m_space_size;
	sg_font_header_t m_header;
	mutable Bitmap m_canvas;
	mutable u8 m_current_canvas;
	u32 m_canvas_start;
	u32 m_canvas_size;
	var::Vector<sg_font_kerning_pair_t> m_kerning_pairs;
	const fs::File & m_file;

	void refresh();
	static int to_charset(char ascii);
	static const var::String m_ascii_character_set;

	void draw_char_on_bitmap(const sg_font_char_t & ch, Bitmap & dest, const Point & point) const;
	int load_char(sg_font_char_t & ch, char c, bool ascii) const;
	sg_font_kerning_pair_t load_kerning(u32 offset) const;
	int load_kerning(u16 first, u16 second) const;
	/*! \endcond */

private:

};

}

#endif /* SAPI_SGFX_FONT_HPP_ */
