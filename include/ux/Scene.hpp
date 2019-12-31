#ifndef SAPI_UX_SCENE_HPP
#define SAPI_UX_SCENE_HPP

#include "../api/WorkObject.hpp"
#include "../var/Vector.hpp"

#include "Component.hpp"

namespace ux {

class EventLoop;

class Scene : api::WorkObject {
public:
   Scene();
   ~Scene();

   Scene& add_component(Component* component){
      component->set_scene(this);
      m_component_list.push_back(component);
      return *this;
   }

   Scene& remove_component(const var::String & name){
      //delete and remove
      return *this;
   }

   virtual Scene * handle_event(const Event & event);

private:
   var::Vector<Component*> m_component_list;
   volatile bool m_is_running;
   bool m_is_stopped;

};

}

#endif // SAPI_UX_SCENE_HPP
