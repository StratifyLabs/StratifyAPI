#ifndef SAPI_UX_EVENTLOOP_HPP
#define SAPI_UX_EVENTLOOP_HPP

#include "Scene.hpp"
#include "../chrono/Timer.hpp"

namespace ux {

class EventLoop {
public:
   EventLoop();

   int loop(
         SceneCollection & scene_collection
         );

   const chrono::Timer & timer(){
      return m_timer;
   }

   void handle_event(const Event & event);
   /*! \details Process events should be implemented
    * to call handle_event() for each
    * event in the system that happens.
    *
    */
   virtual void process_events() = 0;

private:
   chrono::Timer m_timer;
   chrono::Timer m_update_timer;
   SceneCollection * m_scene_collection;
   Scene * m_current_scene;

   void check_for_scene_change();
   void process_update_event();

};

}

#endif // SAPI_UX_EVENTLOOP_HPP
