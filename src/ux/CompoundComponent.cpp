/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "ux/CompoundComponent.hpp"
#include "ux/Scene.hpp"

using namespace sgfx;
using namespace ux;

void CompoundComponent::initialize(){
	if( m_is_initialized == false ){
		for(auto component_pointer: m_sub_component_list){
			//reference attributes are the location within the compound component
			//translate reference attributes based on compound component attributes
			component_pointer->reference_drawing_attributes() =
					reference_drawing_attributes() +
					component_pointer->reference_drawing_attributes().point() +
					component_pointer->reference_drawing_attributes().area();

			//add the parent scene
			component_pointer->set_scene( scene() );
			component_pointer->enable( scene()->scene_collection()->display() );
		}
	}
	m_is_initialized = true;
}


void CompoundComponent::handle_event(const ux::Event & event){
	if( event.type() == SystemEvent::event_type() ){
		if( event.id() == SystemEvent::id_enter ){
			initialize();
			for(auto component_pointer: m_sub_component_list){
				//add the parent scene
				component_pointer->set_scene( scene() );
				component_pointer->enable( scene()->scene_collection()->display() );
			}
		}
	}

	for(auto component_pointer: m_sub_component_list){
		//pass events to each component
		if( component_pointer->is_enabled() ){
			component_pointer->handle_event(event);
		}
	}

	for(auto component_pointer: m_sub_component_list){
		if( component_pointer->is_refresh_drawing_pending() ){
			component_pointer->refresh_drawing();
		}
	}

	if( event.type() == SystemEvent::event_type() ){
		if( event.id() == SystemEvent::id_exit ){
			for(auto component_pointer: m_sub_component_list){
				component_pointer->disable();
			}
		}
	}

	Component::handle_event(event);
}
