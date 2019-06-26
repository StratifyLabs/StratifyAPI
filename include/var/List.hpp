#ifndef SAPI_VAR_LIST_HPP_
#define SAPI_VAR_LIST_HPP_

#include "Queue.hpp"

/*! \cond */

#if 0
namespace var {

//this shouldn't be documented until it is implemented and tested
template <class T> class List : public Queue {
public:
	List();

	//insert
	//erase
	//remove //remove if equal to another value
	//remove_if (uses Predicates)
	//sort
	//unique
	//resize
	//clear

	//push_front
	//push_back -- same as Queue::push
	/*! \details Pushes an item on the back of the list.
	  *
	  * @param value The item to copy to the list
	  * @return Zero on success
	  */
	int push_back(const T & value){
		return push(value);
	}
	//pop_front

	/*! \details Pops the first item in the list. */
	void pop_front(){ pop(); }

	//pop_back


};

}
#endif

/*! \endcond */


#endif // SAPI_VAR_LIST_HPP_
