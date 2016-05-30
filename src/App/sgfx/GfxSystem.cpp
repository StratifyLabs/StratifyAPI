/*
 * GfxSystem.cpp
 *
 *  Created on: May 29, 2016
 *      Author: tgil
 */

#include "sgfx/GfxSystem.hpp"
using namespace sgfx;


bool GfxSystem::load_icons(const sg_icon_t * icons, int count){
	m_icons = icons;
	m_count = count;
	return true;
}


const sg_icon_t * GfxSystem::get_icon(int icon){
	if( icon < m_count ){
		return m_icons + icon;
	}
	return 0;
}
