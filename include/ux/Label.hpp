/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_UX_LABEL_HPP
#define SAPI_UX_LABEL_HPP

#include "../sgfx/Font.hpp"
#include "Component.hpp"
#include "Icon.hpp"

namespace ux {

#define LABEL_SIGNATURE COMPONENT_SIGNATURE('l','a','b','l')

class Label :
		public ComponentAccess<Label, LABEL_SIGNATURE>,
public DrawingAlignment<Label>,
public DrawingComponentProperties<Label> {
	public:


	Label(const var::String & name) : ComponentAccess(name){

	}

	static u32 whatis_signature(){
		return LABEL_SIGNATURE;
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
