/*
 * GfxSystem.cpp
 *
 *  Created on: May 29, 2016
 *      Author: tgil
 */

#include "sgfx/VectorIconSystem.hpp"
using namespace sgfx;

const sg_vector_icon_t * VectorIconSystem::m_icons = 0;
u16 VectorIconSystem::m_count = 0;


bool VectorIconSystem::load_icons(const sg_vector_icon_t * icons, u32 count){
	m_icons = icons;
	m_count = count;
	return true;
}


const sg_vector_icon_t * VectorIconSystem::get_icon(u32 icon){
	if( icon < m_count ){
		return m_icons + icon;
	}
	return 0;
}
