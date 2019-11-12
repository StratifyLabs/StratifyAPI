#ifndef SAPI_VAR_VECTOR_HPP_
#define SAPI_VAR_VECTOR_HPP_

#include <iterator>
#include <new>
#include "Data.hpp"

namespace var {

/*! \brief Vector Class
 * \details The Vector class
 * is similar to std::vector but is embedded friendly.
 *
 *
 */
template<typename T> class Vector : public Data {
public:

	/*! \details Constructs an empty object.
	  *
	  */
	Vector(){
		m_count = 0;
	}

	using iterator = std::iterator<std::random_access_iterator_tag,T,ptrdiff_t,T*,T&>;
	using const_iterator = std::iterator<std::random_access_iterator_tag,const T,ptrdiff_t,const T*,const T&>;

	T * begin(){ return to<T>(); }
	const T * begin() const { return to<const T>(); }
	T * end(){
		T * result = to<T>();
		if( result ){
			return result + m_count;
		}
		return nullptr;
	}
	const T * end() const {
		const T * result = to<const T>();
		if( result ){
			return result + m_count;
		}
		return nullptr;
	}




	/*! \details Constructs a vector with \a count uninitialized items. */
	Vector(const arg::Count count){
		m_count = 0;
		resize(count);
	}

	~Vector(){
		//call destructors on objects in array
		clear();
	}

	Vector(const Vector<T> & a){
		m_count = 0;
		copy_object(a);
	}

	Vector(Vector<T> && a){
		move_object(a);
	}

	Vector& operator=(Vector<T> && a){
		move_object(a);
		return *this;
	}

	Vector& operator=(const Vector<T> & a){
		copy_object(a);
		return *this;
	}


	/*! \details Returns a referece to the element
	  * at the specified position.
	  *
	  * @param pos The position in the vector to access.
	  *
	  * If \a pos is not a valid value, reference to
	  * the first eleemnt will be returned.
	  *
	  * If there are zero elements in the vector and
	  * no memory has been allocated, this method will
	  * dereference a null-pointer and cause the application
	  * to crash.
	  *
	  * If there are no elements and the vector has been
	  * allocated memory, this method will return a value
	  * that is no longer valid.
	  *
	  */
	T & at(u32 pos){ return Data::at<T>(pos); }

	/*! \details Provides a read-only reference to an element in the Vector.
	  *
	  * The same limitations apply to this method as apply to the read-write version.
	  *
	  */
	const T & at(u32 pos) const { return Data::at<const T>(pos); }

	/*! \details Provides un-bounded access to the specified element (read-only).
	  *
	  * If the Vector has no data, this method may dereference a null pointer
	  * and cause the program to crash.
	  */
	const T & operator[](u32 idx) const { return vector_data_const()[idx]; }

	/*! \details Provides un-bounded access to the specified element.
	  *
	  * If the Vector has no data, this method may dereference a null pointer
	  * and cause the program to crash.
	  *
	  */
	T & operator[](u32 idx) { return vector_data()[idx]; }

	Vector<T> & operator << (const T & a){
		push_back(a);
		return *this;
	}

	Vector<T> & operator << (const Vector<T> & a){
		for(u32 i=0; i < a.count(); i++){
			*this << a.at(i);
		}
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
			if( at(i) == a ){
				return i;
			}
		}
		return count();
	}

	u32 find(const T & a, bool (*compare)(const T & a, const T & b)){
		for(u32 i=0; i < count(); i++){
			if( compare(at(i), a) ){
				return i;
			}
		}
		return count();
	}

	T * search(const T & a){
		return (T*)bsearch(&a, to_const_void(), count(), sizeof(T), ascending);
	}

	T * search(const T & a, int (*compare)(const void *, const void *)){
		return (T*)bsearch(&a, to_const_void(), count(), sizeof(T), compare);
	}


	/*! \details Returns the number of elements that are
	  * able to fit in the memory that is already allocated.
	  *
	  */
	u32 capacity() const {
		//how many elements can fit in current space
		return Data::capacity() / sizeof(T);
	}

	/*! \details Adds an element to the end of the Vector.
	  *
	  * If the element won't fit, space will be added. If space
	  * cannot be added, nothing will happen.
	  *
	  */
	int push_back(const T & value){
		if( add_space() == 0 ){
			asm volatile("nop");
			new((void*)(vector_data() + m_count++)) T(value);
			asm volatile("nop");
			return 0;
		}
		return -1;
	}

	/*! \details Removes the last element of the vector.
	  *
	  * This makes the last element inaccessible. It does not affect
	  * the memory allocation of the vector. Once items are popped,
	  * the memory can be freed using the method shrink_to_fit().
	  *
	  */
	void pop_back(){
		if( m_count ){
			//call the destructor
			vector_data()[m_count-1].~T();
			m_count--;
		}
	}

