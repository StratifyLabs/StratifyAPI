#ifndef SAPI_VAR_VECTOR_HPP_
#define SAPI_VAR_VECTOR_HPP_

#include <vector>
#include "../arg/Argument.hpp"

namespace var {

/*! \brief Vector Class
 * \details The Vector class
 * is similar to std::vector but is embedded friendly.
 *
 *
 */
template<typename T> class Vector:
      public api::WorkObject {
public:


	/*! \details Constructs an empty object.
	  *
	  */
	Vector(){}

	/*! \details Constructs a vector with \a count uninitialized items. */
	Vector(size_t count){
		m_vector.resize(count);
	}

	~Vector(){}

	Vector<T> & operator << (const T & a){
		return push_back(a);
	}

	typename std::vector<T>::const_iterator begin() const noexcept { return m_vector.begin(); }
	typename std::vector<T>::iterator begin() noexcept { return m_vector.begin(); }

	typename std::vector<T>::const_iterator end() const noexcept { return m_vector.end(); }
	typename std::vector<T>::iterator end() noexcept { return m_vector.end(); }

	typename std::vector<T>::const_iterator cbegin() const noexcept { return m_vector.cbegin(); }
	typename std::vector<T>::const_iterator cend() const noexcept { return m_vector.cend(); }

	typename std::vector<T>::const_reverse_iterator rbegin() const noexcept { return m_vector.rbegin(); }
	typename std::vector<T>::reverse_iterator rbegin() noexcept { return m_vector.rbegin(); }

	typename std::vector<T>::const_reverse_iterator rend() const noexcept { return m_vector.rend(); }
	typename std::vector<T>::reverse_iterator rend() noexcept { return m_vector.rend(); }

	typename std::vector<T>::const_reverse_iterator crbegin() const noexcept { return m_vector.crbegin(); }
	typename std::vector<T>::const_reverse_iterator crend() const noexcept { return m_vector.crend(); }


	Vector<T> & operator << (const Vector<T> & a){
		for(u32 i=0; i < a.count(); i++){
			*this << a.at(i);
		}
		return *this;
	}

	T & at(u32 position){ return m_vector.at(position); }
	const T & at(u32 position) const { return m_vector.at(position); }

	Vector<T> & push_back(const T & a ){
		m_vector.push_back(a);
		return *this;
	}

	Vector<T> & pop_back(){
		m_vector.pop_back();
		return *this;
	}

	/*! \details Finds an object in the array.
	  *
	  * @param a The equivalent object to find
	  * @return The index of the object or count() it if wasn't found
	  *
	  */
	u32 find(const T & a){
		for(u32 i=0; i < count(); i++){
			if( this->at(i) == a ){
				return i;
			}
		}
		return count();
	}

	u32 find(
			const T & a,
			bool (*compare)(const T & a, const T & b)
			){
		for(u32 i=0; i < count(); i++){
			if( compare(this->at(i), a) ){
				return i;
			}
		}
		return count();
	}

	T * search(const T & a){
		return (T*)bsearch(&a, std::vector<T>::data(), count(), sizeof(T), ascending);
	}

	T * search(
			const T & a,
			int (*compare)(const void *, const void *)
			){
		return (T*)bsearch(
					&a,
					std::vector<T>::data(),
					count(),
					sizeof(T),
					compare
					);
	}

	/*!
	  * \details Fills the vector will the specified value.
	  * \param value The value to assign to each element of the vector
	  */
	void fill(const T & value){
		for(auto & element: m_vector){
			element = value;
		}
	}

	int resize(size_t count){
		m_vector.resize(count);
		return 0;
	}


	int free(){
		m_vector.clear();
		return 0;
	}

	static int ascending(const void * a, const void * b){
		const T * object_a = (const T*)a;
		const T * object_b = (const T*)b;
		if( *object_a < *object_b ){ return -1; }
		if( *object_a > *object_b ){ return 1; }
		return 0;
	}

	static int descending(const void * a, const void * b){
		const T * object_a = (const T*)a;
		const T * object_b = (const T*)b;
		if( *object_a < *object_b ){ return 1; }
		if( *object_a > *object_b ){ return -1; }
		return 0;
	}

	typedef int (*sort_compartor_t)(const void*, const void *);

	void sort(sort_compartor_t compare_function){
		qsort(m_vector.data(),
				count(),
				sizeof(T),
				compare_function
				);
	}

	size_t size() const {
		return count() * sizeof(T);
	}

	/*! \details Returns the number of elemens in the Vector.
	  *
	  * The count includes the number of elements added to the vector
	  * using push_back() or insert(). If the resize() method is called,
	  * it will directly set the count() and ensure there is enough
	  * memory available to hold count() items.
	  *
	  *
	  */
	u32 count() const { return m_vector.size(); }

	Vector<T> operator + (const Vector<T> & a) const {
		return operate(a, add);
	}

	Vector<T> operator - (const Vector<T> & a) const {
		return operate(a, subtract);
	}

	Vector<T> operator * (const Vector<T> & a) const {
		return operate(a, multiply);
	}

	Vector<T> operator / (const Vector<T> & a) const {
		return operate(a, divide);
	}

	Vector<T> operator + (const T & a) const {
		return operate_single(a, add);
	}

	Vector<T> operator - (const T & a) const {
		return operate_single(a, subtract);
	}

	Vector<T> operator * (const T & a) const {
		return operate_single(a, multiply);
	}

	Vector<T> operator / (const T & a) const {
		return operate_single(a, divide);
	}

	Vector<T> operator << (u32 a) const {
		Vector<T> result;
		result.resize(count());
		if( a < count() ){
			for(u32 i=0; i < count()-a; i++){
				result.at(i) = at(i+a);
			}
		}
		return result;
	}

	Vector<T> operator >> (u32 a) const {
		Vector<T> result;
		result.resize(count());
		if( a < count() ){
			for(u32 i=0; i < count()-a; i++){
				result.at(i+a) = at(i);
			}
		}
		return result;
	}

	void clear(){
		m_vector.clear();
	}

	std::vector<T> & vector(){ return m_vector; }
	const std::vector<T> & vector() const { return m_vector; }

	const T * data() const { return m_vector.data(); }
	T * data(){ return m_vector.data(); }

	void * to_void(){ return (void*)m_vector.data(); }
	const void * to_const_void() const { return (const void*)m_vector.data(); }

protected:



	Vector<T> operate(const Vector<T> & a, T (*fn)(const T &, const T &)) const {
		Vector<T> result;
		u32 c = a.count() < count() ? a.count() : count();
		for(u32 i=0; i < c; i++){
			result.push_back(fn(this->at(i), a.at(i)));
		}
		return result;
	}

	Vector<T> operate_single(const T & a, T (*fn)(const T &, const T &)) const {
		Vector<T> result;
		u32 c = count();
		for(u32 i=0; i < c; i++){
			result.push_back(fn(this->at(i), a));
		}
		return result;
	}

	static T add(const T & a, const T & b){ return a+b; }
	static T subtract(const T & a, const T & b){ return a-b; }
	static T multiply(const T & a, const T & b){ return a*b; }
	static T divide(const T & a, const T & b){ return a/b; }


	static bool compare(const Vector<T> & a, const Vector<T> & b){
		u32 i;
		if( a.count() != b.count() ){
			return false;
		}
		for(i=0; i < a.count(); i++){
			if( a[i] != b[i] ){
				return false;
			}
		}
		return true;
	}



private:

	std::vector<T> m_vector;

};

}

#endif // SAPI_VAR_VECTOR_HPP_
