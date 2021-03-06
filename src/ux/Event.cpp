/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
/* Copyright 2020 tgil All Rights Reserved */

#include "ux/Event.hpp"
using namespace ux;

Event::Event() {
   m_type = 0;
   m_id = 0;
	 m_component = nullptr;
}

Event::Event(u32 type, u32 id, Component* component){
   m_type = type;
   m_id = id;
	 m_component = component;
}