	/*! \details Frees unused memory that is reserved for this Vector. */
	void shrink_to_fit(){
		Data::resize(m_count*sizeof(T));
	}

	/*! \details Resizes the vector.
	  *
	  * @param count The new number of element for the vector to have
	  *
	  * This method will increase/decrease the number of elements in
	  * the vector. If elements are added, they will not be initialized.
	  *
	  * The method shrink_to_fit() will free memory that is no longer needed. It doesn't affect
	  * the count() of the object.
	  *
	  * The method reserve() will increase the memory that is available but does
	  * not affect the count() of the object().
	  *
	  */
	int resize(const arg::Count count){
		//this needs to destruct each item that will be lost
		if( count.argument() < this->count() ){
			//size reduction if count < this->count()
			while( count.argument() < this->count() ){
				pop_back();
			}
		} else {
			while( count.argument() > this->count() ){
				push_back(T());
			}
		}
		return 0;
	}

	/*!
	  * \details Fills the vector will the specified value.
	  * \param value The value to assign to each element of the vector
	  */
	virtual void fill(const T & value){
		u32 i;
		for(i=0; i < count(); i++){
			this->at(i) = value;
		}
	}

	/*! \details Reserves enough data for the specified number of
	  * elements.
	  *
	  * @param new_capacity The number of elements to make room for.
	  *
	  */
	void reserve(const arg::ImplicitSize new_capacity){
		if( Data::capacity() < new_capacity.argument()*sizeof(T) ){
			Data::resize(
						new_capacity.argument()*sizeof(T)
						);
		}
	}

	/*! \details Removes all elements from the vector.
	  *
	  * This method sets the count() to zero. It doesn't
	  * free any memory associated with the object.
	  *
	  */
	void clear(){
		//call the destructor on each item
		while( count() ){
			pop_back();
		}
	}

	int free(){
		clear();
		return Data::free();
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
		qsort(to_void(), count(), sizeof(T), compare_function);
	}

	/*! \details Inserts an element at the specified position.
	  *
	  * @param pos The position to insert the object.
	  * @param value The element to insert
	  *
	  * If \a pos is at or past the end, \a value
	  * will be push_back() is used to add the element.
	  *
	  */
	int insert(u32 pos, const T & value){
		//add space
		if( push_back(value) < 0 ){
			return -1;
		}

		if( pos >= count() ){
			//already inserted on the end
			return 0;
		} else if( add_space() == 0 ){
			//move elements from pos to end back one
			for(u32 i=count(); i > pos; i-- ){
				vector_data()[i] = vector_data()[i-1];
			}
			vector_data()[pos] = value;
			return 0;
		}
		return -1;
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
	u32 count() const { return m_count; }

	/*! \details Returns the number of bytes used by the Vector.
	  *
	  * This accounts only for valid objects. Reserved space is not
	  * counted. To return the total number of bytes including reserve,
	  * use capacity() * sizeof(T).
	  *
	  */
	u32 size() const { return count()*sizeof(T); }

	T * vector_data(){ return to<T>(); }
	const T * vector_data_const() const { return to<T>(); }

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
		result.fill(T());
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
		result.fill(T());
		if( a < count() ){
			for(u32 i=0; i < count()-a; i++){
				result.at(i+a) = at(i);
			}
		}
		return result;
	}

protected:



	Vector<T> operate(const Vector<T> & a, T (*fn)(const T &, const T &)) const {
		Vector<T> result;
		u32 c = a.count() < count() ? a.count() : count();
		for(u32 i=0; i < c; i++){
			result.push_back(fn(at(i), a.at(i)));
		}
		return result;
	}

	Vector<T> operate_single(const T & a, T (*fn)(const T &, const T &)) const {
		Vector<T> result;
		u32 c = count();
		for(u32 i=0; i < c; i++){
			result.push_back(fn(at(i), a));
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

	int add_space(){
		if( (capacity() == 0) || (count() >= capacity()-1) ){
			if( Data::resize(
					 ((m_count + jump_size()) * sizeof(T))
					 ) < 0 ){
				return -1;
			}
		}
		return 0;
	}

	void copy_object(const Vector<T> & a){
		if( this != &a ){
			clear();
			for(u32 i=0; i < a.count(); i++){
				push_back(a.at(i));
			}
		}
	}

	void move_object(Vector<T> & a){
		if( this != &a ){
			Data::move_object(a);
			m_count = a.m_count;
			a.m_count = 0;
		}
	}

	static u32 jump_size(){ return 16; }
	u32 m_count;

};

}

#endif // SAPI_VAR_VECTOR_HPP_
