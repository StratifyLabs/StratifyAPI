//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "sgfx.hpp"
#include "draw.hpp"
#include "sys/Assets.hpp"
#include "ui/ListItem.hpp"
#include "ev/Button.hpp"
#include "ui/Event.hpp"

using namespace ui;

ListItem::ListItem(
		const var::String & label,
		LinkedElement * parent,
		LinkedElement * child
		) : LinkedElement(parent, child){
	m_text_attr.string() = label;
	set_animation_type(AnimationAttr::PUSH_LEFT);
}


void ListItem::draw_to_scale(const DrawingScaledAttr & attr){
#if defined LEGACY_ICON
	sg_region_t bounds;
	sg_area_t icon_dim;
	Area d = attr.area();
	sg_point_t p = attr.point();
	char buffer[32];

	int height;
	sg_point_t icon_point;
	sg_size_t icon_height;
	Font * font;

	//draw the label and the icon
	Area padded;
	icon_height = d.height()/2;
	Bitmap icon_bitmap(icon_height, icon_height);
	icon_bitmap.set_pen(icon().pen());

	if( &(icon_attr().icon()) != 0 ){
		icon_bitmap.clear();
		VectorMap map(icon_bitmap, icon_attr().rotation());
		Vector::draw(icon_bitmap,
						 icon_attr_const().icon(),
						 map,
						 &bounds);
		icon_dim = bounds.dim;
	} else {
		icon_dim.dim = 0;
	}

	padded = Area(d.width() - icon_dim.width, d.height()*80/100 );

	if( m_text_attr.font_size() == 0 ){
		height = padded.height();
	} else {
		height = m_text_attr.font_size();
	}

	font = sys::Assets::get_font(height, text_attr().font_bold());
	height = font->get_height();

	icon_point.x = p.x + d.width() - (bounds.point.x + bounds.dim.width);


	if( is_align_top() ){
		icon_point.y = p.y;
	} else if( is_align_bottom() ){
		p.y = d.height() - height;
		icon_point.y = d.height() - icon_dim.height;
	} else {
		icon_point.y = p.y + d.height()/2 - icon_dim.height/2;
		p.y = p.y + d.height()/2 - height/2;
	}

	strcpy(buffer, text_attr().text());

	if( icon_dim.width > 0 ){
		if( m_text_attr.font_size() != 0 ){
			int len;
			len = font->calc_len(text_attr().text());
			if( len > (icon_point.x - p.x) ){
				memset(buffer, 0, 32);
				strncpy(buffer, text_attr().text(), 6);
				strcat(buffer, "...");
			}
		}
	}

	font->draw(buffer, attr.bitmap(), p);

	//draw the icon -- on the right side
	if( icon_dim.width > 0 ){
		attr.bitmap().draw_bitmap(icon_point, icon_bitmap);
	}
#endif
}


Element * ListItem::handle_event(const Event  & event, const DrawingAttr & attr){

	if( event.type() == Event::SETUP ){
		if( child() ){
			child()->set_parent(parent());
		}
	} else if( event.type() == Event::LIST_ITEM_ACTUATED ){
		if ( child() ){
			child()->set_animation_type(AnimationAttr::PUSH_LEFT);
			return child();
		}
		return parent();
	} else if( event.type() == Event::LIST_ITEM_SELECTED ){
		return parent()->handle_event(event, attr);
	}

	return LinkedElement::handle_event(event, attr);
}


ListItemToggle::ListItemToggle(const char * label, LinkedElement * parent) :
	ListItem(label, 0, parent){
#if defined LEGACY_ICON
	m_toggle_enabled_icon = 0;
	m_toggle_disabled_icon = 0;
#endif
}

void ListItemToggle::set_enabled(bool v){
	ListItem::set_enabled(v);
#if defined LEGACY_ICON
	if( v ){
		icon_attr().set_icon(m_toggle_enabled_icon);
	} else {
		icon_attr().set_icon(m_toggle_disabled_icon);
	}
#endif
}


Element * ListItemToggle::handle_event(const Event  & event, const DrawingAttr & attr){
	if( event.type() == Event::LIST_ITEM_ACTUATED ){
		set_enabled( !is_enabled() );
	}
	return ListItem::handle_event(event, attr);
}






ListItemCheck::ListItemCheck(const char * label, List * parent) :
	ListItem(label, 0, parent){
	set_enabled(false);
}


DirList::DirList(
		const var::String & path,
		LinkedElement * parent,
		LinkedElement * child) :
	List(parent),
	m_item("TBD", this, child) {
	set_path(path);
}

DirList::~DirList(){
	m_dir.close();
}

LinkedElement & DirList::at(list_attr_size_t i){
	m_dir.rewind();
	u32 j;
	j=0;

	if( i == 0 ){
		recount();
	}

	while( (m_dir.read() != 0) && (j < i) ){
		j++;
	}

	if( !is_suffix_visible() ){
		s32 k;
		for(k=strlen(m_dir.name())-1; k>=0; k--){
			if( m_dir.name()[k] == '.' ){
				m_dir.data()[k] = 0;
				break;
			}
		}
	}

	m_item.text_attr().string().assign(m_dir.name());

	return m_item;
}


void DirList::set_path(const var::String & path){
	m_path = path;
	m_dir.close();
	if( m_dir.open(
			 path
			 ) < 0 ){

	}
	recount();

}

void DirList::recount(void){
	int ret;
	ret = m_dir.count();
	if( ret > 0 ){
		m_size = ret;
	} else {
		m_size = 1;
		m_item.text_attr().string().assign("Empty");
	}
}

Element * DirList::handle_event(const Event  & event, const DrawingAttr & attr){
	switch(event.type()){

		case Event::BUTTON_ACTUATION:
			if( event.button()->event_id() == Event::SELECT_BUTTON ){
				if( m_callback ){ m_callback(this); }
			}
			// no break
		default:
			return List::handle_event(event, attr);
	}

}

