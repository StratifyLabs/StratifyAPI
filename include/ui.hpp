/* Copyright 2016 tgil All Rights Reserved */


#ifndef UI_HPP_
#define UI_HPP_

/*! \brief User Interface Namespace
 * \details The ui namespace is a collection of classes
 * used to build a graphical User Interface on embedded devices.
 *
 * The ui is event driven. Each ui Element needs to
 * provide a method for handling events and drawing
 * the display.
 *
 *
 */
namespace ui {}

#include "draw.hpp"
#include "ui/Event.hpp"
#include "ui/Element.hpp"
#include "ui/ElementLinked.hpp"
#include "ui/List.hpp"
#include "ui/ListAttr.hpp"
#include "ui/ListItem.hpp"
#include "ui/ListItemInfo.hpp"
#include "ui/Menu.hpp"
#include "ui/MenuList.hpp"
#include "ui/Button.hpp"
#include "ui/ButtonPin.hpp"
#include "ui/TabIcon.hpp"
#include "ui/TabTime.hpp"
#include "ui/TabText.hpp"
#include "ui/TabProgress.hpp"
#include "ui/TabBar.hpp"
#include "ui/Tab.hpp"

using namespace ui;




#endif /* UI_HPP_ */
