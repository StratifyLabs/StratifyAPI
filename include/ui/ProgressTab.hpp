/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef SAPI_UI_PROGRESSTAB_HPP_
#define SAPI_UI_PROGRESSTAB_HPP_

#include "Tab.hpp"
#include "../draw/ArcProgress.hpp"
#include "../draw/CircleProgress.hpp"

namespace ui {

/*! \brief Tab Progress Class
 * \details This class is a tab that can show a progress bar.
 */
class ProgressTab : public Tab {
public:
	ProgressTab();
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
class ArcProgressTab : public ProgressTab {
public:

	/*! \details Construct a progress arc tab */
	ArcProgressTab(){}
	draw::Progress & progress(){ return m_progress; }
private:
	draw::ArcProgress m_progress;
};

/*! \brief Tab Progress Circle
 * \details This class shows the progress on the tab as a circle (pie).
 */
class CircleProgressTab : public ProgressTab {
public:
	/*! \details Construct a progress circle tab */
	CircleProgressTab(){}
	draw::Progress & progress(){ return m_progress; }
private:
	draw::CircleProgress m_progress;
};


}


#endif /* SAPI_UI_PROGRESSTAB_HPP_ */
