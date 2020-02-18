/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_UX_LABEL_HPP
#define SAPI_UX_LABEL_HPP

#include "../sgfx/Font.hpp"
#include "Component.hpp"
#include "Icon.hpp"

namespace ux {

class Label :
		public ComponentAccess<Label, COMPONENT_SIGNATURE('l','a','b','l')>,
public DrawingAlignment<Label>,
public DrawingComponentProperties<Label> {
	public:

	static u32 whatis_signature(){
		return COMPONENT_SIGNATURE('l','a','b','l');
	}

	Label& set_label(const var::String & value){
		m_label = value;
		return *this;
	}

	Label& set_icon_name(const var::String & value){
		m_icon_name = value;
		return *this;
	}

	void draw_to_scale(const DrawingScaledAttributes & attributes);

	private:
	var::String m_label;
	var::String m_icon_name;

};

}

#endif // SAPI_UX_LABEL_HPP
