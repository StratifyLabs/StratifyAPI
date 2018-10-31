/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef UI_TEXTTAB_HPP_
#define UI_TEXTTAB_HPP_

#include "../draw/Text.hpp"
#include "Tab.hpp"

namespace ui {

/*! \brief Tab Text Class
 * \details This class shows a Tab with text on it.
 */
class TextTab : public Tab {
public:

	/*! \details Construct a TabText object */
	TextTab();
	void draw(const draw::DrawingAttr & attr);

	/*! \details Access the draw::Text object associated with the Tab */
	draw::Text & text(){ return m_text; }

private:
	draw::Text m_text;
};

}

#endif /* UI_TEXTTAB_HPP_ */
