#include "ux/Scene.hpp"


using namespace ux;


Scene::~Scene(){
   for(auto component_pointer: m_component_list){
      //pass events to each component
      delete component_pointer;
   }
}

Scene& Scene::enable(){
   for(auto component: m_component_list){
      component->enable(scene_collection()->display());
   }
   return *this;
}

Scene& Scene::disable(){
   for(auto component: m_component_list){
      component->disable();
   }
   return *this;
}

void Scene::handle_event(const Event & event){

   if( event.type() == SystemEvent::event_type() ){
      if( event.id() == SystemEvent::id_enter ){
         for(auto component_pointer: m_component_list){
            component_pointer->enable(scene_collection()->display());
         }
      }
   }

   for(auto component_pointer: m_component_list){
      //pass events to each component
      if( component_pointer->is_enabled() ){
         component_pointer->handle_event(event);
      }
   }

   if( event.type() == SystemEvent::event_type() ){
      if( event.id() == SystemEvent::id_exit ){
         for(auto component_pointer: m_component_list){
            component_pointer->disable();
         }
      }
   }

}


Component * Scene::find_component(
      const var::String & name
      ){
   for(auto component_pointer: m_component_list){
      //pass events to each component
      if( component_pointer->name() == name ){
         return component_pointer;
      }
   }
   return nullptr;
}


SceneCollection::~SceneCollection(){
   for(auto scene_pointer: m_scene_list){
      delete scene_pointer;
   }
}

Scene * SceneCollection::find_scene(const var::String & name){
   for(auto scene_pointer: m_scene_list){
      if( scene_pointer->name() == name ){
         return scene_pointer;
      }
   }
   return nullptr;
}

