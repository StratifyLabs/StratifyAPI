/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "ux/List.hpp"
#include "ux/draw/Rectangle.hpp"
#include "ux/draw/Text.hpp"
#include "ux/draw/Icon.hpp"
#include "ux/EventLoop.hpp"

using namespace sgfx;
using namespace ux;

void ListItem::draw(const DrawingScaledAttributes & attributes){

	draw_base_properties(
				attributes.bitmap(),
				attributes.region(),
				theme()
				);

	Region region_inside_padding = calculate_region_inside_padding(
				attributes.region()
				);


	draw::Text()
			.set_string(key())
			.set_font_name(theme()->primary_font_name())
			.set_color(theme()->text_color())
			.set_align_left()
			.set_align_middle()
			.draw(
				attributes + region_inside_padding.point() + region_inside_padding.area()
				);

	if( m_value.find("icon@") == 0 ){
		draw::Icon()
				.set_icon_font_name(theme()->primary_icon_font_name())
				.set_name(m_value)
				.set_color(theme()->text_color())
				.set_align_right()
				.draw(
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
				.draw(
					attributes +
					region_inside_padding.point() +
					region_inside_padding.area()
					);
	}
}

void ListFiller::draw(const DrawingScaledAttributes & attributes){
	draw_base_properties(
				attributes.bitmap(),
				attributes.region(),
				theme()
				);
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

List& List::add_filler(enum sgfx::Theme::styles style){

	drawing_size_t height = 0;
	if( component_list().count() ){
		height = component_list().back().component()->reference_drawing_attributes().point().y() +
				component_list().back().component()->reference_drawing_attributes().area().height();
	}
	if( height < 1010 ){
		ListFiller * list_filler =
				&(ListFiller::create(name() + "Filler")
					.set_drawing_area(1000, 1010 - height)
					.set_theme_style(style)
					);
		set_vertical_scroll_enabled(false);
		return LayoutAccess<List>::add_component(*list_filler);
	}

	return *this;

}
