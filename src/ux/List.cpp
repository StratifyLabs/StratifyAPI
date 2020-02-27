/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "ux/List.hpp"
#include "ux/draw/Rectangle.hpp"
#include "ux/draw/Text.hpp"
#include "ux/draw/Icon.hpp"
#include "ux/EventLoop.hpp"

using namespace sgfx;
using namespace ux;

void ListItem::draw_to_scale(const DrawingScaledAttributes & attributes){

	draw_base_properties(
				attributes.bitmap(),
				attributes.region(),
				theme()
				);

	Region region_inside_padding = calculate_region_inside_padding(
				attributes.area()
				);


	draw::Text()
			.set_string(key())
			.set_font_name(theme()->primary_font_name())
			.set_color(theme()->text_color())
			.set_align_left()
			.set_align_middle()
			.draw_to_scale(
				attributes + region_inside_padding.point() + region_inside_padding.area()
				);

	if( m_icon.is_empty() == false ){
		draw::Icon()
				.set_icon_font_name(theme()->primary_icon_font_name())
				.set_name(m_icon)
				.set_color(theme()->text_color())
				.set_align_right()
				.draw_to_scale(
					attributes +
					region_inside_padding.point() +
					region_inside_padding.area()
					);
	} else  if( m_value.is_empty() == false ){
		draw::Text()
				.set_font_name(theme()->primary_font_name())
				.set_string(value())
				.set_color(theme()->text_color())
				.set_align_right()
				.set_align_middle()
				.draw_to_scale(
					attributes +
					region_inside_padding.point() +
					region_inside_padding.area()
					);
	}
}


void ListItem::handle_event(const ux::Event & event){
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
			if( theme_state() == Theme::state_highlighted ){
				if( contains(
							touch_event.point()
							) ){
					event_loop()->trigger_event(
								ListEvent(*this)
								);
				}
				set_theme_state(Theme::state_default);
				set_refresh_drawing_pending();
			}
		}

		if( touch_event.id() == ux::TouchEvent::id_pressed ){

			if( contains(
						touch_event.point()
						) ){
				set_theme_state(Theme::state_highlighted);
				set_refresh_drawing_pending();
			}
		}
	}
	Component::handle_event(event);
}

List& List::add_component(
		Component& component
		){
	component.set_drawing_area(
				DrawingArea(1000, m_item_height)
				);
	//component.set_theme_style( theme_style() );
	return LayoutAccess<List>::add_component(component);
}
