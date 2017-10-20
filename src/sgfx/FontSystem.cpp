//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include <limits.h>

#include "sgfx/FontSystem.hpp"
using namespace sgfx;

const sg_font_ref_t * FontSystem::m_system_fonts = 0;
Font ** FontSystem::m_font_array = 0;
int FontSystem::m_font_count;


bool FontSystem::load_fonts(const sg_font_ref_t * fonts, int count){
	int i;

	//count the fonts in the table
	m_system_fonts = fonts;

	if( fonts ){

		m_font_count = count;
		m_font_array = (Font**)malloc((count)*sizeof(Font*));
		if( m_font_array == 0 ){
			m_system_fonts = 0;
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
			}
		}
	} else {
		m_font_count = 0;
		if( m_font_array != 0 ){
			free(m_font_array);
			m_font_array = 0;
		}
	}

	return true;
}


Font * FontSystem::get_font(sg_size_t h, bool bold){
	int best_match = 0;
	int h_diff = INT_MAX;
	int h_tmp;
	int i;

	if( m_system_fonts ){
		best_match = 0;
		for(i=0; i < m_font_count; i++){
			h_tmp = h - m_system_fonts[i].height;
			if( (h_tmp > 0) && (h_tmp < h_diff) && (bold == ((m_system_fonts[i].flags & SG_FONT_FLAG_BOLD) == SG_FONT_FLAG_BOLD)) ){
				best_match = i;
			}
		}
		return m_font_array[best_match];
	}

	return 0;
}
