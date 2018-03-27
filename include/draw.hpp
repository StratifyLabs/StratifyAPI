/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_HPP_
#define DRAW_HPP_

/*! \brief Drawing Graphics
 *
 * \details The draw namespace contains classes
 * used for drawing and animating objects. The
 * classes are built on the sgfx namespace which provides
 * an API for accessing the Stratify Graphics library.
 *
 * The base object is a Drawing. Classes that inherit
 * objects must implement one of two methods: draw()
 * or draw_to_scale().
 *
 * The draw() method will draw objects on an abstract
 * drawing canvas with a width and height of DrawingAttr::scale()
 * usually 1000.
 *
 * The draw_to_scale() is ultimately called by the draw() method
 * to draw directly on a hal::Display.
 *
 */
namespace draw {}

#include "sgfx.hpp"
#include "draw/Animation.hpp"
#include "draw/Drawing.hpp"
#include "draw/GraphLine.hpp"
#include "draw/GraphBar.hpp"
#include "draw/Icon.hpp"
#include "draw/Image.hpp"
#include "draw/Panel.hpp"
#include "draw/ProgressBar.hpp"
#include "draw/ProgressArc.hpp"
#include "draw/ProgressCircle.hpp"
#include "draw/Rect.hpp"
#include "draw/Text.hpp"
#include "draw/TextBox.hpp"

using namespace draw;


#endif /* DRAW_HPP_ */
