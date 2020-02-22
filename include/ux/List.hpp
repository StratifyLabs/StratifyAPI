/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_UX_LIST_HPP
#define SAPI_UX_LIST_HPP

#include "Layout.hpp"
#include "TouchGesture.hpp"

namespace ux {

class ListItem;
class List;

class ListEvent : public EventObject<EVENT_TYPE('_','l','s','t')> {
	public:

	ListEvent(
				const ListItem & item
				) :
		EventObject<EVENT_TYPE('_','l','s','t')>(0),
		m_list_item(item){

	}

	const ListItem & item() const {
		return m_list_item;
	}

private:
	const ListItem & m_list_item;

};


class ListItem :
		public ComponentAccess<ListItem, COMPONENT_SIGNATURE('l','s','t','i')>,
		public DrawingComponentProperties<ListItem> {
public:
	enum type {
		type_icon, //just an icon
		type_value, //text value
		type_toggle, //toggle switch
		type_button //button
	};

	ListItem& set_key(const var::String & value){
		m_key = value;
		return *this;
	}

	ListItem& set_value(const var::String & value){
		m_value = value;
		return *this;
	}

	ListItem& set_icon(const var::String & value){
		m_icon = value;
		return *this;
	}

	ListItem& set_type(enum type value){
		m_type = value;
		return *this;
	}

	const var::String & key() const { return m_key; }
	const var::String & value() const { return m_value; }

	void draw_to_scale(const DrawingScaledAttributes & attributes);
	void handle_event(const ux::Event & event);

private:
	enum type m_type;
	var::String m_key;
	var::String m_value;
	var::String m_icon;

};

class List : public LayoutAccess<List> {
public:

	List(const var::String & name, EventLoop * event_loop) : LayoutAccess<List>(name, event_loop){
		set_flow(flow_vertical);
	}

	List& add_component(
			Component& component
			);

	List& set_item_height(drawing_size_t value){
		m_item_height = value;
		return *this;
	}

	u8 border_size() const {
		return m_border_size;
	}

private:
	drawing_size_t m_item_height;
	u8 m_border_size = 1;



};

}

#endif // SAPI_UX_LIST_HPP
