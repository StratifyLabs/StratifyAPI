/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_VAR_LIST_HPP_
#define SAPI_VAR_LIST_HPP_

#include <list>
#include "Queue.hpp"

/*! \cond */

namespace var {

//this shouldn't be documented until it is implemented and tested
template <class T> class List : public api::WorkObject {
public:
	List();



private:
	std::list<T> m_list;


};

}

/*! \endcond */


#endif // SAPI_VAR_LIST_HPP_
