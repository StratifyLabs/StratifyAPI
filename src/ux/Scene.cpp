#include "ux/Scene.hpp"


using namespace ux;

Scene::Scene(){

}

Scene * Scene::handle_event(const Event & event){
   for(auto component: m_component_list){
      //pass events to each component
      component->handle_event(event);
   }

   //change to another scene
   return this;
}

