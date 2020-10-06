/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef VAR_API_QUEUE_HPP_
#define VAR_API_QUEUE_HPP_

#include <new>
#include <cstdio>
#include <deque>
#include "../arg/Argument.hpp"


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
	Queue(){}

	~Queue(){
	}

	/*! \details Returns a reference to the back item.
	  *
	  * The back item is the one that has most recently
	  * been pushed using push().
	  *
	  */
	T & back(){
		return m_deque.back();
	}

	/*! \details Returns a read-only reference to the back item.
	  *
	  * The back item is the one that has most recently
	  * been pushed using push().
	  *
	  */
	const T & back() const {
		return m_deque.back();
	}

	/*! \details Returns a read-only reference to the front item.
	  *
	  * The front item is the one that has been in
	  * the queue the longest. It will be popped
	  * on the next call to pop().
	  */
	const T & front() const {
		return m_deque.front();
	}

	/*! \details Returns a reference to the front item.
	  *
	  * The front item is the one that has been in
	  * the queue the longest. It will be popped
	  * on the next call to pop().
	  */
	T & front(){
		return m_deque.front();
	}

	/*! \details Pushes an item on the queue.
	  *
	  * @param value The item to push
	  *
	  */
	Queue& push(const T & value){
		m_deque.push_back(value);
		return *this;
	}

	/*! \details Pops an item from the front of the queue. */
	Queue& pop(){
		m_deque.pop_front();
		return *this;
	}


	/*! \details Returns true if the queue is empty. */
	bool is_empty() const { return m_deque.empty(); }

	/*! \details Returns the number of items in the queue. */
	u32 count() const {
		return m_deque.size();
	}

	/*! \details Clears the contents of the queue.
	  *
	  * This will empty the queue and free all the
	  * resources associated with it.
	  *
	  */
	Queue& clear(){
		//deconstruct objects in the list using pop
		m_deque.clear();
		return *this;
	}


private:
	std::deque<T> m_deque;


};


}
#endif // VAR_API_QUEUE_HPP_
