/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "ux/Button.hpp"
#include "ux/Rectangle.hpp"
#include "ux/Text.hpp"
#include "ux/EventLoop.hpp"
#include "sys/Printer.hpp"

using namespace sgfx;
using namespace ux;

void Button::draw_to_scale(const DrawingScaledAttributes & attributes){

	//draw the Border

	draw_base_properties(
				attributes.bitmap(),
				attributes.region(),
				theme()
				);

	Region region_inside_padding =
			calculate_region_inside_padding(attributes.area());

	//if the icon is available, draw it
	if( m_icon_name.is_empty() == false ){
		Icon()
				.set_icon_font_name(theme()->primary_icon_font_name())
				.set_name(m_icon_name)
				.set_color(theme()->text_color())
				.set_align_center()
				.set_align_middle()
				.draw_to_scale(
					attributes + region_inside_padding.point() + region_inside_padding.area()
					);

	} else if( m_label.is_empty() == false ){
		//if the label is available, draw it
		Text().set_string(m_label)
				.set_font_name(theme()->primary_font_name())
				.set_color(theme()->text_color())
				.set_align_center()
				.set_align_middle()
				.draw_to_scale(
					attributes + region_inside_padding.point() + region_inside_padding.area()
					);
	}

	apply_antialias_filter(attributes);

}

void Button::handle_event(const ux::Event & event){
	//change the state when an event happens in the component
	if( event.type() == ux::TouchEvent::event_type() ){
		const ux::TouchEvent & touch_event
				= event.reinterpret<ux::TouchEvent>();

		if( touch_event.id() == ux::TouchEvent::id_dragged ){
			if( theme_state() == Theme::state_highlighted ){
				set_theme_state(Theme::state_default);
				set_refresh_drawing_pending();
			}
		}

		if( touch_event.id() == ux::TouchEvent::id_released ){

			if( contains(touch_event.point()) &&
					(theme_state() == Theme::state_highlighted) ){
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
