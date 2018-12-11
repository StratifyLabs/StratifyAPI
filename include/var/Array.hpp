#ifndef ARRAY_HPP
#define ARRAY_HPP

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
template <typename T, u32 size_value> class Array : public api::VarWorkObject {
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
	u32 count() const { return size_value; }

	/*! \details Returns the size of the array in bytes.
	  *
	  * Use count() to get the number of objects in the array.
	  *
	  * \code
	  * #include <sapi/var.hpp>
	  * Array<u32, 64> array;
	  * printf("This array uses %ld bytes in memory\n", array.size());
	  * \endcode
	  *
	  */
	u32 size() const { return count() * sizeof(T); }

	/*! \details Returns a reference to the first object in the array. */
	T & front(){ return m_array[0]; }
	/*! \details Returns a reference to first object in the array (read-only). */
	const T & front() const { return m_array[0]; }

	/*! \details Returns a reference last object in the array. */
	T & back(){
		if( size_value ){
			return m_array[size_value-1];
		} else {
			return m_array[0];
		}
	}
	/*! \details Returns a reference to the last object in the array (read-only). */
	const T & back() const {
		if( size_value ){
			return m_array[size_value-1];
		} else {
			return m_array[0];
		}
	}

	/*! \details Fills the array with the specified \a value. */
	void fill(const T & value){
		u32 i;
		for(i=0; i < size_value; i++){
			m_array[i] = value;
		}
	}

	/*! \details Returns a read-only item of the index value specified.
	  *
	  * @param idx The object index
	  *
	  * If \a idx exceeds the size of the array, ApiObject::exit_fatal()
	  * is called.
	  *
	  *
	  */
	const T & at(u32 idx) const {
		if( idx >= size_value ){
			exit_fatal("Array out of bounds");
		}
		return m_array[idx];
	}

	/*! \details Returns an item of the index value specified.
	  *
	  * @param idx The object index
	  *
	  * If the idx exceeds the size of the array, SObject::exit_fatal()
	  * is called.
	  *
	  */
	T & at(u32 idx){
		if( idx >= size_value ){
			exit_fatal("Array out of bounds");
		}
		return m_array[idx];
	}

	/*! \details Returns a pointer to the data. */
	T * data(){ return m_array; }

	/*! \details Returns a read-only pointer to the data. */
	const T * data() const { return m_array; }

private:
	T m_array[size_value];
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

#endif // ARRAY_HPP
