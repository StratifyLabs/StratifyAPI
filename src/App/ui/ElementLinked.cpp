//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "draw.hpp"
#include "ui/ElementLinked.hpp"
using namespace ui;

ElementLinked::ElementLinked(ElementLinked * parent, ElementLinked * child) {
	// TODO Auto-generated constructor stub
	set_parent(parent);
	set_child(child);
}

Element * ElementLinked::event_handler(int event, const DrawingAttr & attr){
	switch(event){
	case SETUP:
		if( parent() ){
			//set_dim(parent());
		}

		if( child() ){
			child()->event_handler(SETUP, attr);
		}
		break;
	}
	return Element::event_handler(event, attr);
}
