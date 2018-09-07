#include <cstring>
#include <cstdlib>
#include <cstdio>

#include "var/LinkedList.hpp"

using namespace var;

LinkedList::LinkedList(u32 size){
	m_size = size;
	m_front = 0;
	m_back = 0;
}

LinkedList::~LinkedList(){
	clear();
}

void LinkedList::swap(LinkedList & list){
	printf("Swap lists\n");
	u16 size;
	item_t * front;
	item_t * back;
	front = this->m_front;
	back = this->m_back;
	size = this->m_size;
	this->m_front = list.m_front;
	this->m_back = list.m_back;
	this->m_size = list.m_size;
	list.m_front = front;
	list.m_back = back;
	list.m_size = size;
}

LinkedList::LinkedList(const LinkedList & list){
	assign(list);
}

LinkedList & LinkedList::operator=(const LinkedList & list){
	assign(list);
	return *this;
}

LinkedList::LinkedList(LinkedList && list){
	swap(list);
}

LinkedList & LinkedList::operator=(LinkedList && list){
	swap(list);
	return *this;
}

void LinkedList::assign(const LinkedList & list){
	clear();
	m_size = list.m_size;
	item_t * next_item;
	if( list.m_front ){
		next_item = list.m_front;
		do {
			if( push_back() ){
				memcpy(back(), data(next_item), m_size);
				next_item = next(next_item);
			} else {
				next_item = 0;
			}
		} while( next_item != 0 );
	}
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
		do {
			next_item = next(m_front);
			::free(m_front);
			m_front = next_item;
		} while( m_front );
		m_front = 0;
		m_back = 0;
	}
	m_size = 0;
}


int LinkedList::push_back(){
	//the back is the last item
	if( m_back ){
		m_back->next = new_item();
		if( m_back->next ){
			void * old_back = m_back;
			m_back = next(m_back);
			m_back->previous = old_back;
			m_back->next = 0;
			return 0;
		}
	} else {
		m_back = new_item();
		if( m_back ){
			m_back->previous = 0;
			m_back->next = 0;
			m_front = m_back;
			return 0;
		}
	}
	return -1;
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

