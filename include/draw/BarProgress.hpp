/*! \file */ //Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#ifndef SAPI_DRAW_BARPROGRESS_HPP_
#define SAPI_DRAW_BARPROGRESS_HPP_

#include "Progress.hpp"

namespace draw {

/*! \brief Bar Progress Class
 * \details The Bar Progress Class displays a progress bar on the screen.
 *
 * The following code snippet will draw a progress bar using
 * the given DrawingAttributes. It will be drawn along the top
 * using 100% of the width (1000/1000) and 10% of the height (100/1000).
 *
 * ```
 * DrawingAttributes attributes;
 *
 *	BarProgress()
 * .set_progress(25, 100) //25% complete
 * .set_border_thickness(25) //border with 25 of height/2 can be up to 100
 * .set_background_color(0) //color 0 background
 * .set_color(1) //color 1 foreground
 * .draw(attributes + DrawingPoint(0,0) + DrawingArea(1000,100));
 *
 * ```
 *
 */
class BarProgress : public Progress {
public:
	BarProgress();
	void draw_to_scale(const DrawingScaledAttr & attr);

};

}

#endif /* SAPI_DRAW_BARPROGRESS_HPP_ */
