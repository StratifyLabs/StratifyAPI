/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_UX_LABEL_HPP
#define SAPI_UX_LABEL_HPP

#include "../sgfx/Font.hpp"
#include "Component.hpp"
#include "draw/Icon.hpp"

namespace ux {

class Label :
		public ComponentAccess<Label> {
	public:


	Label(const var::String & name) : ComponentAccess(name){
		set_label(name);
	}

	Label& set_label(const var::String & value){
		m_label = value;
		return *this;
	}

	Label& set_icon_name(const var::String & value){
		m_icon_name = value;
		return *this;
	}

	void draw(const DrawingScaledAttributes & attributes);

	const var::String& label() const {
		return m_label;
	}

	const var::String& icon_name() const {
		return m_icon_name;
	}

	private:
	var::String m_label;
	var::String m_icon_name;

};

}

#endif // SAPI_UX_LABEL_HPP
