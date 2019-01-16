/* Copyright 2016 tgil All Rights Reserved */


#ifndef SAPI_UI_HPP_
#define SAPI_UI_HPP_

/*! \brief User Interface
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
#include "ui/EventLoop.hpp"
#include "ui/Element.hpp"
#include "ui/LinkedElement.hpp"
#include "ui/List.hpp"
#include "ui/ListAttr.hpp"
#include "ui/ListItem.hpp"
#include "ui/ListItemInfo.hpp"
#include "ui/Menu.hpp"
#include "ui/MenuList.hpp"
#include "ui/IconTab.hpp"
#include "ui/TimeTab.hpp"
#include "ui/TextTab.hpp"
#include "ui/ProgressTab.hpp"
#include "ui/TabBar.hpp"
#include "ui/Tab.hpp"


using namespace ui;




#endif /* SAPI_UI_HPP_ */
