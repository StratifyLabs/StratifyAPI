/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "ux/Slider.hpp"
#include "ux/Rectangle.hpp"
#include "ux/EventLoop.hpp"

using namespace ux;

Slider::Slider()
{

}

void Slider::draw(const DrawingAttributes & attributes){

	const DrawingArea indicator_area(100,1000);
	const DrawingArea slider_area(900,200);

	//draw the background
	Rectangle()
			.set_color(theme()->background_color())
			.draw(attributes, DrawingPoint(0,0), DrawingArea(1000,1000));

	//draw the slider bar
	Rectangle()
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
	Rectangle()
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

		if( (touch_event.id() == ux::TouchEvent::id_released) &&
				contains(touch_event.point()) ){

			DrawingPoint point = translate_point(touch_event.point());

			if( point.x() < 50 ){
				m_value = 0;
			} else if( point.x() > 950 ){
				m_value = m_maximum;
			} else {
				m_value = (point.x() - 50) * m_maximum / 900;
			}

			handle_event(
						SliderEvent(name(), m_value, m_maximum)
						);

			redraw();
		} else if( (touch_event.id() == ux::TouchEvent::id_pressed) &&
							 contains(touch_event.point()) ){
			//need to check for dragging

		}
	}

	Component::handle_event(event);
}
