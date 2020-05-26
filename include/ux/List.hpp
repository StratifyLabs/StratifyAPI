/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_UX_LIST_HPP
#define SAPI_UX_LIST_HPP

#include "Layout.hpp"
#include "TouchGesture.hpp"



namespace ux {

class ListItem;

class ListEvent : public EventObject<ListItem, EVENT_TYPE('_','l','s','t')> {
	public:

	ListEvent(
				ListItem & item
				) :
		EventObject(0, &item){}
};

class ListItem : public ComponentAccess<ListItem>{
public:
	ListItem(const var::String & name) : ComponentAccess(name){}
	void draw(const DrawingScaledAttributes & attributes);
	void handle_event(const ux::Event & event);

private:
	API_ACCESS_COMPOUND(ListItem,var::String,key);
	API_ACCESS_COMPOUND(ListItem,var::String,value);
};

template<class T> class ListItemAccess: public ListItem {
public:
	ListItemAccess(const var::String&name) : ListItem(name){}

	API_ACCESS_DERIVED_COMPOUND(ListItem,T,var::String,key)
	API_ACCESS_DERIVED_COMPOUND(ListItem,T,var::String,value)

	COMPONENT_ACCESS_CREATE()

};

class ListFiller : public ComponentAccess<ListFiller>{
public:
	ListFiller(const var::String & name) : ComponentAccess(name){}
	void draw(const DrawingScaledAttributes & attributes);
};


class List : public LayoutAccess<List> {
public:

	List(const var::String & name, EventLoop * event_loop) :
		LayoutAccess(name, event_loop){
		set_flow(flow_vertical);
	}

	List& add_component(Component& component);

	List& add_filler(
			enum sgfx::Theme::styles style = sgfx::Theme::style_outline_dark
			);

private:
	API_ACCESS_FUNDAMENTAL(List,drawing_size_t,item_height,250);
	API_ACCESS_FUNDAMENTAL(List,u8,border_size,1);

};

}

#endif // SAPI_UX_LIST_HPP
