//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "sgfx.hpp"
#include "draw.hpp"
#include "ui/ListItem.hpp"

using namespace ui;

ListItem::ListItem(const char * label, ElementLinked * parent) : ElementLinked(parent){
	m_label.assign(label);
	set_app_icon(0);
	gfx_attr().set(IconAttr::TOTAL, 5, 0);
	set_animation_type(AnimationAttr::PUSH_LEFT);
}

ListItem::ListItem(const char * label, ElementLinked * child, ElementLinked * parent) : ElementLinked(child, parent){
	m_label.assign(label);
	set_app_icon(0);
	gfx_attr().set(IconAttr::TOTAL, 5, 0);
	set_animation_type(AnimationAttr::PUSH_LEFT);
}

ListItem::ListItem(const char * label, enum IconAttr::sys_gfx gfx, ElementLinked * parent) : ElementLinked(parent){
	m_label.assign(label);
	set_app_icon(0);
	gfx_attr().set(gfx, 5, 0);
	set_animation_type(AnimationAttr::PUSH_LEFT);
}

ListItem::ListItem(const char * label, enum IconAttr::sys_gfx gfx, ElementLinked * child, ElementLinked * parent) : ElementLinked(child, parent){
	m_label.assign(label);
	set_app_icon(0);
	gfx_attr().set(gfx, 5, 0);
	set_animation_type(AnimationAttr::PUSH_LEFT);
}


ListItem::ListItem(const char * label, const sg_bitmap_hdr_t * icon, ElementLinked * parent) : ElementLinked(parent){
	m_label.assign(label);
	set_app_icon(icon);
	gfx_attr().set(IconAttr::TOTAL, 5, 0);
	set_animation_type(AnimationAttr::PUSH_LEFT);
}

void ListItem::load_icon(Icon & icon, sg_size_t w, sg_size_t h, const IconAttr * gfx, sg_bounds_t * attr) const {
	if( m_app_icon != 0 ){
		//icon.set_app(m_app_icon);
	} else if( (gfx != 0) && (gfx->gfx() !=  IconAttr::TOTAL) ){
		//icon.set_gfx(*gfx, w, h, attr);
	} else {
		memset(attr, 0, sizeof(sg_bounds_t));
		//icon.free();
	}
}

void ListItem::draw_to_scale(const DrawingScaledAttr & attr){
	Icon icon;
	sg_bounds_t sg_attr;
	sg_dim_t icon_dim;
	Dim d = attr.d();
	sg_point_t p = attr.p();
	char buffer[32];

	int height;
	sg_point_t icon_point;
	sg_size_t icon_height;

	//draw the label and the icon
	Dim padded;
	icon_height = d.h()/2;

	/*
	load_icon(icon, icon_height, icon_height, &icon_attr_const(), &sg_attr);
	icon_dim = sg_draw_attr_dim(&sg_attr);

	padded.set(d.w() - icon_dim.w, d.h()*80/100 );

	if( m_label.font_size() == 0 ){
		height = App::select_system_font(padded, label(), font_bold());
	} else {
		height = App::select_system_font(m_label.font_size(), font_bold());
	}


	icon_point.x = p.x + d.w() - sg_attr.bottom_right.x;

	if( align_top() ){
		icon_point.y = p.y;
	} else if( align_bottom() ){
		p.y = d.h() - height;
		icon_point.y = d.h() - icon.h();
	} else {
		icon_point.y = p.y + d.h()/2 - icon.h()/2;
		p.y = p.y + d.h()/2 - height/2;
	}

	strcpy(buffer, label());

	if( icon_dim.w > 0 ){
		if( m_label.font_size() != 0 ){
			int len;
			len = App::font()->len(label());
			if( len > (icon_point.x - p.x) ){
				memset(buffer, 0, 32);
				strncpy(buffer, label(), 6);
				strcat(buffer, "...");
			}
		}
	}


	App::font()->set_str(buffer, attr.b(), p);

	//draw the icon -- on the right side
	if( icon_dim.w > 0 ){
		attr.b()->set_bitmap(&icon, icon_point);
	}
	*/
}

Element * ListItem::event_handler(int event, const DrawingAttr & attr){
	switch(event){
	/*
	case LEFT_PRESS:
	case RIGHT_PRESS:
	case ENTER:
		if( gfx_attr().gfx() != IconAttr::TOTAL ){
			App::bar().set_center( gfx_attr().gfx(), 3, gfx_attr().rotation() );
		}
		break;
	case RIGHT_HOLD:
	case CENTER_PRESS:
		if( child() ){
			child()->set_animation_type(AnimationAttr::PUSH_LEFT);
			return child();
		}
		break;
		*/
	}
	return ElementLinked::event_handler(event, attr);
}

/*
ListItemToggle::ListItemToggle(const char * label, ElementLinked * parent) :
													//SettingInt(label, Setting::bool_attrs()),
													ListItem(key().name(), parent){
	set_sys_gfx(GfxAttr::TOTAL);
	set_thickness(0);
}
*/

void ListItemToggle::set_enabled(bool v){
	ListItem::set_enabled(v);
	if( v ){
		set_sys_gfx(IconAttr::OK);
	} else {
		set_sys_gfx(IconAttr::TOTAL);
	}

	//save key to file
	//set_value( enabled() );

	//save();
}


