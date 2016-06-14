//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include <cstdio>

#include "ui/ListAttr.hpp"
using namespace ui;

ListAttr::ListAttr() {
	// TODO Auto-generated constructor stub
	m_selected = 0;
	m_visible_items = 3;
}

void ListAttr::set_visible_items(list_attr_size_t v){
	if( v == 0 ){
		v = 1;
	}
	m_visible_items = v;
}


void ListAttr::set_selected(u8 v){
	if( v < size() ){
		m_selected = v;
	}
}

void ListAttr::inc_selected(void){
	if( selected() < size() - 1 ){
		m_selected++;
	}
}

void ListAttr::dec_selected(void){
	if( selected() > 0 ){
		m_selected--;
	}
}

list_attr_size_t ListAttr::calc_visible_offset(void) const{
	list_attr_size_t offset;
	list_attr_size_t half;
	if( size() > visible_items() ){
		half = visible_items()/2;
		if( selected() < half ){
			offset = 0;
		} else {
			offset = selected() - half;
		}

		if( offset > size() - visible_items() ){
			offset = size() - visible_items();
		}

	} else {
		offset = 0;
	}
	return offset;
}

list_attr_size_t ListAttr::calc_display_items() const {
	return visible_items() > size() ? size() : visible_items();
}

list_attr_size_t ListAttr::calc_next_visible(s8 dir) const {
	list_attr_size_t next_visible;
	list_attr_size_t visible_offset;
	list_attr_size_t display_items;

	if( (selected() == 0) && visible_items() > 1 ){
		return size();
	}

	if( (selected() == size()-1) && visible_items() > 1 ){
		return size();
	}

	if( visible_items() >= size() ){
		return size();
	}

	visible_offset = calc_visible_offset();
	display_items = calc_display_items();
	next_visible = size();

	if( dir < 0  ){
		if( visible_offset > 0 ){
			next_visible = visible_offset - 1;
		}
	} else {
		if( visible_offset + display_items < size() ){
			next_visible = visible_offset + display_items;
		}
	}

	return next_visible;
}

