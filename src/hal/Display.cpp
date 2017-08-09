//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "sys.hpp"
#include "hal/Display.hpp"

namespace hal {

void DisplayPalette::set_color(u32 v, u8 r, u8 g, u8 b){
	u8 * ptr = color(v);
	if( ptr == 0 ){
		return;
	}

	switch(pixel_format()){
	default:
	case PIXEL_FORMAT_1BPP: break;
	case PIXEL_FORMAT_RGB565:
		break;
	case PIXEL_FORMAT_RGB666:
		break;
	case PIXEL_FORMAT_RGB888:
		ptr[0] = r;
		ptr[1] = g;
		ptr[2] = b;
		break;
	}

}

void DisplayPalette::set_colors(void * v, int count, int pixel_size, bool readonly){
	m_colors.free();
	data()->pixel_size = pixel_size;
	data()->count = count;
	data()->colors = v;
	m_colors.set(v, item().count*item().pixel_size, readonly);
}

u8 * DisplayPalette::color(u32 v) const {
	if( v < count() ){
		return ((u8*)(item().colors)) + (v*pixel_size());
	}
	return 0;
}

int DisplayPalette::alloc_colors(int count, int pixel_size){
	data()->pixel_size = pixel_size;
	data()->count = count;

	if( m_colors.alloc(item().count * item().pixel_size) < 0 ){
		data()->pixel_size = 0;
		data()->count = 0;
		data()->colors = 0;
		return -1;
	}

	data()->colors = m_colors.data();

	return 0;
}

int DisplayPalette::save(const char * path) const{
	File f;
	int ret;
	display_palette_t palette;

	if( f.create(path, true) < 0 ){ return -1; }

	palette = item();
	palette.colors = 0;
	if( f.write(&palette, size()) != (int)size() ){
		f.close();
		return -1;
	}
	ret = f.write(item().colors, item().count * item().pixel_size);
	f.close();

	if( ret < 0 ){ return ret; }
	return 0;
}

int DisplayPalette::load(const char * path){
	File f;
	int ret;

	if( f.open_readonly(path) < 0 ){ return -1; }

	if( f.read(data(), size()) != (int)size() ){
		f.close();
		return -1;
	}

	if( m_colors.alloc(item().count * item().pixel_size) < 0 ){
		f.close();
		return -1;
	}

	data()->colors = m_colors.data();

	ret = f.read(item().colors, item().count * item().pixel_size);

	f.close();

	if( ret < 0 ){
		return ret;
	}
	return 0;


}

} /* namespace hal */
