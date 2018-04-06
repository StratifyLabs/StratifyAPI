/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_PROGRESSBAR_HPP_
#define DRAW_PROGRESSBAR_HPP_

#include "BarProgress.hpp"

namespace draw {

#if !defined __link
[[deprecated("Use BarProgress")]]
#endif
typedef BarProgress ProgressBar;

}

#endif /* DRAW_PROGRESSBAR_HPP_ */
