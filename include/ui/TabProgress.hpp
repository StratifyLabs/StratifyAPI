/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef UI_TABPROGRESS_HPP_
#define UI_TABPROGRESS_HPP_

#include "ProgressTab.hpp"

namespace ui {


#if !defined __link
[[deprecated("Use ProgressTab")]]
#endif
typedef ProgressTab ProgressTab;

#if !defined __link
[[deprecated("Use ArcProgressTab")]]
#endif
typedef ArcProgressTab TabProgressArc;

#if !defined __link
[[deprecated("Use CircleProgressTab")]]
#endif
typedef CircleProgressTab TabProgressCircle;

}


#endif /* UI_TAB_TABPROGRESS_HPP_ */
