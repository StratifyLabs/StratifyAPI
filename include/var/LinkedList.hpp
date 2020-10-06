/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef VAR_API_LINKEDLIST_HPP_
#define VAR_API_LINKEDLIST_HPP_

#include <cstdlib>
#include "../api/VarObject.hpp"

namespace var {

class LinkedListIndex;

/*! \brief Linked List Class
 * \details The Linked List provides
 * a class that manages dynamically allocated
 * linked lists of data.
 */
class LinkedList : public api::WorkObject {
public:
	LinkedList(u32 size);
	~LinkedList();

	LinkedList(const LinkedList & list);
	LinkedList & operator=(const LinkedList & list);
	LinkedList(LinkedList && list);
	LinkedList & operator=(LinkedList && list);

	/*! \details Returns a pointer to the data
	 * stored in the back list element.
	 */
	void * back() const { return data(m_back); }

	/*! \details Returns a pointer to the data
	 * stored in the front element.
	 */
	void * front() const { return data(m_front); }

	/*! \details Allocates a new item at the back
	 * of the list.
	 *
	 * @return Less than zero if push fails
	 *
	 */
	int push_back();

	/*! \details Deletes an item at the back of
	 * the list.
	 */
	void pop_back();

	/*! \details Pushes an item on the front of the
	 * list.
	 */
	bool push_front();

	/*! \details Deletes an item from the front of
	 * the list.
	 */
	void pop_front();

	/*! \details Clears the list.
	 *
	 * A call to is_empty() will return true after
	 * calling clear().
	 *
	 */
	void clear();

	/*! \details Counts the number if items in
	 * the list.
	 */
	u32 count() const;

	/*! \details Returns true if the list is empty. */
	bool is_empty() const { return (m_front == 0); }

	/*! \cond */
	void swap(LinkedList & list);
	/*! \endcond */

protected:
private:

	/*! \cond */
	void assign(const LinkedList & list);
	friend class LinkedListIndex;
	typedef struct {
		void * previous;
		void * next;
	} item_t;

	u16 m_list_size;
	item_t * m_front;
	item_t * m_back;

	static void * data(const item_t * item){
		if( item ){
			return (void*)(item + 1);
		}
		return 0;
	}

	static item_t * next(const item_t * item){
		if( item ){ return (item_t*)item->next; }
		return 0;
	}

	static item_t * previous(const item_t * item){
		if( item ){ return (item_t*)item->previous; }
		return 0;
	}

	item_t * new_item(){
		return (item_t*)malloc(calc_item_size());
	}

	u16 calc_item_size() const { return sizeof(item_t) + m_list_size; }
	/*! \endcond */

};

class LinkedListIndex {
public:
	LinkedListIndex(const LinkedList & list){
		m_current_item = 0;
		m_list = &list;
		set_front();
	}

	const LinkedList & linked_list() const { return *m_list; }
	const LinkedList & list() const { return linked_list(); }

	void * current() const {
		if( m_current_item ){
			return LinkedList::data(m_current_item);
		} else {
			return 0;
		}
	}

	LinkedListIndex & operator=(const LinkedList & a){
		m_list = &a;
		set_front();
		return *this;
	}

	LinkedListIndex & operator++(){ increment(); return *this; }
	LinkedListIndex & operator--(){ decrement(); return *this; }


	void set_front(){ m_current_item = linked_list().m_front; }
	void set_back(){ m_current_item = linked_list().m_back; }

	void increment(){
		m_current_item = LinkedList::next(m_current_item);
	}

	void decrement(){
		m_current_item = LinkedList::previous(m_current_item);
	}

	void * next() const {
		return LinkedList::data( LinkedList::next(m_current_item) );
	}

	void * previous() const {
		return LinkedList::data( LinkedList::previous(m_current_item) );
	}

private:

	void check_current_item(){
		if( m_current_item == 0 ){
			m_current_item = linked_list().m_front;
		}
	}

	const LinkedList * m_list;
	LinkedList::item_t * m_current_item;
};

}

#endif // VAR_API_LINKEDLIST_HPP_
