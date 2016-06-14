/*
 * GfxSystem.cpp
 *
 *  Created on: May 29, 2016
 *      Author: tgil
 */

#include "sgfx/IconSystem.hpp"
using namespace sgfx;

const sg_icon_t * IconSystem::m_icons = 0;
u16 IconSystem::m_count = 0;


bool IconSystem::load_icons(const sg_icon_t * icons, u32 count){
	m_icons = icons;
	m_count = count;
	return true;
}


const sg_icon_t * IconSystem::get_icon(u32 icon){
	if( icon < m_count ){
		return m_icons + icon;
	}
	return 0;
}
