#include "ux/EventLoop.hpp"

using namespace ux;

EventLoop::EventLoop()
{

}


int EventLoop::start(Scene & start_scene){
   m_active_scene = &start_scene;
   return 0;
}

int EventLoop::loop(){
   do {
      process_events();
   } while( m_active_scene != nullptr );
   return 0;
}

void EventLoop::handle_event(const Event & event){
   Scene * next_scene;

   if( m_active_scene ){
      next_scene = m_active_scene->handle_event(event);
      if( next_scene != m_active_scene ){
         m_active_scene->handle_event(
                  SystemEvent(SystemEvent::id_update)
                  );
         m_timer.restart();
         m_active_scene = next_scene;
      }
   }

}
