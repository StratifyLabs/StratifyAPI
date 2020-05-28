/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "ux/Layout.hpp"
#include "ux/EventLoop.hpp"
#include "sys/Printer.hpp"

using namespace sgfx;
using namespace ux;

Layout::Layout(
		const var::String & name,
		EventLoop* event_loop
		) : ComponentAccess(name){
	set_event_loop(event_loop);
	m_flow = flow_free;
	m_origin = DrawingPoint(0,0);
	set_align_left();
	set_align_top();
	set_layout();
}

Layout::~Layout(){
	for(auto component_pointer: m_component_list){
		delete component_pointer.component();
	}
}

bool Layout::transition(
		const var::String & next_layout_name
		){

	if( is_layout() == false ){
		return false;
	}

	Component * next = nullptr;
	for(auto & cp: m_component_list){
		if( (cp.component()->is_layout() ) &&
				(cp.component()->name() == next_layout_name) ){
			next = cp.component();
			break;
		}
	}

	return transition(next->reinterpret<Layout>());
}

bool Layout::transition(
		Layout * next_layout
		){
	if( next_layout ){
		for(auto & cp: m_component_list){
			//disable all other layouts
			if( cp.component()->is_layout() ){
				cp.component()->set_enabled_internal(false);
			}
		}
		next_layout->set_enabled_internal(true);
		return true;
	}
	printf("not found\n");
	return false;
}

void Layout::examine_visibility(){
	if( is_ready_to_draw() ){

		if( m_is_created == false ){
			printf("fatal %s was note created using Component::create()\n", name().cstring());
			exit(1);
		}

		if( display() == nullptr ){
			m_is_visible = false;
			return;
		}

		m_reference_drawing_attributes.set_bitmap(*display());
		m_refresh_region =
				Region(
					Point(0,0),
					Region(reference_drawing_attributes().calculate_region_on_bitmap()).area()
					);

		m_touch_gesture.set_region(
					reference_drawing_attributes().calculate_region_on_bitmap()
					);

		shift_origin(DrawingPoint(0,0));

		if( m_event_handler ){
			m_event_handler(this, SystemEvent(SystemEvent::id_enter));
		}
	} else {
		//if layout is enabled and visible -- components are not visible
		//erase();
		for(auto component_pointer: m_component_list){
			component_pointer.component()->set_visible_internal(false);
		}
		if( m_event_handler ){
			m_event_handler(this, SystemEvent(SystemEvent::id_exit));
		}
	}

}

void Layout::set_refresh_region(const sgfx::Region & region){
	if( is_ready_to_draw() ){
		sgfx::Region layout_region =
				reference_drawing_attributes()
				.calculate_region_on_bitmap();

		layout_region =
				Region(
					layout_region.point() + region.point(),
					region.area()
					);

		for(auto component_pointer: m_component_list){
			sgfx::Region component_region =
					component_pointer.component()->reference_drawing_attributes()
					.calculate_region_on_bitmap();

			component_pointer.component()->set_refresh_region(
						layout_region.overlap(component_region)
						);
		}
	}
	m_refresh_region = region;
}

Layout& Layout::add_component(
		Component& component
		){

	component.set_event_loop( event_loop() );
	component.set_parent(this);
	m_component_list.push_back(
				LayoutComponent(&component)
				);
	return *this;
}

void Layout::update_drawing_point(
		const Component * component,
		const DrawingPoint & point
		){
	for(auto & cp: m_component_list){
		if( component == cp.component() ){
			cp.set_drawing_point(point);
			shift_origin(DrawingPoint(0,0));
			return;
		}
	}
}

void Layout::update_drawing_area(
		const Component * component,
		const DrawingArea & area
		){
	for(auto & cp: m_component_list){
		if( component == cp.component() ){
			cp.set_drawing_area(area);
			shift_origin(DrawingPoint(0,0));
			return;
		}
	}
}

void Layout::shift_origin(DrawingPoint shift){
	m_origin += shift;

	if( is_ready_to_draw() ){
		//determine scroll ends
		generate_layout_positions();

		sgfx::Region layout_region =
				reference_drawing_attributes()
				.calculate_region_on_bitmap();

		layout_region =
				Region(
					layout_region.point()
					+ m_refresh_region.point(),
					m_refresh_region.area()
					);

		for(LayoutComponent& component_pointer: m_component_list){
			//reference attributes are the location within the compound component
			//translate reference attributes based on compound component attributes
			component_pointer.component()->reference_drawing_attributes() =
					reference_drawing_attributes() +
					m_origin +
					component_pointer.drawing_point() +
					component_pointer.drawing_area();

			sgfx::Region component_region =
					component_pointer.component()->reference_drawing_attributes()
					.calculate_region_on_bitmap();

			sgfx::Region overlap = layout_region.overlap(component_region);

			if( (overlap.width() * overlap.height()) > 0 ){
				//this calculates if only part of the element should be refreshed (the mask)

				component_pointer.component()->set_refresh_drawing_pending();

				if( component_pointer.component()->is_visible() ){
					component_pointer.component()->touch_drawing_attributes();
				} else {
					component_pointer.component()->set_visible_internal(true);
				}

				component_pointer.component()->set_refresh_region(
							overlap
							);

			} else {
				component_pointer.component()->set_visible_internal(false);
			}

		}
	}

}

DrawingPoint Layout::calculate_next_point(
		const DrawingPoint& point,
		const DrawingArea & area
		){
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
		case flow_free:
			result = point;
			break;
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
	if( is_ready_to_draw() ){
		shift_origin(value);
	}
}


void Layout::draw(const DrawingAttributes & attributes){
	for(const auto& component_pointer: m_component_list){
		if( component_pointer.component()->is_visible() ){
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
					this->scroll(
								DrawingPoint(
									horizontal_drawing_scroll,
									vertical_drawing_scroll
									)
								);

					event_loop()->handle_event(
								TouchEvent(TouchEvent::id_dragged, Point(0,0))
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
		if( component_pointer.component()->is_enabled() &&
				component_pointer.component()->is_refresh_drawing_pending() ){
			component_pointer.component()->refresh_drawing();
		}
	}

	if( event.type() == SystemEvent::event_type() ){
		if( event.id() == SystemEvent::id_exit ){
			for(auto component_pointer: m_component_list){
				component_pointer.component()->set_visible_internal(false);
			}
		}
	}

	if( m_event_handler ){
		m_event_handler(this, event);
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
		case flow_free: generate_free_layout_positions(); return;
		case flow_vertical: generate_vertical_layout_positions(); return;
		case flow_horizontal: generate_horizontal_layout_positions(); return;
	}
}

void Layout::generate_free_layout_positions(){
	drawing_int_t x_max = 0;
	drawing_int_t y_max = 0;

	for(auto & component: m_component_list){
		if( component.drawing_point().x() + component.drawing_area().width() > x_max ){
			x_max = component.drawing_point().x() + component.drawing_area().width();
		}

		if( component.drawing_point().y() + component.drawing_area().height() > y_max ){
			x_max = component.drawing_point().y() + component.drawing_area().height();
		}
	}

	m_area = DrawingArea(x_max, y_max);
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

		bitmap_cursor += height_on_bitmap-1;
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

		bitmap_cursor += width_on_bitmap-1;
		drawing_cursor = reference_drawing_attributes().calculate_width_on_drawing(bitmap_cursor);
	}

	m_area.set_width(drawing_cursor);
}
