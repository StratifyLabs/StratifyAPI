/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_UX_COMPONENT_HPP
#define SAPI_UX_COMPONENT_HPP

#include "../api/WorkObject.hpp"
#include "../hal/Display.hpp"
#include "../var/String.hpp"
#include "../sgfx/Theme.hpp"
#include "Drawing.hpp"
#include "Event.hpp"

namespace ux {

class EventLoop;

class Component : public Drawing {
public:

	using EventHandlerFunction = std::function<void(Component * object, const Event & event)>;
	virtual ~Component();

	Component& set_event_handler(EventHandlerFunction event_handler){
		m_event_handler = event_handler;
		return *this;
	}

	Component& set_drawing_area(const DrawingArea & drawing_area){
		m_reference_drawing_attributes.set_area(drawing_area);
		return *this;
	}

	Component& set_drawing_point(const DrawingPoint & drawing_point){
		m_reference_drawing_attributes.set_point(drawing_point);
		return *this;
	}

	Component& set_theme_style(enum sgfx::Theme::style value){
		m_theme_style = value;
		return *this;
	}

	Component& set_theme_state(enum sgfx::Theme::state value){
		m_theme_state = value;
		return *this;
	}

	Component& set_event_loop(EventLoop * event_loop){
		m_event_loop = event_loop;
		return *this;
	}

	virtual Component& set_enabled(bool value = true){
		m_is_enabled = value;
		if( value == false ){
			set_visible(false);
		}
		return *this;
	}

	bool is_enabled() const {
		return m_is_enabled;
	}


	enum sgfx::Theme::style theme_style() const {
		return m_theme_style;
	}

	enum sgfx::Theme::state theme_state() const {
		return m_theme_state;
	}

	bool is_antialias() const {
		return m_is_antialias;
	}

	Component& set_antialias(bool value = true){
		m_is_antialias = value;
		return *this;
	}

	virtual void set_visible(bool value = true);

	bool is_visible() const {
		return m_is_visible;
	}


	const sgfx::Theme * theme() const;
	const hal::Display * display() const;
	hal::Display * display();

	//update the location of the component (allow animations)

	virtual void handle_event(const ux::Event & event){
		if( m_event_handler ){
			m_event_handler(this, event);
		}
	}

	const var::String & name() const {
		return m_name;
	}

	void apply_antialias_filter(const DrawingAttributes & attributes);
	void apply_antialias_filter(const DrawingScaledAttributes & attributes);

	void redraw(){
		draw(local_drawing_attributes());
		set_refresh_drawing_pending();
	}

	sgfx::Region region() const {
		return m_reference_drawing_attributes.calculate_region_on_bitmap();
	}

	bool contains(const sgfx::Point & point){
		return sgfx::Region(
					m_reference_drawing_attributes.calculate_region_on_bitmap()
					).contains(point);
	}

	DrawingPoint translate_point(const sgfx::Point & point);

	void erase();

	void set_refresh_drawing_pending(){
		m_is_refresh_drawing_pending = true;
	}

	EventLoop * event_loop(){
		return m_event_loop;
	}

	const EventLoop * event_loop() const {
		return m_event_loop;
	}

protected:

	bool m_is_visible = false;
	bool m_is_enabled = false;

	void set_refresh_region(const sgfx::Region & region){
		if( region.width() * region.height() == 0 ){
			m_refresh_region = m_local_bitmap.region();
		} else {
			m_refresh_region = region;
		}
	}

	bool is_refresh_drawing_pending() const {
		return m_is_refresh_drawing_pending;
	}

	void refresh_drawing();
	friend class Layout;
	friend class LayoutComponent;

	const DrawingAttributes & local_drawing_attributes() const {
		return m_local_drawing_attributes;
	}

	const DrawingAttributes& reference_drawing_attributes() const {
		return m_reference_drawing_attributes;
	}

	DrawingAttributes& reference_drawing_attributes(){
		return m_reference_drawing_attributes;
	}

protected:

private:

	var::String m_name;
	u32 m_type_id;
	//needs to know where on the display it is drawn
	DrawingAttributes m_reference_drawing_attributes;
	DrawingAttributes m_local_drawing_attributes;
	sgfx::Bitmap m_local_bitmap;
	enum sgfx::Theme::style m_theme_style = sgfx::Theme::style_brand_primary;
	enum sgfx::Theme::state m_theme_state = sgfx::Theme::state_default;
	bool m_is_antialias = true;
	bool m_is_refresh_drawing_pending;
	sgfx::Region m_refresh_region;

	//needs a palette to use while drawing
	sgfx::Palette * m_palette = nullptr;
	EventLoop * m_event_loop = nullptr;
	EventHandlerFunction m_event_handler;

	void set_name(const var::String & name){
		m_name = name;
	}


};


}

#endif // SAPI_UX_COMPONENT_HPP
