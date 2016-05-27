/* Copyright 2014-2016 Fogo Digital, Inc; All Rights Reserved
 *
 */

#ifndef UI_TAB_TABPROGRESS_HPP_
#define UI_TAB_TABPROGRESS_HPP_

#include "Tab.hpp"
#include "../draw/ProgressArc.hpp"
#include "../draw/ProgressCircle.hpp"

namespace ui {

class TabProgress : public Tab {
public:
	TabProgress();
	virtual void draw(const draw::DrawingAttr & attr);

	virtual draw::ProgressObject * progress() = 0;
	draw::ProgressAttr & attr(){ return progress()->attr(); }

};

class TabProgressArc : public TabProgress {
public:
	TabProgressArc(){}
	draw::ProgressObject * progress(){ return &m_progress; }
private:
	draw::ProgressArc m_progress;
};

class TabProgressCircle : public TabProgress {
public:
	TabProgressCircle(){}
	draw::ProgressObject * progress(){ return &m_progress; }
private:
	draw::ProgressCircle m_progress;
};

};


#endif /* UI_TAB_TABPROGRESS_HPP_ */
