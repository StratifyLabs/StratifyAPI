#ifndef SAPI_UX_LIST_HPP
#define SAPI_UX_LIST_HPP

#include "Component.hpp"

namespace ux {

class ListItem;
class List;

class ListEvent : public EventObject<EVENT_TYPE('_','l','s','t')> {
public:

   ListEvent(
            const ListItem & item
            ) :
      EventObject<EVENT_TYPE('_','l','s','t')>(0),
      m_list_item(item){

   }

   const ListItem & item() const {
      return m_list_item;
   }

private:
   const ListItem & m_list_item;

};


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

   const sgfx::Region region() const {
      return m_region;
   }

   sgfx::Region region(){
      return m_region;
   }

private:
   friend class List;
   var::String m_key;
   var::String m_value;
   var::String m_icon;
   enum type m_type;
   const List * m_list = nullptr;
   sgfx::Region m_region;

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
