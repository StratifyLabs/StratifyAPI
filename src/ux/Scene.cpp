#include "ux/Scene.hpp"
#include "ux/Rectangle.hpp"

using namespace ux;
using namespace sgfx;

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

void Scene::draw_background(const DrawingPoint & point, const DrawingArea & area){
   hal::Display & display = scene_collection()->display();
   scene_collection()->theme().set_display_palette(
            display,
            Theme::style_brand_primary,
            Theme::state_default
            );
   display.clear();

   for(auto component_pointer: m_background_component_list){
      component_pointer->enable(scene_collection()->display());
      component_pointer->redraw();
   }

   for(auto component_pointer: m_background_component_list){
      if( component_pointer->is_refresh_drawing_pending() ){
         component_pointer->refresh_drawing();
      }
   }

   for(auto component_pointer: m_background_component_list){
      component_pointer->disable();
   }

}

void Scene::handle_event(const Event & event){

   if( event.type() == SystemEvent::event_type() ){
      if( event.id() == SystemEvent::id_enter ){
         printf("Enter scene %s\n", name().cstring());

         printf("Draw background\n");
         draw_background();
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
         printf("Exit scene %s\n", name().cstring());
         for(auto component_pointer: m_component_list){
            component_pointer->disable();
         }
      }
   }

   for(auto component_pointer: m_component_list){
      if( component_pointer->is_refresh_drawing_pending() ){
         component_pointer->refresh_drawing();
      }
   }

}

void Scene::trigger_event(const Event & event){
   //send the event to the scene collection
   handle_event(event);
}



Component * Scene::find_generic_component(
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

void SceneCollection::trigger_event(const Event & event){
   //allow the application to catch the event
}


