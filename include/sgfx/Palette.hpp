#ifndef SAPI_SGFX_PALETTE_HPP
#define SAPI_SGFX_PALETTE_HPP

#include <sapi/sg_types.h>
#include "../var/String.hpp"
#include "../var/Data.hpp"
#include "../sys/Printer.hpp"

namespace sgfx {

class PaletteFlags {
public:

	using Position = var::Reference::Position;
	using IsAscending = arg::Argument<bool, struct PaletteIsAscendingTag>;

	enum color_count {
		color_count_invalid = 0,
		color_count_1bpp = 2,
		color_count_2bpp = 4,
		color_count_4bpp = 16,
		color_count_8bpp = 256
	};

	enum pixel_format {
		pixel_format_invalid,
		pixel_format_rgb332,
		pixel_format_rgb444,
		pixel_format_rgb565,
		pixel_format_rgb666,
		pixel_format_rgb888,
		pixel_format_rgba8888
	};

	typedef struct MCU_PACK {
		u8 red;
		u8 green;
		u8 blue;
		u8 alpha;
	} palette_color_t;

	static enum pixel_format decode_pixel_format(const var::String & format);
	static u32 bits_per_pixel_format(enum pixel_format format);

};

class PaletteColor : public PaletteFlags {
public:

	using Rgb332 = arg::Argument<u16, struct PaletteColorRgb332Tag>;
	using Rgb444 = arg::Argument<u16, struct PaletteColorRgb444Tag>;
	using Rgb565 = arg::Argument<u16, struct PaletteColorRgb565Tag>;
	using Rgb666 = arg::Argument<u16, struct PaletteColorRgb666Tag>;
	using Rgb888 = arg::Argument<u32, struct PaletteColorRgb888Tag>;
	using Rgba8888 = arg::Argument<u32, struct PaletteColorRgba8888Tag>;

	PaletteColor(const var::String & hex_code){
		import_hex_code(hex_code);
	}


	PaletteColor(Rgb332 rgb332){
		const u16 value = rgb332.argument();
		m_color.alpha = 0xff;
		m_color.red = (value >> 5) & 0x07;
		m_color.green = (value >> 2) & 0x07;
		m_color.blue = value & 0x03;
	}

	PaletteColor(Rgb444 rgb444){
		const u16 value = rgb444.argument();
		m_color.alpha = 0xff;
		m_color.red = ((value >> 8) & 0x0f) << 4;
		m_color.green = ((value >> 4) & 0x0f) << 4;
		m_color.blue = (value & 0x0f) << 4;
	}

	PaletteColor(Rgb565 rgb565){
		const u16 value = rgb565.argument();
		m_color.alpha = 0xff;
		m_color.red = ((value >> 11) & 0x1f) << 3;
		m_color.green = ((value >> 5) & 0x3f) << 2;
		m_color.blue = (value & 0x1f) << 3;
	}

	PaletteColor(Rgb666 rgb666){
		const u16 value = rgb666.argument();
		m_color.alpha = 0xff;
		m_color.red = ((value >> 12) & 0x3f) << 2;
		m_color.green = ((value >> 6) & 0x3f) << 2;
		m_color.blue = (value & 0x3f) << 2;
	}

	PaletteColor(Rgb888 rgb888){
		const u32 value = rgb888.argument();
		m_color.alpha = 0xff;
		m_color.red = value >> 16;
		m_color.green = value >> 8;
		m_color.blue = value;
	}

	PaletteColor(Rgba8888 rgba8888){
		const u32 value = rgba8888.argument();
		m_color.alpha = value >> 24;
		m_color.red = value >> 16;
		m_color.green = value >> 8;
		m_color.blue = value;
	}

	PaletteColor(sg_color_t c){
		m_color.alpha = c>>24;
		m_color.red = c>>16;
		m_color.green = c>>8;
		m_color.blue = c;
	}

	PaletteColor(){
		m_color = {0};
	}

	PaletteColor(const palette_color_t & color){
		m_color = color;
	}

	PaletteColor lighter(float a) const;
	PaletteColor darker(float a) const;
	PaletteColor blend(const PaletteColor & a, float ratio) const;

	u32 to(enum pixel_format pixel_format){
		switch(pixel_format){
			case pixel_format_invalid: return 0;
			case pixel_format_rgb332: return to_rgb332();
			case pixel_format_rgb444: return to_rgb444();
			case pixel_format_rgb565: return to_rgb565();
			case pixel_format_rgb666: return to_rgb666();
			case pixel_format_rgb888: return to_rgb888();
			case pixel_format_rgba8888: return to_rgba8888();
		}
		return 0;
	}

	PaletteColor operator * (float a) const {
		PaletteColor result;
		result.m_color.alpha = m_color.alpha;
		result.m_color.red = multiply_component(m_color.red, a);
		result.m_color.green = multiply_component(m_color.green, a);
		result.m_color.blue = multiply_component(m_color.blue, a);
		return result;
	}

	PaletteColor& operator * (float a){
		*this = *this * a;
		return *this;
	}

	var::String to_hex_code() const {
		var::String result = "#" +
				var::String::number(red(), "%02x") +
				var::String::number(green(), "%02x") +
				var::String::number(blue(), "%02x");
		return result;
	}

	sg_color_t to_pixel_format(enum pixel_format pixel_format) const;
	sg_color_t to_rgb332() const;
	sg_color_t to_rgb444() const;
	sg_color_t to_rgb565() const;
	sg_color_t to_rgb666() const;
	sg_color_t to_rgb888() const;
	sg_color_t to_rgba8888() const;

	u8 red() const { return m_color.red; }
	u8 green() const { return m_color.green; }
	u8 blue() const { return m_color.blue; }
	u8 alpha() const { return m_color.alpha; }
	sg_color_t color() const { return to_rgba8888(); }

	static var::Vector<PaletteColor> calculate_gradient(
			const PaletteColor & start,
			const PaletteColor & end,
			size_t count
			);

private:
	palette_color_t m_color;

	PaletteColor adjust(float a) const;
	static u8 adjust_component(u8 value, float a);
	static u8 multiply_component(u8 value, float a);
	static u8 add_component(u8 a, u8 b);
	static u8 subtract_component(u8 a, u8 b);
	static u8 blend_component(u8 a, u8 b, float ratio);

	void import_hex_code(
			const var::String & hex
			);
};

class Palette : public PaletteFlags {
public:
	Palette();
	Palette& set_pixel_format(enum pixel_format value){
		m_pixel_format = value;
		return *this;
	}

	bool is_valid() const {
		return (colors().count() > 0) &&
				(pixel_format() != pixel_format_invalid
				);
	}

	static enum color_count get_color_count(u8 bits_per_pixel);
	u8 get_bits_per_pixel() const;

	Palette& set_color_count(enum color_count color_count);

	Palette& assign_color(
			u32 v,
			const PaletteColor & color
			);

	/*! \details Returns the pixel format. */
	enum pixel_format pixel_format() const {
		return m_pixel_format;
	}

	Palette & create_gradient(
			const PaletteColor & color,
			IsAscending is_ascending = IsAscending(true)
			);

	var::Vector<sg_color_t> & colors(){ return m_colors; }
	const var::Vector<sg_color_t> & colors() const { return m_colors; }

	PaletteColor palette_color(size_t offset) const;

private:
	enum pixel_format m_pixel_format;
	var::Vector<sg_color_t> m_colors;
};


}

sys::Printer& operator << (sys::Printer& printer, const sgfx::Palette & palette);

#endif // SAPI_SGFX_PALETTE_HPP
