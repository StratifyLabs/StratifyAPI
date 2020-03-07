/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "ux/Slider.hpp"
#include "ux/draw/Rectangle.hpp"
#include "ux/EventLoop.hpp"

using namespace ux;


void Slider::draw(const DrawingAttributes & attributes){

	const DrawingArea indicator_area(100,1000);
	const DrawingArea slider_area(900,200);

	//draw the background
	draw::Rectangle()
			.set_color(theme()->background_color())
			.draw(attributes, DrawingPoint(0,0), DrawingArea(1000,1000));

	//draw the slider bar
	draw::Rectangle()
			.set_color(theme()->border_color())
			.draw(attributes, DrawingPoint(
							50,
							500 - slider_area.height()/2
							),
						slider_area);

	u16 indicator_range = 1000 - indicator_area.width();

	drawing_int_t indicator_position =
			m_value * indicator_range / m_maximum;

	//draw the position indicator
	draw::Rectangle()
			.set_color(theme()->color())
			.draw(attributes,
						DrawingPoint(indicator_position,0),
						indicator_area
						);

	apply_antialias_filter(attributes);

}

void Slider::handle_event(const ux::Event & event){
	//change the state when an event happens in the component
	if( event.type() == ux::TouchEvent::event_type() ){
		const ux::TouchEvent & touch_event
				= event.reinterpret<ux::TouchEvent>();


		if( (touch_event.id() == ux::TouchEvent::id_pressed) &&
				contains(touch_event.point()) ){
			m_is_touched = true;
			update_touch_point(touch_event.point());
		}

		if( m_is_touched ){
			if( touch_event.id() == ux::TouchEvent::id_released ){
				m_is_touched = false;
				update_touch_point(touch_event.point());
				event_loop()->handle_event(
							SliderEvent(SliderEvent::id_released, *this)
							);
			} else if( touch_event.id() == ux::TouchEvent::id_active ){
				//need to check for dragging
				update_touch_point(touch_event.point());
				event_loop()->handle_event(
							SliderEvent(SliderEvent::id_active, *this)
							);
			}
		}

		if( (touch_event.id() == ux::TouchEvent::id_pressed) &&
							 contains(touch_event.point()) ){
			m_is_touched = true;
			update_touch_point(touch_event.point());
			event_loop()->handle_event(
						SliderEvent(SliderEvent::id_pressed, *this)
						);
		}
	}

	Component::handle_event(event);
}

void Slider::update_touch_point(const sgfx::Point display_point){
	DrawingPoint point = translate_point(display_point);

	if( point.x() < 50 ){
		m_value = 0;
	} else if( point.x() > 950 ){
		m_value = m_maximum;
	} else {
		m_value = (point.x() - 50) * m_maximum / 900;
	}

	redraw();
}
