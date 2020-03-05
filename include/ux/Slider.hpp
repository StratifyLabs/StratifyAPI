/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_UX_SLIDER_HPP
#define SAPI_UX_SLIDER_HPP

#include "Component.hpp"
#include "TouchGesture.hpp"

namespace ux {

class SliderEvent : public EventObject<EVENT_TYPE('_','s','l','d')> {
	public:

	enum id {
		id_none,
		id_pressed,
		id_active,
		id_released
	};

	SliderEvent(
				const var::String name,
				enum id id,
				u16 value,
				u16 maximum) :
		EventObject<EVENT_TYPE('_','s','l','d')>(id),
		m_name(name),
		m_value(value),
		m_maximum(maximum){

	}

	const var::String & name() const {
		return m_name;
	}

	const u16 value() const {
		return m_value;
	}

	const u16 maximum() const {
		return m_maximum;
	}

	private:
	const var::String & m_name;
	u16 m_value;
	u16 m_maximum;

};

class Slider : public ComponentAccess<
		Slider,
		COMPONENT_SIGNATURE('s','l','d','r')
> {
	public:

	Slider(const var::String & name) : ComponentAccess(name){

	}

	void draw(const DrawingAttributes & attributes);
	void handle_event(const ux::Event & event);

	u16 value(){ return m_value; }
	u16 maximum(){ return m_maximum; }

	Slider & set_value(u16 value){
		m_value = value;
		return *this;
	}

	Slider & set_maximum(u16 value){
		m_maximum = value;
		return *this;
	}

	private:
	u16 m_value;
	u16 m_maximum;
	bool m_is_touched = false;

	void update_touch_point(const sgfx::Point display_point);

};

}

#endif // SLIDER_HPP
