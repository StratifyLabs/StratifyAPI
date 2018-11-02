//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "sys.hpp"
#include "hal/Display.hpp"

namespace hal {

DisplayPalette::DisplayPalette(){
	memset(&m_palette, 0, sizeof(m_palette));
}

DisplayPalette::DisplayPalette(const display_palette_t & palette, bool readonly){
	set_colors(palette.colors, palette.count, palette.pixel_format, readonly);
}

void DisplayPalette::set_color(display_palette_pixel_format_rgb444_t & pixel, u8 r, u8 g, u8 b, u8 offset){
	//this stores 2 colors in 3 bytes
	if( (offset & 0x01) == 0){
		//first slot
		pixel.r4g4 = (r & 0xf0) | (g >> 4);
		pixel.b4r4 &= 0x0f;
		pixel.b4r4 |= (b & 0xf0);
	} else {
		//second slot
		pixel.b4r4 &= 0xf0;
		pixel.b4r4 |= (r >> 4);
		pixel.g4b4 = (g & 0xf0) | (b >> 4);
	}
}

void DisplayPalette::set_color(display_palette_pixel_format_rgb565_t & pixel, u8 r, u8 g, u8 b){
	/*          ---HB--- ---LB---
	 * RGB565 - RRRRRGGG GGGBBBBB
	 *
	 * But with little endian, it needs to look like this:
	 *          ---HB--- ---LB---
	 * RGB565 - GGGBBBBB RRRRRGGG -- low byte goes out on SPI first (little-endian) and is the high byte of RGB565
	 */

	pixel.r5g3 =
			(r & 0xf1) | //top 5 bits of red is top 5 bits of low byte
			(g >> 5); //green (upper 3 bits) is bottom 3 bits of low byte
	pixel.g3b5 =
			(g << 5) | //blue is bottom 5 bits of high byte
			(b >> 3); //green (lower 3 bits) is top 3 bits of high byte
}

void DisplayPalette::set_color(display_palette_pixel_format_rgb666_t & pixel, u8 r, u8 g, u8 b){
	pixel.r6x2 = r & 0xfc;
	pixel.g6x2 = g & 0xfc;
	pixel.b6x2 = b & 0xfc;
}

void DisplayPalette::set_color(display_palette_pixel_format_rgb888_t & pixel, u8 r, u8 g, u8 b){
	pixel.r8 = r;
	pixel.g8 = g;
	pixel.b8 = b;
}

void DisplayPalette::set_color(u32 v, u8 r, u8 g, u8 b){
	if( v < count() ){
		switch(pixel_format()){
			default:
			case PIXEL_FORMAT_1BPP:
				//displays with 1bpp don't really use a palette?
				break;

			case PIXEL_FORMAT_RGB444:
				set_color( colors().at<display_palette_pixel_format_rgb444_t>(v/2), r, g, b, (v & 0x01));
				break;

			case PIXEL_FORMAT_RGB565:
				set_color( colors().at<display_palette_pixel_format_rgb565_t>(v), r, g, b );
				break;

			case PIXEL_FORMAT_RGB666:
				set_color( colors().at<display_palette_pixel_format_rgb666_t>(v), r, g, b );
				break;

			case PIXEL_FORMAT_RGB888:
				set_color( colors().at<display_palette_pixel_format_rgb888_t>(v), r, g, b );
				break;
		}
	}
}

u16 DisplayPalette::bits_per_pixel() const {
	switch(m_palette.pixel_format){
		case PIXEL_FORMAT_1BPP: return 2;
		case PIXEL_FORMAT_RGB444: return 12;
		case PIXEL_FORMAT_RGB565: return 16;
		case PIXEL_FORMAT_RGB666:
		case PIXEL_FORMAT_RGB888: return 24;
		default:
			return 0;
	}
}


void DisplayPalette::set_colors(void * v, int count, int pixel_format, bool readonly){
	m_colors.free();
	m_palette.count = count;
	m_palette.colors = v;
	m_palette.pixel_format = pixel_format;
	m_colors.set(v, (m_palette.count*bits_per_pixel()+7)/8, readonly);
}

int DisplayPalette::alloc_colors(int count, int pixel_format){
	m_palette.pixel_format = pixel_format;
	if( m_colors.alloc((count * bits_per_pixel()+7)/8) < 0 ){
		m_palette.count = 0;
		m_palette.colors = 0;
		return -1;
	}

	m_palette.count = count;
	m_palette.colors = m_colors.data();
	return 0;
}

int DisplayPalette::save(const char * path) const{
	File f;
	display_palette_t palette;

	if( f.create(path, true) < 0 ){ return -1; }
	f << var::Data(&palette, sizeof(palette)) << colors();
	f.close();

	if( f.error_number() != 0 ){
		return -1;
	}

	return 0;
}

int DisplayPalette::set_monochrome(){
	if( alloc_colors(2, 1) < 0 ){ return -1; }

	set_color(0, 0, 0, 0);
	set_color(1, 255, 255, 255);
	return 0;
}

int DisplayPalette::load(const char * path){
	File f;

	if( f.open_readonly(path) < 0 ){ return -1; }

	if( f.read(&m_palette, sizeof(m_palette)) != sizeof(m_palette) ){
		f.close();
		return -1;
	}

	if( m_colors.alloc(m_palette.count * bits_per_pixel()/8) < 0 ){
		f.close();
		return -1;
	}

	m_palette.colors = colors().to_void();

	f >> m_colors;
	f.close();

	if( f.error_number() ){
		return -1;
	}
	return 0;


}

} /* namespace hal */
