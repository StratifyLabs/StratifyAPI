#ifndef SAPI_VAR_QUEUE_HPP_
#define SAPI_VAR_QUEUE_HPP_

#include <new>
#include <cstdio>
#include "LinkedList.hpp"
#include "../api/VarObject.hpp"

namespace var {

/*! \brief Queue Class
 * \details The Queue class is a FIFO data structure
 * that allows data to be pushed on the back
 * and popped from the front. It is similar to the
 * std::queue container class.
 *
 */
template<typename T> class Queue : public api::WorkObject {
public:

	/*! \details Constructs a new Queue. */
	Queue() : m_linked_list(sizeof(T)*jump_size()) {
		set_initial_values();
	}

	~Queue(){
		clear();
	}

	Queue(const Queue & a) : m_linked_list(sizeof(T)*jump_size()){
		set_initial_values();
		copy_object(a);
	}

	Queue & operator=(const Queue & a){ copy_object(a); }
	Queue(Queue && a){ move_object(a); }
	Queue & operator=(Queue && a){ move_object(a); }

	/*! \details Returns a reference to the back item.
	  *
	  * The back item is the one that has most recently
	  * been pushed using push().
	  *
	  */
	T & back(){
		T * back_item = (T*)m_linked_list.back();
		//fatal error if back_item is null
		if( back_item == 0 ){
			exit_fatal("Queue::back()");
		}
		return back_item[m_back_idx];
	}

	/*! \details Returns a read-only reference to the back item.
	  *
	  * The back item is the one that has most recently
	  * been pushed using push().
	  *
	  */
	const T & back() const {
		const T * back_item = (const T*)m_linked_list.back();
		//fatal error if back_item is null
		if( back_item == 0 ){
			exit_fatal("Queue::back()");
		}
		return back_item[m_back_idx];
	}

	/*! \details Returns a read-only reference to the front item.
	  *
	  * The front item is the one that has been in
	  * the queue the longest. It will be popped
	  * on the next call to pop().
	  */
	const T & front() const {
		const T * front_item = (const T*)m_linked_list.front();
		//fatal error if front_item is null
		if( front_item == 0 ){
			exit_fatal("Queue::front()");
		}
		return front_item[m_front_idx];
	}

	/*! \details Returns a reference to the front item.
	  *
	  * The front item is the one that has been in
	  * the queue the longest. It will be popped
	  * on the next call to pop().
	  */
	T & front(){
		T * front_item = (T*)m_linked_list.front();
		//fatal error if front_item is null
		if( front_item == 0 ){
			exit_fatal("Queue::front()");
		}
		return front_item[m_front_idx];
	}

	/*! \details Pushes an item on the back of the queue.
	  *
	  * @param value The item to push
	  *
	  */
	int push(const T & value){
		m_back_idx++;
		if( m_back_idx == jump_size() ){
			if( m_linked_list.push_back() < 0 ){
				return -1;
			}
			m_back_idx = 0;
		}
		//execute the copy construtor in place (at memory location back())
		void * new_back = &back();
		new(new_back) T(value);
		return 0;
	}

	/*! \details Pops an item from the front of the queue. */
	void pop(){
		if( m_linked_list.is_empty() ){
			return;
		}

		//execute the descructor explicitly
		front().~T();

		//if the last item was popped -- clear the list (ie make empty)
		if( (m_linked_list.front() == m_linked_list.back())
			 && (m_front_idx == m_back_idx) ){
			m_linked_list.clear();
			set_initial_values();
		} else {
			m_front_idx++;

			//if all items in the front array have been popped free it
			if( m_front_idx == jump_size() ){
				m_linked_list.pop_front();
				m_front_idx = 0;
			}
		}
	}


	/*! \details Returns true if the queue is empty. */
	bool is_empty() const { return m_linked_list.is_empty(); }

	/*! \details Returns the number of items in the queue. */
	u32 count() const {
		int linked_count = m_linked_list.count();
		if( linked_count ){

			if( linked_count == 1 ){
				return m_back_idx - m_front_idx + 1;
			}

			return m_back_idx +
					jump_size() - m_front_idx +
					(linked_count-2)*jump_size() +
					1;
		}
		return 0;
	}

	/*! \details Clears the contents of the queue.
	  *
	  * This will empty the queue and free all the
	  * resources associated with it.
	  *
	  */
	void clear(){
		//deconstruct objects in the list using pop
		while( is_empty() == false ){ pop(); }
	}


private:
	u16 m_front_idx;
	u16 m_back_idx;
	LinkedList m_linked_list;

	void set_initial_values(){
		m_front_idx = 0;
		m_back_idx = jump_size()-1;
	}

	static int jump_size(){ return 16; }


	void copy_object(const Queue<T> & a){
		if( this != &a ){
			clear();
			LinkedListIndex idx(a.m_linked_list);
			const T * current;
			u32 count = 0;
			u32 a_count	= a.count();

			//go through each linked list and push items
			while( (current = (const T *)idx.current()) != 0 ){
				for(u32 i=0; i < jump_size() && count < a_count; i++){
					push(current[i]);
					count++;
				}
				idx.increment();
			}
		}
	}

	void move_object(Queue<T> & a){
		if( this != &a ){
			m_linked_list.swap(a.m_linked_list);
			m_front_idx = a.m_front_idx;
			m_back_idx = a.m_back_idx;
			a.m_front_idx = 0;
			a.m_back_idx = 0;
		}
	}

};

}
#endif // SAPI_VAR_QUEUE_HPP_
