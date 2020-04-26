/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "sgfx/Theme.hpp"
#include "var/Json.hpp"
#include "fs/File.hpp"
#include "sys/Printer.hpp"

#define VERSION 0x0308

using namespace sgfx;
using namespace hal;

Theme::Theme(fs::File & color_file) : m_color_file(color_file){
}

var::String Theme::get_style_name(enum style value){
	switch(value){
		case style_dark: return "dark";
		case style_light: return "light";
		case style_brand_primary: return "brandPrimary";
		case style_brand_secondary: return "brandSecondary";
		case style_info: return "info";
		case style_success: return "success";
		case style_warning: return "warning";
		case style_danger: return "danger";
		case style_outline_dark: return "outlineDark";
		case style_outline_light: return "outlineLight";
		case style_outline_brand_primary: return "outlineBrandPrimary";
		case style_outline_brand_secondary: return "outlineBrandSecondary";
		case style_outline_info: return "outlineInfo";
		case style_outline_success: return "outlineSuccess";
		case style_outline_warning: return "outlineWarning";
		case style_outline_danger: return "outlineDanger";
	}
	return "";
}

sg_color_t Theme::text_color() const {
	if( bits_per_pixel() == 1 ){
		return 1;
	}
	return (1<<bits_per_pixel()) - 2;

}

sg_color_t Theme::border_color() const {
	if( bits_per_pixel() == 1 ){
		return 0;
	}
	return (1<<bits_per_pixel()) - 1;
}

var::String Theme::get_state_name(enum state value){
	switch(value){
		case state_default: return "default";
		case state_disabled: return "disabled";
		case state_highlighted: return "highlighted";
	}
	return "";
}

size_t Theme::calculate_color_offset(enum style style, enum state state) const {
	return (style * m_color_count * (last_state + 1) + (state * m_color_count))*sizeof(sg_color_t) + sizeof(header_t);
}


Theme& Theme::set_bits_per_pixel(u8 bits_per_pixel){
	m_header.bits_per_pixel = bits_per_pixel;
	m_color_count = header_color_count();
	return *this;
}

int Theme::write_palette(enum style style,
		enum state state,
		const Palette& palette
		){
	if( palette.colors().count() != m_color_count ){
		return -1;
	}

	return m_color_file.write(
				fs::File::Location(calculate_color_offset(style,state)),
				palette.colors()
				);
}

Palette Theme::read_palette(
		enum style style,
		enum state state
		) const {

	Palette result;

	result
			.set_pixel_format(
				static_cast<enum Palette::pixel_format>(m_header.pixel_format)
				)
			.set_color_count(
				static_cast<enum Palette::color_count>(m_color_count)
				);

	int offset = calculate_color_offset(style,state);
	if( m_color_file.read(
				fs::File::Location(offset),
				result.colors()
				) < 0 ){
		return Palette();
	}

	return result;
}


int Theme::load(const var::String & path){

	if( m_color_file.open(path, fs::OpenFlags::read_only()) < 0 ){
		return -1;
	}

	if( m_color_file.read(var::Reference(m_header)) < 0 ){
		return -1;
	}

	m_color_count = header_color_count();

	return 0;
}

int Theme::create(
		const var::String & path,
		fs::File::IsOverwrite is_overwrite,
		BitsPerPixel bits_per_pixel,
		enum pixel_format pixel_format
		){

	if( m_color_file.create(path, is_overwrite) < 0 ){
		return -1;
	}

	m_header.version = VERSION;
	m_header.bits_per_pixel = bits_per_pixel.argument();
	m_header.pixel_format = pixel_format;
	m_color_count = 0;

	if( m_color_file.write(var::Reference(m_header)) < 0 ){
		return -1;
	}

	m_color_count = header_color_count();


	return 0;
}

int Theme::set_display_palette(
		const Display & display,
		enum style style,
		enum state state
		) const {

	return display.set_palette(
				read_palette(style, state)
				);
}


