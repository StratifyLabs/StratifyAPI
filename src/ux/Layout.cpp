/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "ux/Layout.hpp"
#include "ux/Scene.hpp"
#include "sys/Printer.hpp"

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

	if( m_is_enabled == false ){
		m_display = &display; //layout never directly draws on display
		m_reference_drawing_attributes.set_bitmap(display);
		m_is_enabled = true;
	}

	set_refresh_region(reference_drawing_attributes().calculate_region_on_bitmap());
	m_touch_gesture.set_region(
				reference_drawing_attributes().calculate_region_on_bitmap()
				);

	for(auto component_pointer: m_component_list){
		component_pointer.component()->set_scene( scene() );
	}

	shift_origin(DrawingPoint(0,0));

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
	m_component_list.push_back(
				LayoutComponent(&component)
				);

	return *this;
}

void Layout::shift_origin(DrawingPoint shift){
	m_origin += shift;

	//determine scroll ends
	generate_layout_positions();

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
			component_pointer.component()->enable( scene()->scene_collection()->display() );
		} else {
			component_pointer.component()->disable();
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
					printf("vertical scroll is %d for %s\n", vertical_drawing_scroll, name().cstring());
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
		component_pointer.component()->handle_event(event);
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

void Layout::generate_layout_positions(){
	switch(m_flow){
		default:
		case flow_vertical: generate_vertical_layout_positions(); return;
		case flow_horizontal: generate_horizontal_layout_positions(); return;
	}
}

void Layout::generate_vertical_layout_positions(){
	drawing_int_t drawing_cursor = 0;
	sg_int_t bitmap_cursor = 0;

	for(auto & component: m_component_list){
		const DrawingPoint point(0, drawing_cursor);
		const DrawingArea area(1000, component.drawing_area().height());
		component.set_drawing_point(point);

		DrawingAttributes tmp_attributes = reference_drawing_attributes() + m_origin + point + area;
		sg_size_t height_on_bitmap = tmp_attributes.calculate_area_on_bitmap().height;

#if 0
		printf("%d (%d) - %d (%d) -- %d\n",
					 bitmap_cursor,
					 height_on_bitmap,
					 drawing_cursor,
					 component.drawing_area().height(),
					 tmp_attributes.calculate_point_on_bitmap().y - 54
					 );
#endif

		bitmap_cursor += height_on_bitmap;
		drawing_cursor = reference_drawing_attributes().calculate_height_on_drawing(bitmap_cursor);
	}

	m_area.set_height(drawing_cursor);
}

void Layout::generate_horizontal_layout_positions(){
	drawing_int_t drawing_cursor = 0;
	sg_int_t bitmap_cursor = 0;

	for(auto & component: m_component_list){
		const DrawingPoint point(drawing_cursor, 0);
		const DrawingArea area(component.drawing_area().width(), 1000);
		component.set_drawing_point(point);

		DrawingAttributes tmp_attributes = reference_drawing_attributes() + area;
		sg_size_t width_on_bitmap = tmp_attributes.calculate_area_on_bitmap().width;

		bitmap_cursor += width_on_bitmap;
		drawing_cursor = reference_drawing_attributes().calculate_width_on_drawing(bitmap_cursor);
	}

	m_area.set_width(drawing_cursor);
}
