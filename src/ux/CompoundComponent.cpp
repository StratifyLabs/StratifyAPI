#include "ux/CompoundComponent.hpp"
#include "ux/Scene.hpp"

using namespace sgfx;
using namespace ux;



void CompoundComponent::handle_event(const ux::Event & event){
	if( event.type() == SystemEvent::event_type() ){
		if( event.id() == SystemEvent::id_enter ){
			for(auto component_pointer: m_sub_component_list){
				//reference attributes are the location within the compound component
				//translate reference attributes based on compound component attributes


				if( m_is_initialized == false ){
					component_pointer->reference_drawing_attributes() =
							reference_drawing_attributes() +
							component_pointer->reference_drawing_attributes().point() +
							component_pointer->reference_drawing_attributes().area();
				}

				//add the parent scene
				component_pointer->set_scene( scene() );
				component_pointer->enable( scene()->scene_collection()->display() );
			}
			m_is_initialized = true;
		}
	}

	for(auto component_pointer: m_sub_component_list){
		//pass events to each component
		if( component_pointer->is_enabled() ){
			component_pointer->handle_event(event);
		}
	}

	if( event.type() == SystemEvent::event_type() ){
		if( event.id() == SystemEvent::id_exit ){
			for(auto component_pointer: m_sub_component_list){
				component_pointer->disable();
			}
		}
	}

	for(auto component_pointer: m_sub_component_list){
		if( component_pointer->is_refresh_drawing_pending() ){
			component_pointer->refresh_drawing();
		}
	}

	Component::handle_event(event);
}
