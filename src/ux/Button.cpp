/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "ux/Button.hpp"
#include "ux/Rectangle.hpp"
#include "ux/Text.hpp"
#include "ux/EventLoop.hpp"
#include "sys/Printer.hpp"

using namespace sgfx;
using namespace ux;

void Button::draw_to_scale(const DrawingScaledAttributes & attributes){
	sg_size_t border_size = attributes.height()/2 * m_border_size / 100;
	if( m_border_size && !border_size ){
		border_size = 1; //at least 1 pixel if non-zero
	}

	const Area icon_area = Area(
				attributes.height()*60/100,
				attributes.height()*60/100
				);

	const Point icon_padding(
				(attributes.width()/2 - icon_area.width()/2),
				(attributes.height()/2 - icon_area.height()/2)
				);

	//draw the Border
	attributes.bitmap() << Pen().set_color(theme()->border_color());
	attributes.bitmap().draw_rectangle(
				attributes.region()
				);

	attributes.bitmap() << Pen().set_color(theme()->color());
	attributes.bitmap().draw_rectangle(
				attributes.point() + Point(border_size, border_size),
				attributes.area() - Area(border_size*2, border_size*2)
				);


	//if the icon is available, draw it
	if( m_icon_name.is_empty() == false ){
		Icon()
				.set_icon_font_name(theme()->primary_icon_font_name())
				.set_name(m_icon_name)
				.set_color(theme()->text_color())
				.set_align_center()
				.set_align_middle()
				.draw_to_scale(
					attributes + icon_padding + icon_area
					);
	} else if( m_label.is_empty() == false ){
		//if the label is available, draw it
		Text().set_string(m_label)
				.set_font_name(theme()->primary_font_name())
				.set_color(theme()->text_color())
				.set_align_center()
				.set_align_middle()
				.draw_to_scale(
					attributes
					);
	}

	apply_antialias_filter(attributes);

}

void Button::handle_event(const ux::Event & event){
	//change the state when an event happens in the component
	if( event.type() == ux::TouchEvent::event_type() ){
		const ux::TouchEvent & touch_event
				= event.reinterpret<ux::TouchEvent>();

		if( touch_event.id() == ux::TouchEvent::id_released ){

			if( contains(touch_event.point()) ){
				toggle();
				event_loop()->handle_event(
							ButtonEvent(name(), ButtonEvent::id_released)
							);
			}

			if( theme_state() == Theme::state_highlighted ){
				set_theme_state(Theme::state_default);
				set_refresh_drawing_pending();
			}
		}

		if( (touch_event.id() == ux::TouchEvent::id_pressed) &&
				contains(touch_event.point()) ){
			toggle();
			event_loop()->handle_event(
						ButtonEvent(name(), ButtonEvent::id_pressed)
						);


			set_theme_state(Theme::state_highlighted);
			refresh_drawing();
		}
	}

	Component::handle_event(event);
}
