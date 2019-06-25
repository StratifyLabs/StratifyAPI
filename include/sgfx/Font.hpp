
/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SGFX_FONT_HPP_
#define SGFX_FONT_HPP_

#include <sapi/sg_font_types.h>
#include "../api/WorkObject.hpp"
#include "Bitmap.hpp"
#include "../api/SgfxObject.hpp"
#include "../var/String.hpp"

namespace sgfx {

class Font;

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
class FontInfo : public api::SgfxInfoObject {
public:

	FontInfo(u8 point_size = 0, u8 style = 0,	Font * font = 0);

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
	FontInfo(const var::ConstString & path);

	/*! \details Returns true if the object is valid. */
	bool is_valid() const { return m_point_size; }

	/*! \details Lists the font styles that are available.
	 *
	 */
	enum style {
		ANY,
		THIN /*! Thin font (t) */,
		THIN_ITALIC /*! Thin italic (ti) */,
		EXTRA_LIGHT /*! Extra light (el) */,
		EXTRA_LIGHT_ITALIC /*! Extra light italic (eli) */,
		LIGHT /*! Light (l) */,
		LIGHT_ITALIC /*! Light italic (li) */,
		REGULAR /*! Regular (r) */,
		REGULAR_ITALIC /*! Regular Italic (ri) */,
		MEDIUM /*! Medium (m) */,
		MEDIUM_ITALIC /*! Medium italic (m) */,
		SEMI_BOLD /*! Semi bold (sb) */,
		SEMI_BOLD_ITALIC /*! Semi bold italic (sbi) */,
		BOLD /*! Bold (b) */,
		BOLD_ITALIC /*! Bold italic (bi) */,
		EXTRA_BOLD /*! Extra bold (eb) */,
		EXTRA_BOLD_ITALIC /*! Extra bold italic (ebi) */,
		ICONS /*! Font is a collection of bitmap icons (ico) */
	};

	/*! \details Returns the name of the font. */
	const var::ConstString & name() const { return m_name; }

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
	void set_font(Font * font){ m_font = font; }

	/*! \details Enables sorting FontInfo objects by point size. */
	static int ascending_point_size(const void * a, const void * b);
	/*! \details Enables sorting FontInfo objects by style. */
	static int ascending_style(const void * a, const void * b);

private:
	var::String m_name;
	var::String m_path;
	u8 m_style;
	u8 m_point_size;
	Font * m_font;

};

/*! \brief Font class
 *
 */
class Font : public api::SgfxWorkObject {
public:

	Font();

	/*! \details Returns a string of the available character set */
	static const var::ConstString & ascii_character_set();


	/*! \details Returns the maximum height of any character in the font. */
	virtual sg_size_t get_height() const = 0;

	/*! \details Returns the maximum width of any character in the font. */
	virtual sg_size_t get_width() const = 0;

	//Attribute access methods
	int offset_x() const { return m_char.offset_x; }
	int offset_y() const { return m_char.offset_y; }

	/*! \details Calculates the length (pixels on x-axis) of the specified string. */
	int calculate_length(const var::ConstString & str) const;
	/*! \cond */
	int calc_len(const var::ConstString & str) const { return calculate_length(str); }
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
	int draw(const var::ConstString & const_string, Bitmap & dest, const Point & point) const;

	/*! \details Draws a character on the bitmap.
	 *
	 * @param c The character to draw
	 * @param dest The destination bitmap
	 * @param point The top-left corner where to draw on \a dest
	 * @return Zero on success (-1 if character fails to load)
	 */
	int draw(char c, Bitmap & dest, const Point & point) const;

	const sg_font_char_t & character() const { return m_char; }

	/*! \details Accesses the number of characters in the font. */
	u16 character_count() const { return m_header.character_count; }
	/*! \details Accesses the number of bits per pixel in the font. */
	u16 bits_per_pixel() const { return m_header.bits_per_pixel; }
	/*! \details Accesses the number of kerning pairs in the font. */
	u16 kerning_pair_count() const { return m_header.kerning_pair_count; }

	/*! \details Enables (or disables) kerning. */
	void set_kerning_enabled(bool value = true){ m_is_kerning_enabled = value; }

	/*! \details Returns true if kerning is enabled. */
	bool is_kerning_enabled() const { return m_is_kerning_enabled; }

protected:

	/*! \cond */
	mutable int m_offset;
	mutable sg_font_char_t m_char;
	bool m_is_kerning_enabled;
	sg_size_t m_letter_spacing;
	int m_space_size;
	sg_font_header_t m_header;

	static int to_charset(char ascii);
	static const var::ConstString m_ascii_character_set;

	virtual void draw_char_on_bitmap(const sg_font_char_t & ch, Bitmap & dest, const Point & point) const = 0;
	virtual int load_char(sg_font_char_t & ch, char c, bool ascii) const = 0;
	virtual int load_kerning(u16 first, u16 second) const { return 0; }
	/*! \endcond */

private:

};

}

#endif /* SGFX_FONT_HPP_ */
