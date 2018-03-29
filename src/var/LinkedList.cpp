#include "var/LinkedList.hpp"

#include <cstdlib>

using namespace var;

LinkedList::LinkedList(u32 size){
    m_size = size;
    m_front = 0;
    m_back = 0;
}

LinkedList::~LinkedList(){
    clear();
}

u32 LinkedList::count() const {
    u32 count_value = 0;
    item_t * item;

    if( m_front ){
        item = m_front;
        count_value++;
        while( next(item) ){
            item = next(item);
            count_value++;
        }
    }
    return count_value;
}

void LinkedList::clear(){
    item_t * next_item;
    if( m_front ){
        while( (next_item = next(m_front)) != 0 ){
            ::free(m_front);
            m_front = next_item;
        }
        m_front = 0;
        m_back = 0;
    }
}


bool LinkedList::push_back(){
    //the back is the last item
    if( m_back ){
        m_back->next = new_item();
        if( m_back->next ){
            void * old_back = m_back;
            m_back = next(m_back);
            m_back->previous = old_back;
            m_back->next = 0;
            return true;
        }
    } else {
        m_back = new_item();
        if( m_back ){
            m_back->previous = 0;
            m_back->next = 0;
            m_front = m_back;
            return true;
        }
    }
    return false;
}

void LinkedList::pop_back(){
    //remove an item from the back of the list
    item_t * previous_item;
    previous_item = previous(m_back);
    if( previous_item ){
        m_back = previous_item;
        ::free(m_back->next);
        m_back->next = 0;
    } else if( m_back ){
        //current item is the only item
        ::free(m_back);
        m_back = 0;
        m_front = 0;
    }
}


void LinkedList::pop_front(){
    item_t * next_item = next(m_front);
    if( next_item ){
        ::free(m_front);
        m_front = next_item;
        m_front->previous = 0;
    } else if( m_front ){
        //current item is the only item
        ::free(m_front);
        m_back = 0;
        m_front = 0;
    }
}


bool LinkedList::push_front(){
    //add an element to the front
    if( m_front ){
        m_front->previous = new_item();
        if( m_front->previous != 0 ){
            previous(m_front)->next = m_front;
            m_front = previous(m_front);
            m_front->previous = 0;
            return true;
        }
    } else {
        //if list has no entries, push_front() is the same as push_back()
        push_back();
    }
    return false;
}

