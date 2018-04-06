/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef UI_TABTIME_HPP_
#define UI_TABTIME_HPP_

#include "TimeTab.hpp"

namespace ui {

#if !defined __link
[[deprecated("Use TimeTab")]]
#endif
typedef TimeTab TabTime;

}

#endif /* UI_TABTIME_HPP_ */
