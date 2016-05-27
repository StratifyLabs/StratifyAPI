/* Copyright 2014-2016 Fogo Digital, Inc; All Rights Reserved
 *
 */

#ifndef UI_LISTITEMINFO_HPP_
#define UI_LISTITEMINFO_HPP_

#include "../draw/TextAttr.hpp"
#include "../draw/Icon.hpp"
#include "ElementLinked.hpp"
#include "List.hpp"

namespace ui {

class ListItemInfo : public ElementLinked {
public:
	ListItemInfo(ElementLinked * parent = 0);
	ListItemInfo(const char * label, const char * value, ElementLinked * parent = 0);


	const char * label() const { return m_label.text(); }
	const char * value() const { return m_value.text(); }

	void set_font_size(sg_size_t v){ m_label.set_font_size(v); }
	sg_size_t font_size() const { return m_label.font_size(); }

	void set(const char * l, const char * v);
	void set_label(const char * v);
	void set_value(const char * v);

	virtual Element * event_handler(int event, const draw::DrawingAttr & attr);

	var::String & label_str(){ return m_label.str(); }
	var::String & value_str(){ return m_value.str(); }

	void draw_to_scale(const draw::DrawingScaledAttr & attr);
private:
	draw::TextAttr m_label;
	draw::TextAttr m_value;

};

}

#endif /* UI_LISTITEMINFO_HPP_ */