Element * ListItemToggle::event_handler(int event, const DrawingAttr & attr){
	/*
	switch(event){
	case SETUP:
		//load();
		//set_enabled( value() );
		if( enabled() ){
			set_sys_gfx(IconAttr::OK);
		} else {
			set_sys_gfx(IconAttr::TOTAL);
		}
		return ListItem::event_handler(SETUP, attr);

	case RIGHT_HOLD:
	case CENTER_PRESS:
		set_enabled(!enabled());
		return parent();

	case LEFT_PRESS:
	case RIGHT_PRESS:
	case ENTER:
		App::bar().set_center( IconAttr::OK, 0 );
		return Element::event_handler(event, attr);
	}
	*/

	return ListItem::event_handler(event, attr);

}

ListItemCallback::ListItemCallback(list_item_callback_t callback, const char * label, enum IconAttr::sys_gfx gfx, ElementLinked * parent) :
																		ListItem(label, gfx, parent){ m_callback = callback; }


ListItemCallback::ListItemCallback(list_item_callback_t callback, const char * label, const sg_bitmap_hdr_t * icon, ElementLinked * parent) :
																		ListItem(label, icon, parent){ m_callback = callback; }

Element * ListItemCallback::event_handler(int event, const DrawingAttr & attr){
	/*
	switch(event){
	case RIGHT_HOLD:
	case CENTER_PRESS:
		if( m_callback != 0 ){
			return m_callback(parent(), this, 0);
		}
		return parent();
	default:
		return ListItem::event_handler(event, attr);
	}
	*/
}

ListItemElement::ListItemElement(ElementLinked * child, const char * label, ElementLinked * parent)
: ListItem(label, child, parent){ this->child()->set_parent(parent); }
ListItemElement::ListItemElement(ElementLinked * child, const char * label, enum IconAttr::sys_gfx gfx, ElementLinked * parent)
: ListItem(label, gfx, child, parent){
	if( this->child() ){
		this->child()->set_parent(parent);
	}
}
ListItemElement::ListItemElement(ElementLinked * child, const char * label, const sg_bitmap_hdr_t * icon, ElementLinked * parent)
: ListItem(label, icon, parent){ set_child(child); this->child()->set_parent(parent); }



ListItemBack::ListItemBack(ElementLinked * parent) : ListItem("Back", IconAttr::CHEVRON, parent){
	set_rotation(IconAttr::LEFT);
}

Element * ListItemBack::event_handler(int event, const DrawingAttr & attr){
	/*
	switch(event){
	case RIGHT_HOLD:
	case CENTER_PRESS:
		if( parent() && parent()->parent() ){
			parent()->parent()->set_animation_type(AnimationAttr::PUSH_RIGHT);
			return parent()->parent();
		}
	}
	*/
	return ListItem::event_handler(event, attr);
}



ListItemExit::ListItemExit(ElementLinked * parent) : ListItemBack(parent){
	set_label("Exit");
	set_rotation(SG_TRIG_POINTS/4);
}


ListItemCheck::ListItemCheck(const char * label, List * parent) :
						ListItem(label, IconAttr::TOTAL, parent){
	set_enabled(false);
}

void ListItemCheck::set_enabled(bool v){
	ListItem::set_enabled(v);
	if( enabled() ){
		//set_sys_gfx(IconAttr::OK);
	} else {
		set_sys_gfx(IconAttr::TOTAL);
	}
}


Element * ListItemCheck::event_handler(int event, const DrawingAttr & attr){
	/*
	switch(event){
	case RIGHT_HOLD:
	case CENTER_PRESS:
		set_enabled( !enabled() );
		if( parent() ){
			if( parent()->exclusive_checklist() == true ){
				List * list = (List*)parent();
				ListItemCheck * item;
				for(size_t i = 0; i < list->size(); i++){
					item = (ListItemCheck *)list->at(i);
					item->set_enabled(false);
				}
				set_enabled();
			}
		}
		//return the list the item belongs to
		return parent();
	default:
		return ListItem::event_handler(event, attr);
	}
	*/
}

ListDir::ListDir(const char * path, ElementLinked * child, ElementLinked * parent) : List(parent), m_item(child, "TBD", IconAttr::CHEVRON, this) {
	set_path(path);
}

ListDir::~ListDir(){
	m_dir.close();
}

ElementLinked * ListDir::at(list_attr_size_t i){
	m_dir.rewind();
	size_t j;
	j=0;

	if( i == 0 ){
		recount();
	}

	while( (m_dir.read() != 0) && (j < i) ){
		j++;
	}

	if( !suffix_visible() ){
		i32 k;
		for(k=strlen(m_dir.name())-1; k>=0; k--){
			if( m_dir.name()[k] == '.' ){
				m_dir.data()[k] = 0;
				break;
			}
		}
	}

	m_item.set_label(m_dir.name());

	return &m_item;
}


void ListDir::set_path(const char * path){
	m_path = path;
	m_dir.close();
	if( m_dir.open(path) < 0 ){

	}
	recount();

}

void ListDir::recount(void){
	int ret;
	ret = m_dir.count();
	if( ret > 0 ){
		m_total = ret;
	} else {
		m_total = 1;
		item().set_label("Empty");
	}
}

Element * ListDir::event_handler(int event, const DrawingAttr & attr){
	/*
	switch(event){
	case RIGHT_HOLD:
	case CENTER_PRESS:
		if( m_callback ){ m_callback(this); }
		// no break
	default:
		return List::event_handler(event, attr);
	}
	*/
}

