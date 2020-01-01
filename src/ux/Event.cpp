/* Copyright 2020 tgil All Rights Reserved */

#include "ux/Event.hpp"
using namespace ux;

Event::Event() {
   m_type = 0;
   m_id = 0;
   m_context = nullptr;
}

Event::Event(u32 type, u32 id, void * context){
   m_type = type;
   m_id = id;
   m_context = context;
}
