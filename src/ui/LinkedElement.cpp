//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "draw.hpp"
#include "ui/LinkedElement.hpp"
using namespace ui;

LinkedElement::LinkedElement(){
	set_parent(0);
	set_child(0);
}

LinkedElement::LinkedElement(LinkedElement * parent, LinkedElement * child) {
	// TODO Auto-generated constructor stub
	set_parent(parent);
	set_child(child);
}

Element * LinkedElement::handle_event(const Event  & event, const DrawingAttr & attr){
	switch(event.type()){
		case Event::SETUP:
			if( child() && (child() != this) ){
				child()->handle_event(event, attr);
			}
			break;
		default:
			break;
	}
	return Element::handle_event(event, attr);
}
