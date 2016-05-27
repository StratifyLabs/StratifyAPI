/* Copyright 2014-2016 Fogo Digital, Inc; All Rights Reserved
 *
 */

#ifndef UI_TAB_TABLABEL_HPP_
#define UI_TAB_TABLABEL_HPP_

#include "../draw/Text.hpp"
#include "Tab.hpp"

namespace ui {

class TabLabel : public Tab {
public:
	TabLabel();
	void draw(const draw::DrawingAttr & attr);
	draw::Text & label(){ return m_label; }

private:
	draw::Text m_label;
};

};

#endif /* UI_TAB_TABLABEL_HPP_ */
