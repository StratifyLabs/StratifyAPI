//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "draw.hpp"
#include "ui/ElementLinked.hpp"
using namespace ui;

ElementLinked::ElementLinked(){
	set_parent(0);
	set_child(0);
}

ElementLinked::ElementLinked(ElementLinked * parent, ElementLinked * child) {
	// TODO Auto-generated constructor stub
	set_parent(parent);
	set_child(child);
}

Element * ElementLinked::handle_event(const Event  & event, const DrawingAttr & attr){
	switch(event.type()){
	case Event::SETUP:
		if( parent() ){
			//set_dim(parent());
		}

		if( child() ){
			child()->handle_event(event, attr);
		}
		break;
	default:
		break;
	}
	return Element::handle_event(event, attr);
}
