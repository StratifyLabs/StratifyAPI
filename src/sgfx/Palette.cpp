#include "sgfx/Palette.hpp"
#include "var/String.hpp"

using namespace sgfx;

enum PaletteFlags::pixel_format
		PaletteFlags::decode_pixel_format(
		const var::String & format
		){
	if( format == "rgb332" ){ return pixel_format_rgb332; }
	if( format == "rgb444" ){ return pixel_format_rgb444; }
	if( format == "rgb565" ){ return pixel_format_rgb565; }
	if( format == "rgb666" ){ return pixel_format_rgb666; }
	if( format == "rgb888" ){ return pixel_format_rgb888; }
	if( format == "rgba8888" ){ return pixel_format_rgba8888; }
	return pixel_format_invalid;
}

void PaletteColor::import_hex_code(
		const var::String & hex
		){

	if( hex.length() != 7 ){
		return;
	}
	m_color.red = hex.create_sub_string(
				var::String::Position(1),
				var::String::Length(2)
				).to_unsigned_long(var::String::base_16);
	m_color.green = hex.create_sub_string(
				var::String::Position(3),
				var::String::Length(2)
				).to_unsigned_long(var::String::base_16);
	m_color.blue = hex.create_sub_string(
				var::String::Position(5),
				var::String::Length(2)
				).to_unsigned_long(var::String::base_16);
}

u8 PaletteColor::multiply_component(u8 value, float a){
	u32 tmp = value*(1.0f + a);
	if( tmp > 255 ){ tmp = 255; }
	if( tmp < 0 ){ tmp = 0; }
	return tmp;
}

u8 PaletteColor::add_component(u8 a, u8 b){
	u32 tmp = a + b;
	if( tmp > 255 ){ tmp = 255; }
	return tmp;
}

u8 PaletteColor::subtract_component(u8 a, u8 b){
	s32 tmp = a - b;
	if( tmp < 0 ){ tmp = 0; }
	return tmp;
}

u8 PaletteColor::blend_component(u8 a, u8 b, float ratio){
	return add_component(a*ratio, b * (1.0f - ratio));
}

u8 PaletteColor::adjust_component(u8 value, float a){
	return multiply_component(
				value,
				(a > 0.0f ? 1.0f : -1.0f) +
				a);
}

PaletteColor PaletteColor::blend(
		const PaletteColor & a,
		float ratio
		) const{
	palette_color_t color;
	color.red = blend_component(a.red(), red(), ratio);
	color.green = blend_component(a.green(), green(), ratio);
	color.blue = blend_component(a.blue(), blue(), ratio);
	return PaletteColor(color);
}

PaletteColor PaletteColor::adjust(float a) const {
	PaletteColor result(*this);
	result.m_color.red = adjust_component(m_color.red, a);
	result.m_color.green = adjust_component(m_color.green, a);
	result.m_color.blue = adjust_component(m_color.blue, a);
	return result;
}

PaletteColor PaletteColor::lighter(float a) const {
	return adjust(a);
}

PaletteColor PaletteColor::darker(float a) const {
	return adjust(-1.0*a);
}

sg_color_t PaletteColor::to_pixel_format(enum pixel_format pixel_format) const {
	switch(pixel_format){
		case pixel_format_invalid: return 0;
		case pixel_format_rgb332: return to_rgb332();
		case pixel_format_rgb444: return to_rgb444();
		case pixel_format_rgb565: return to_rgb565();
		case pixel_format_rgb666: return to_rgb666();
		case pixel_format_rgb888: return to_rgb888();
		case pixel_format_rgba8888: return to_rgba8888();
	}
}

sg_color_t PaletteColor::to_rgb332() const{
	u8 rgb = (m_color.red & 0xe0); //top 3
	rgb |= (m_color.green & 0xe0) >> 3; //top 3 green, shifted 3
	rgb |= (m_color.blue & 0xc0) >> 6; //top 2 blue, shifted 6
	return rgb;
}

sg_color_t PaletteColor::to_rgb565() const{
	u16 rgb = (m_color.red & 0xf8) << 8;
	rgb |= (m_color.green & 0xfc) << 3;
	rgb |= (m_color.blue & 0xf8) >> 3;
	return rgb;
}

sg_color_t PaletteColor::to_rgb444() const{
	u16 rgb = (m_color.red & 0xf0) << 4;
	rgb |= (m_color.green & 0xf0);
	rgb |= (m_color.blue & 0xf0) >> 4;
	return rgb;
}

sg_color_t PaletteColor::to_rgb666() const{
	u32 rgb = (m_color.red & 0xfc) << 10;
	rgb |= (m_color.green & 0xfc) << 4;
	rgb |= (m_color.blue & 0xfc) >> 2;
	return rgb;
}

sg_color_t PaletteColor::to_rgb888() const{
	u32 result;
	result =
			(m_color.red << 16) |
			(m_color.green << 8) |
			m_color.blue;
	return result;
}

sg_color_t PaletteColor::to_rgba8888() const{
	u32 result;
	result =
			(m_color.alpha << 24) |
			(m_color.red << 16) |
			(m_color.green << 8) |
			m_color.blue;
	return result;
}

var::Vector<PaletteColor> PaletteColor::calculate_gradient(
			const PaletteColor & start,
			const PaletteColor & end,
			size_t count
		){
	var::Vector<PaletteColor> result(count);
	for(size_t i=0; i < result.count(); i++){
		float ratio = (i * 1.0f) / (result.count() - 1);
		result.at(i) = start.blend(end, ratio);
	}
	return result;
}

Palette::Palette(){
	m_pixel_format = pixel_format_invalid;
}

enum Palette::color_count Palette::get_color_count(u8 bits_per_pixel){
	if( bits_per_pixel == 1 ){ return color_count_1bpp; }
	if( bits_per_pixel == 2 ){ return color_count_2bpp; }
	if( bits_per_pixel == 4 ){ return color_count_4bpp; }
	if( bits_per_pixel == 8 ){ return color_count_8bpp; }
	return color_count_invalid;
}

Palette& Palette::set_color_count(enum color_count color_count){
	m_colors.resize(color_count);
	return *this;
}

Palette& Palette::assign_color(u32 v, const PaletteColor & color){
	if( colors().count() == 0 ){
		printf("Zero colors\n");
		return *this;
	}
	u32 offset = v % colors().count();
	sg_color_t c = color.to_pixel_format(pixel_format());
	m_colors.at(offset) =  c;
	return *this;
}

Palette & Palette::fill_gradient(
		const PaletteColor & color,
		IsAscending is_ascending
		){

	if( is_ascending.argument() ){
		assign_color(0, PaletteColor(0));
	} else {
		assign_color(0, color);
	}

	for(u32 i=1; i < color_count() - 1; i++){
		float scale;
		if( is_ascending.argument() ){
			scale = color_count() - 1 / i;
		} else {
			scale = i * 1.0f / (color_count() - 1);
		}
		assign_color(i,
								 color * scale
								 );
	}

	if( is_ascending.argument() ){
		assign_color(color_count() - 1, color);
	} else {
		assign_color(color_count() - 1, PaletteColor(0));
	}

	return *this;
}

