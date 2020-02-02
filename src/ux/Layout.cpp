#include "ux/Layout.hpp"
#include "ux/Scene.hpp"

using namespace ux;

Layout::Layout(){
	m_is_initialized = false;
	m_flow = flow_vertical;
	m_origin = DrawingPoint(0,0);
}

Layout::~Layout(){
	for(auto component_pointer: m_component_list){
		delete component_pointer.component();
	}
}

Layout& Layout::add_component(
		const var::String & name,
		Component& component
		){

	component.set_name(name);
	component.set_drawing_point( calculate_next_point() );
	m_component_list.push_back(
				LayoutComponent(&component)
				);

	return *this;
}

void Layout::shift_origin(DrawingPoint shift){
	m_origin += shift;
	for(auto component_pointer: m_component_list){

		component_pointer.component()->set_scene( scene() );

		//reference attributes are the location within the compound component
		//translate reference attributes based on compound component attributes
		component_pointer.component()->reference_drawing_attributes() =
				reference_drawing_attributes() +
				m_origin +
				component_pointer.drawing_point() +
				component_pointer.drawing_area();

		sgfx::Region layout_region =
				reference_drawing_attributes().calculate_region_on_bitmap();
		sgfx::Region component_region =
				component_pointer.component()->reference_drawing_attributes().calculate_region_on_bitmap();

		sgfx::Region overlap = layout_region.overlap(component_region);

		printf("draw %s ", component_pointer.component()->name().cstring());
		printf("at %d,%d %dx%d\n",
					 component_pointer.component()->reference_drawing_attributes().point().x(),
					 component_pointer.component()->reference_drawing_attributes().point().y(),
					 component_pointer.component()->reference_drawing_attributes().area().width(),
					 component_pointer.component()->reference_drawing_attributes().area().height()
					 );

		printf("original at %d,%d %dx%d\n",
					 component_pointer.drawing_point().x(),
					 component_pointer.drawing_point().y(),
					 component_pointer.drawing_area().width(),
					 component_pointer.drawing_area().height()
					 );

		printf("layout region %d,%d %dx%d\n",
					 layout_region.x(),
					 layout_region.y(),
					 layout_region.width(),
					 layout_region.height()
					 );

		printf("component region %d,%d %dx%d\n",
					 component_region.x(),
					 component_region.y(),
					 component_region.width(),
					 component_region.height()
					 );

		printf("overlap region %d,%d %dx%d\n",
					 overlap.x(),
					 overlap.y(),
					 overlap.width(),
					 overlap.height()
					 );

		if( (overlap.width() * overlap.height()) > 0 ){
			component_pointer.set_visible(true);
			component_pointer.component()->set_refresh_drawing_pending();
			component_pointer.component()->enable( scene()->scene_collection()->display() );
		} else {
			component_pointer.set_visible(false);
			component_pointer.component()->disable();
		}

		//this calculates if only part of the element should be refreshed (the mask)
		component_pointer.component()->set_refresh_region(
					overlap
					);
	}
}

void Layout::enter(){
	shift_origin(DrawingPoint(0,0));
	for(auto component_pointer: m_component_list){
		//add the parent scene
		//only enable if the component is visible
		if( component_pointer.is_visible() ){
		}
	}
	shift_origin(DrawingPoint(0,0));
}

DrawingPoint Layout::calculate_next_point(){
	//depending on the layout, calculate the point of the next component
	DrawingPoint result(0,0);
	for(auto component_pointer: m_component_list){
		if( m_flow == flow_vertical ){
			result += DrawingPoint::Y(component_pointer.drawing_area().height());
		} else if( m_flow == flow_horizontal ){
			result += DrawingPoint::X(component_pointer.drawing_area().width());
		}
	}
	return result;
}


void Layout::draw(const DrawingAttributes & attributes){
	for(auto component_pointer: m_component_list){
		component_pointer.component()->draw(attributes);
	}
}

void Layout::handle_event(const ux::Event & event){
	//handle scrolling -- pass events to specific components

	if( (event.type() == SystemEvent::event_type()) &&
			(event.id() == SystemEvent::id_enter) ){
		enter();
	}

	if( event.type() == ux::TouchEvent::event_type() ){
		const ux::TouchEvent & touch_event
				= event.reinterpret<ux::TouchEvent>();

		if( (touch_event.id() == ux::TouchEvent::id_pressed) &&
				contains(touch_event.point()) ){
			//start scrolling with the touch movement
			m_is_drag_active = true;
			m_touch_last = touch_event.point();
		} else if( (touch_event.id() == ux::TouchEvent::id_active) &&
							 m_is_drag_active ){

			sg_int_t vertical_scroll = touch_event.point().y() -
					m_touch_last.y();

			m_touch_last = touch_event.point();

			//convert scroll to drawing scale
			drawing_int_t drawing_scroll = vertical_scroll * 1000 / 240;

			scroll(DrawingPoint(0, drawing_scroll));

		} else if( (touch_event.id() == ux::TouchEvent::id_released) &&
							 m_is_drag_active ){
			m_is_drag_active = false;
		}

	}

	for(auto component_pointer: m_component_list){
		//pass events to each component
		if( component_pointer.component()->is_enabled() ){
			component_pointer.component()->handle_event(event);
		}
	}

	for(auto component_pointer: m_component_list){
		if( component_pointer.component()->is_refresh_drawing_pending() ){
			component_pointer.component()->refresh_drawing();
		}
	}

	if( event.type() == SystemEvent::event_type() ){
		if( event.id() == SystemEvent::id_exit ){
			for(auto component_pointer: m_component_list){
				component_pointer.component()->disable();
			}
		}
	}

	Component::handle_event(event);

}
