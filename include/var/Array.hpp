#ifndef SAPI_VAR_ARRAY_HPP_
#define SAPI_VAR_ARRAY_HPP_

#include <iterator>
#include <array>
#include <cstdio>
#include "../api/VarObject.hpp"

namespace var {


/*! \brief Array Class
 * \details The Array class is a template
 * used to contain a standard C array.
 *
 * It provides methods to access the array
 * safely.
 *
 * \code
 * #include <sapi/var.hpp>
 * u32 c_array[64]; //C style array
 * Array<u32, 64> array; //var::Array
 *
 * printf("C array is %ld bytes\n", sizeof(c_array));
 * printf("Array is %ld bytes\n", array.size());
 *
 * printf("C array has %ld items\n", sizeof(c_array)/sizeof(u32));
 * printf("Array has %ld items\n", array.count());
 *
 * printf("C array back is %ld\n", c_array[sizeof(c_array)/sizeof(u32)-1]);
 * printf("Array back is %ld\n", array.back());
 * \endcode
 *
 * The Array class has a fixed number of items. It can be
 * created dynamically with `new` and `delete` or allocated
 * on the stack or heap.
 *
 * If you are looking for dynamic sizing, see var::Vector is
 * an array that is dynamically sized where items can be appended.
 *
 * var::Queue allows for pushing items on the front of a list
 * and popping them from the back.
 *
 */
template <typename T, u32 size_value> class Array : public api::InfoObject, public std::array<T, size_value> {
public:

	Array(){}



	/*! \details Returns the number of objects in the array.
	  *
	  * \code
	  * #include <sapi/var.hpp>
	  * Array<u32, 64> array;
	  * for(u32 i=0; i < array.count(); i++){
	  *   array.at(i) = i;
	  * }
	  * \endcode
	  *
	  *
	  */
	u32 count() const { return this->size(); }

	bool is_empty() const { return this->empty(); }


private:

};

template <typename T> class Pair : public Array<T, 2> {
public:

	Pair(){}

	Pair(const T & k, const T & v){
		key() = k;
		value() = v;
	}

	T & key(){ return Array<T,2>::at(0); }
	const T & key() const { return Array<T,2>::at(0); }

	T & value(){ return Array<T,2>::at(1); }
	const T & value() const { return Array<T,2>::at(1); }

};

}

#endif // SAPI_VAR_ARRAY_HPP_
