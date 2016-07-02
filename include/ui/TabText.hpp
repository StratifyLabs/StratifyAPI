/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef UI_TAB_TABLABEL_HPP_
#define UI_TAB_TABLABEL_HPP_

#include "../draw/Text.hpp"
#include "Tab.hpp"

namespace ui {

/*! \brief Tab Text Class
 * \details This class shows a Tab with text on it.
 */
class TabText : public Tab {
public:

	/*! \details Construct a TabText object */
	TabText();
	void draw(const draw::DrawingAttr & attr);

	/*! \details Access the draw::Text object associated with the Tab */
	draw::Text & text(){ return m_text; }

private:
	draw::Text m_text;
};

};

#endif /* UI_TAB_TABLABEL_HPP_ */
