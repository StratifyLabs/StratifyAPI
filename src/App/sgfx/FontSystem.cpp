//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include <limits.h>

#include "sgfx/FontSystem.hpp"
using namespace sgfx;

const sg_font_ref_t * FontSystem::m_system_fonts = 0;
FontObject ** FontSystem::m_font_array = 0;


bool FontSystem::load_fonts(const sg_font_ref_t * fonts){
	int i;
	int count;

	//count the fonts in the table
	m_system_fonts = fonts;

	if( fonts ){

		i = 0;
		while( (fonts[i].type != SG_FONT_TYPE_NONE) && (fonts[i].font_ptr != 0) ){
			i++;
		}

		count = i;
		if( count == 0 ){
			return true;
		}

		m_font_array = (FontObject**)malloc((i+1)*sizeof(FontObject*));
		if( m_font_array == 0 ){
			return false;
		}

		//create a table of Font pointers based on fonts
		for(i=0; i < count; i++){
			if( fonts[i].type == SG_FONT_MEMORY ){
				m_font_array[i] = new FontMemory(fonts[i].font_ptr);
			} else if( fonts[i].type == SG_FONT_FILE ){
				m_font_array[i] = new FontFile(fonts[i].font_path, fonts[i].font_file_offset);
			} else {
				m_font_array[i] = 0;  //an invalid entry will terminate the list
				return false;
			}
		}

		m_font_array[count] = 0; //null terminated list
	}

	return true;
}


FontObject * FontSystem::get_font(sg_size_t h, bool bold){
	int best_match = 0;
	int h_diff = INT_MAX;
	int h_tmp;
	int i;

	if( m_system_fonts ){
		i = 0;
		while( m_system_fonts[i].font_ptr != 0 ){
			h_tmp = h - m_system_fonts[i].h;
			if( (h_tmp > 0) && (h_tmp < h_diff) && (bold == ((m_system_fonts[i].flags & SG_FONT_FLAG_BOLD) == SG_FONT_FLAG_BOLD)) ){
				best_match = i;
			}
		}
		return m_font_array[best_match];
	}

	return 0;
}
