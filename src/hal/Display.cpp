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

void DisplayPalette::set_color(u32 v, u8 r, u8 g, u8 b){
	if( v < count() ){
		switch(pixel_format()){
			default:
			case PIXEL_FORMAT_1BPP:
				//displays with 1bpp don't really use a palette?
				break;

			case PIXEL_FORMAT_RGB565:
				colors().at<display_palette_pixel_format_rgb565_t>(v).rgb565 =
						(r << (5+6)) | ((b & 0x3f) << 5) | (g & 0x1f);
				break;
			case PIXEL_FORMAT_RGB666:
				colors().at<display_palette_pixel_format_rgb666_t>(v).r = r;
				colors().at<display_palette_pixel_format_rgb666_t>(v).g = g;
				colors().at<display_palette_pixel_format_rgb666_t>(v).b = b;
				break;
			case PIXEL_FORMAT_RGB888:
				colors().at<display_palette_pixel_format_rgb888_t>(v).r = r;
				colors().at<display_palette_pixel_format_rgb888_t>(v).g = g;
				colors().at<display_palette_pixel_format_rgb888_t>(v).b = b;
				break;
		}
	}
}

void DisplayPalette::set_colors(void * v, int count, int pixel_size, bool readonly){
	m_colors.free();
	m_palette.pixel_size = pixel_size;
	m_palette.count = count;
	m_palette.colors = v;
	m_colors.set(v, m_palette.count*m_palette.pixel_size, readonly);
}

int DisplayPalette::alloc_colors(int count, int pixel_size){
	if( m_colors.alloc(count * pixel_size) < 0 ){
		m_palette.pixel_size = 0;
		m_palette.count = 0;
		m_palette.colors = 0;
		return -1;
	}

	m_palette.pixel_size = pixel_size;
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

	if( m_colors.alloc(m_palette.count * m_palette.pixel_size) < 0 ){
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
