/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef UI_ELEMENTLINKED_HPP_
#define UI_ELEMENTLINKED_HPP_

#include "LinkedElement.hpp"

namespace ui {

#if !defined __link
[[deprecated("Use LinkedElement")]]
#endif
typedef LinkedElement ElementLinked;

}

#endif /* UI_ELEMENTLINKED_HPP_ */
