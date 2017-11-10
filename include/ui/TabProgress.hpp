/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef UI_TAB_TABPROGRESS_HPP_
#define UI_TAB_TABPROGRESS_HPP_

#include "Tab.hpp"
#include "../draw/ProgressArc.hpp"
#include "../draw/ProgressCircle.hpp"

namespace ui {

/*! \brief Tab Progress Class
 * \details This class is a tab that can show a progress bar.
 */
class TabProgress : public Tab {
public:
	TabProgress();
	virtual void draw(const draw::DrawingAttr & attr);

	/*! \details This method gives access to the draw::Progress
	 * object so that the user can set the attributes of the progress.
	 *
	 * @return A reference to the draw::Progress object.
	 */
	virtual draw::Progress & progress() = 0;

};

/*! \brief Tab Progress Arc
 * \details This class shows the progress on the tab as an arc.
 */
class TabProgressArc : public TabProgress {
public:

	/*! \details Construct a progress arc tab */
	TabProgressArc(){}
	draw::Progress & progress(){ return m_progress; }
private:
	draw::ProgressArc m_progress;
};

/*! \brief Tab Progress Circle
 * \details This class shows the progress on the tab as a circle (pie).
 */
class TabProgressCircle : public TabProgress {
public:
	/*! \details Construct a progress circle tab */
	TabProgressCircle(){}
	draw::Progress & progress(){ return m_progress; }
private:
	draw::ProgressCircle m_progress;
};

};


#endif /* UI_TAB_TABPROGRESS_HPP_ */
