#ifndef SAPI_UX_SCENE_HPP
#define SAPI_UX_SCENE_HPP

#include "../api/WorkObject.hpp"
#include "../var/Vector.hpp"
#include "../hal/Display.hpp"
#include "../sgfx/Theme.hpp"

#include "Component.hpp"

namespace ux {

class SceneCollection;

class Scene : api::WorkObject {
public:
   virtual ~Scene();

   Scene& add_component(
         const var::String & name,
         Component* component
         ){
      component->set_scene(this);
      component->set_name(name);
      m_component_list.push_back(component);
      return *this;
   }

   Scene& add_background_component(Component* component){
      component->set_scene(this);
      component->set_name("");
      m_background_component_list.push_back(component);
      return *this;
   }

   Scene& remove_component(const var::String & name){
      //delete and remove
      return *this;
   }

   template <typename T> T * find_component(const var::String & name){
      return static_cast<T*>(find_generic_component(name));
   }

   Scene& enable();
   Scene& disable();

   virtual void draw_background(
         const DrawingPoint & point = DrawingPoint(0,0),
         const DrawingArea & area = DrawingArea(1000,1000)
         );

   const var::String & name(){
      return m_name;
   }

   SceneCollection * scene_collection(){
      return m_scene_collection;
   }

   const SceneCollection * scene_collection() const {
      return m_scene_collection;
   }

   virtual void handle_event(const Event & event);
   void trigger_event(const Event & event);

   void process_event(const Event & event);

   chrono::MicroTime & update_period(){
      return m_update_period;
   }

   const chrono::MicroTime & update_period() const {
      return m_update_period;
   }

private:
   friend class SceneCollection;
   var::String m_name;
   chrono::MicroTime m_update_period = chrono::Milliseconds(30);
   SceneCollection * m_scene_collection = nullptr;
   var::Vector<Component*> m_component_list;
   var::Vector<Component*> m_background_component_list;
   Component * find_generic_component(const var::String & name);


};

class SceneCollection : public api::WorkObject {
public:
   SceneCollection(hal::Display & display) : m_display(display){}
   ~SceneCollection();

   SceneCollection& add_scene(const var::String & name, Scene * scene){
      scene->m_name = name;
      scene->m_scene_collection = this;
      m_scene_list.push_back(scene);
      return *this;
   }

   SceneCollection& remove_scene(const var::String & name){
      //find scene by name
      return *this;
   }

   Scene * find_scene(const var::String & name);
   void trigger_event(const Event & event);


   SceneCollection& set_theme(const sgfx::Theme & theme){
      m_theme = &theme;
      return *this;
   }

   const sgfx::Theme & theme() const {
      return (*m_theme);
   }

   bool set_current_scene(const var::String & name){
      if( name.is_empty() ){
         m_current_scene = nullptr;
         return true;
      }

      Scene * next_scene = find_scene(name);
      if( next_scene == nullptr ){
         return false;
      }
      m_current_scene = next_scene;
      return true;
   }

   Scene * current_scene() const {
      return m_current_scene;
   }

   hal::Display & display(){
      return m_display;
   }

private:
   hal::Display & m_display;
   var::Vector<Scene*> m_scene_list;
   Scene * m_current_scene = nullptr;
   const sgfx::Theme * m_theme = nullptr;

};


}

#endif // SAPI_UX_SCENE_HPP
