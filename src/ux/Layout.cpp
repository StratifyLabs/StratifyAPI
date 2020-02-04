#include "ux/Layout.hpp"
#include "ux/Scene.hpp"

using namespace sgfx;
using namespace ux;

Layout::Layout(){
	m_is_initialized = false;
	m_flow = flow_vertical;
	m_origin = DrawingPoint(0,0);
	set_align_left();
	set_align_top();
}

Layout::~Layout(){
	for(auto component_pointer: m_component_list){
		delete component_pointer.component();
	}
}

void Layout::enable(
		hal::Display & display
		){

	m_display = &display; //layout never directly draws on display
	m_reference_drawing_attributes.set_bitmap(display);

	m_touch_gesture.set_region(
				reference_drawing_attributes().calculate_region_on_bitmap()
				);

	for(auto component_pointer: m_component_list){
		component_pointer.component()->set_scene( scene() );
	}

	shift_origin(DrawingPoint(0,0));

	m_is_enabled = true;
}

void Layout::disable(){
	if( m_is_enabled ){
		for(auto component_pointer: m_component_list){
			component_pointer.component()->disable();
		}
		m_is_enabled = false;
	}
}

Layout& Layout::add_component(
		const var::String & name,
		Component& component
		){

	component.set_name(name);
	component.set_drawing_point(
				calculate_next_point(
					component.reference_drawing_attributes().area()
					)
				);

	m_component_list.push_back(
				LayoutComponent(&component)
				);

	//determine scroll ends
	m_area.set_height(
				m_area.height() +
				component.reference_drawing_attributes().area().height()
				);

	return *this;
}

void Layout::shift_origin(DrawingPoint shift){
	m_origin += shift;

	for(auto component_pointer: m_component_list){
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

		if( (overlap.width() * overlap.height()) > 0 ){
			component_pointer.component()->set_refresh_drawing_pending();
			if( component_pointer.component()->is_enabled() == false ){
				component_pointer.component()->enable( scene()->scene_collection()->display() );
			}
		} else {
			if( component_pointer.component()->is_enabled() == true ){
				component_pointer.component()->disable();
			}
		}

		//this calculates if only part of the element should be refreshed (the mask)
		component_pointer.component()->set_refresh_region(
					overlap
					);
	}
}

DrawingPoint Layout::calculate_next_point(const DrawingArea & area){
	//depending on the layout, calculate the point of the next component
	DrawingPoint result(0,0);
	for(auto component_pointer: m_component_list){
		if( m_flow == flow_vertical ){
			result += DrawingPoint::Y(component_pointer.drawing_area().height());
		} else if( m_flow == flow_horizontal ){
			result += DrawingPoint::X(component_pointer.drawing_area().width());
		}
	}

	switch(m_flow){
		case flow_grid: break;
		case flow_vertical:
			//left,right,center alignment
			if( is_align_left() ){
				result.set_x(0);
			} else if( is_align_right() ){
				result.set_x(Drawing::scale() - area.width());
			} else {
				//center
				result.set_x((Drawing::scale() - area.width())/2);
			}
			break;
		case flow_horizontal:
			//top,bottom,middle alignment
			if( is_align_top() ){
				result.set_y(0);
			} else if( is_align_bottom() ){
				result.set_y(Drawing::scale() - area.height());
			} else {
				//middle
				result.set_y((Drawing::scale() - area.height())/2);
			}
			break;
	}

	return result;
}

void Layout::scroll(DrawingPoint value){
	shift_origin(value);

}


void Layout::draw(const DrawingAttributes & attributes){
	for(auto component_pointer: m_component_list){
		if( component_pointer.component()->is_enabled() ){
			component_pointer.component()->draw(attributes);
		}
	}
}

void Layout::handle_event(const ux::Event & event){
	//handle scrolling -- pass events to specific components

	if( (event.type() == SystemEvent::event_type()) &&
			(event.id() == SystemEvent::id_enter) ){
		for(auto component_pointer: m_component_list){
			component_pointer.component()->handle_event(event);
		}
	}


	if( event.type() == ux::TouchEvent::event_type() ){

		enum TouchGesture::id event_id = m_touch_gesture.process_event(event);
		switch(event_id){
			case TouchGesture::id_none: break;
			case TouchGesture::id_touched: break;
			case TouchGesture::id_complete: break;
			case TouchGesture::id_dragged:
				drawing_int_t vertical_drawing_scroll;
				drawing_int_t horizontal_drawing_scroll;
				horizontal_drawing_scroll =
						handle_vertical_scroll( m_touch_gesture.drag().x() );

				vertical_drawing_scroll =
						handle_vertical_scroll( m_touch_gesture.drag().y() );

				if( vertical_drawing_scroll || horizontal_drawing_scroll ){
					this->scroll(
								DrawingPoint(
									horizontal_drawing_scroll,
									vertical_drawing_scroll
									)
								);
				}
				break;
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

drawing_int_t Layout::handle_vertical_scroll(sg_int_t scroll){
	//convert scroll to drawing scale
	drawing_int_t drawing_scroll =
			scroll * Drawing::scale() / reference_drawing_attributes().calculate_height_on_bitmap();

	if( drawing_scroll < 0 ){

		drawing_int_t max_scroll = m_area.height() - 1000 + m_origin.y();
		if( drawing_scroll*-1 > max_scroll ){
			drawing_scroll = -1*max_scroll;
		}

		if( m_origin.y() + m_area.height() <= 1000 ){
			drawing_scroll = 0;
		}

	} else {
		if( drawing_scroll > m_origin.y()*-1 ){
			drawing_scroll = m_origin.y()*-1;
		}

	}

	return drawing_scroll;
}

drawing_int_t Layout::handle_horizontal_scroll(sg_int_t scroll){
	return 0;
}
