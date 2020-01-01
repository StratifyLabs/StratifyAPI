#include "chrono.hpp"
#include "ux/EventLoop.hpp"

using namespace ux;

EventLoop::EventLoop(){

}


int EventLoop::loop(SceneCollection & scene_collection){
   m_scene_collection = &scene_collection;

   m_current_scene = nullptr;
   m_update_timer.restart();

   while(
         m_scene_collection->current_scene()
         ){
      process_events();
      process_update_event();

   }

   return 0;
}

void EventLoop::process_update_event(){
   Scene * current_scene = m_scene_collection->current_scene();

   if( current_scene == nullptr ){
      return;
   }

   MicroTime elapsed =
         Milliseconds(m_update_timer.milliseconds());

   if( elapsed > current_scene->update_period() ){
      current_scene->handle_event(
               SystemEvent(SystemEvent::id_update)
               );
      check_for_scene_change();
      m_update_timer.restart();
   } else {
      u32 remaining_milliseconds =
            current_scene->update_period().milliseconds() - elapsed.milliseconds();
      //this should be an estimate until the next update event
      wait(Milliseconds(remaining_milliseconds));
   }
}

void EventLoop::check_for_scene_change(){
   if( m_current_scene != m_scene_collection->current_scene() ){

      if( m_current_scene != nullptr ){
         m_current_scene->handle_event(
                  SystemEvent(SystemEvent::id_exit)
                  );
      }

      if( m_scene_collection->current_scene() ){
         m_scene_collection->current_scene()->handle_event(
                  SystemEvent(SystemEvent::id_enter)
                  );
      }

      m_current_scene = m_scene_collection->current_scene();

      m_timer.restart();
      m_update_timer.restart();
   }
}


void EventLoop::handle_event(const Event & event){
   if( m_scene_collection->current_scene() ){
      m_scene_collection->current_scene()->handle_event(event);
   }
   check_for_scene_change();
}
