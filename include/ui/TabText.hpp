/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef UI_TABTEXT_HPP_
#define UI_TABTEXT_HPP_

#include "TextTab.hpp"

namespace ui {

#if !defined __link
[[deprecated("Use TextTab")]]
#endif
typedef TextTab TabText;
}

#endif /* UI_TABTEXT_HPP_ */
