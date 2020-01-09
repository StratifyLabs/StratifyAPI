#ifndef SAPI_UX_LIST_HPP
#define SAPI_UX_LIST_HPP

#include "Component.hpp"

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

class List;

class ListItem : public Drawing {
public:


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

   const List * list() const {
      return m_list;
   }

private:
   friend class List;
   var::String m_key;
   var::String m_value;
   var::String m_icon;
   enum type m_type;
   const List * m_list = nullptr;

};

class List : public Component {
public:

   List& set_item_height(drawing_size_t value){
      m_item_height = value;
      return *this;
   }

   List& append(const ListItem & list_item){
      m_items.push_back(list_item);
      return *this;
   }

   void draw_to_scale(const DrawingScaledAttributes & attributes);
   void handle_event(const ux::Event & event);

   u8 border_size() const {
      return m_border_size;
   }

private:
   u8 m_border_size = 1;
   drawing_size_t m_item_height = 25;
   var::Vector<ListItem> m_items;


};

}

#endif // SAPI_UX_LIST_HPP
