//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "draw.hpp"
#include "ui/ElementLinked.hpp"
using namespace ui;

ElementLinked::ElementLinked(ElementLinked * parent) {
	// TODO Auto-generated constructor stub
	set_parent(parent);
	set_child(0);
}

ElementLinked::ElementLinked(ElementLinked * child, ElementLinked * parent){
	set_child(child);
	set_parent(parent);
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
