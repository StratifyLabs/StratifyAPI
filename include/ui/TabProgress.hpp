/* Copyright 2014-2016 Fogo Digital, Inc; All Rights Reserved
 *
 */

#ifndef UI_TAB_TABPROGRESS_HPP_
#define UI_TAB_TABPROGRESS_HPP_

#include "Tab.hpp"
#include "../draw/ProgressArc.hpp"
#include "../draw/ProgressCircle.hpp"

namespace ui {

class TabProgressObject : public Tab {
public:
	TabProgressObject();
	virtual void draw(const draw::DrawingAttr & attr);

	virtual draw::Progress * progress() = 0;
	draw::ProgressAttr & attr(){ return progress()->attr(); }

};

class TabProgressArc : public TabProgressObject {
public:
	TabProgressArc(){}
	draw::Progress * progress(){ return &m_progress; }
private:
	draw::ProgressArc m_progress;
};

class TabProgressCircle : public TabProgressObject {
public:
	TabProgressCircle(){}
	draw::Progress * progress(){ return &m_progress; }
private:
	draw::ProgressCircle m_progress;
};

};


#endif /* UI_TAB_TABPROGRESS_HPP_ */
