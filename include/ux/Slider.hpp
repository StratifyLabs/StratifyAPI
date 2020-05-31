/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_UX_SLIDER_HPP
#define SAPI_UX_SLIDER_HPP

#include "Component.hpp"
#include "TouchGesture.hpp"

namespace ux {

class Slider;

class SliderEvent : public EventObject<Slider, EVENT_TYPE('_','s','l','d')> {
public:

	enum id {
		id_none,
		id_pressed,
		id_active,
		id_released
	};

	SliderEvent(
				enum id id,
				Slider& slider) :
		EventObject(id, &slider){}

};

class Slider : public ComponentAccess<Slider>{
public:

	COMPONENT_PREFIX(Slider)

	Slider(const var::String & name) :
		ComponentAccess(
			prefix() + name
			){}

	void draw(const DrawingScaledAttributes & attributes);
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
