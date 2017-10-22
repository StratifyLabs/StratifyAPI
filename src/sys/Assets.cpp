//Copyright 2011-2017 Tyler Gilbert; All Rights Reserved

#include <limits.h>

#include "sys/assets.h"
#include "sys/Sys.hpp"
#include "sys/Assets.hpp"
#include "sgfx/FontFile.hpp"
#include "sgfx/FontMemory.hpp"

using namespace sys;
using namespace sgfx;


const sg_font_ref_t * Assets::m_system_fonts = 0;
Font ** Assets::m_font_array = 0;
int Assets::m_font_count;

const sg_vector_icon_t * Assets::m_vector_icons = 0;
u16 Assets::m_vector_icon_count = 0;

const sg_bmap_header_t * Assets::m_bmap_icons = 0;
u16 Assets::m_bmap_icon_count = 0;

int Assets::init(){

	asset_request_t request;

	if( Sys::request(ASSETS_KERNEL_REQUEST_FONTS, &request) == 0 ){
		load_fonts((const sg_font_ref_t*)request.list, request.count);
	}


	if( Sys::request(ASSETS_KERNEL_REQUEST_VECTOR_ICONS, &request) == 0 ){
		load_vector_icons((const sg_vector_icon_t*)request.list, request.count);
		printf("Loaded %d vector iconts\n", request.count);
	} else {

	}

	if( Sys::request(ASSETS_KERNEL_REQUEST_VECTOR_ICONS, &request) == 0 ){
		load_bmap_icons((const sg_bmap_header_t*)request.list, request.count);
	}

	return 0;
}


bool Assets::load_vector_icons(const sg_vector_icon_t * icons, u16 count){
	m_vector_icons = icons;
	m_vector_icon_count = count;
	return true;
}

bool Assets::load_bmap_icons(const sg_bmap_header_t * icons, u16 count){
	m_bmap_icons = icons;
	m_bmap_icon_count = count;
	return true;
}


const sg_vector_icon_t * Assets::get_vector_icon(u16 icon){
	if( icon < m_vector_icon_count ){
		return m_vector_icons + icon;
	}
	printf("Icon %d doesn't exist\n", icon);
	return 0;
}

const sg_bmap_header_t * Assets::get_bmap_icon(u16 icon){
	if( icon < m_bmap_icon_count ){
		return m_bmap_icons + icon;
	}
	return 0;
}

bool Assets::load_fonts(const sg_font_ref_t * fonts, int count){
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

			if( m_font_array[i] ){
				m_font_array[i]->set_space_size( m_font_array[i]->get_height() / 6);
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


Font * Assets::get_font(sg_size_t h, bool bold){
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

