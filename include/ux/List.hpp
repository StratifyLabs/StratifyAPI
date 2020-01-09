#ifndef SAPI_UX_LIST_HPP
#define SAPI_UX_LIST_HPP

#include "Component.hpp"
#include "Icon.hpp"

namespace ux {

class ListEvent : public EventObject<EVENT_TYPE('_','l','s','t')> {
public:

   ListEvent(
            const var::String & item_name
            ) :
      EventObject<EVENT_TYPE('_','l','s','t')>(0),
      m_item_name(item_name){

   }

   const var::String & item_name() const {
      return m_item_name;
   }

private:
   const var::String & m_item_name;

};

class ListItem : public Drawing {
public:

   ListItem(List * list);

   enum type {
      type_icon, //just an icon
      type_value, //text value
      type_toggle, //toggle switch
      type_button //button
   };

   ListItem& set_key(const var::String & value){
      m_key = value;
      return *this;
   }

   ListItem& set_value(const var::String & value){
      m_value = value;
      return *this;
   }

   ListItem& set_icon(const var::String & value){
      m_icon = value;
      return *this;
   }

   ListItem& set_type(enum type value){
      m_type = value;
      return *this;
   }

   const var::String & key() const { return m_key; }
   const var::String & value() const { return m_value; }

   void draw_to_scale(const DrawingScaledAttributes & attributes);

   List * list() const;

private:
   var::String m_key;
   var::String m_value;
   var::String m_icon;
   enum type m_type;
};

class List : public Component {
public:

   void draw_to_scale(const DrawingScaledAttributes & attributes);
   void handle_event(const ux::Event & event);

private:


};

}

#endif // SAPI_UX_LIST_HPP
