#ifndef VECTOR_HPP
#define VECTOR_HPP

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
    Vector(){ m_count = 0; }

    /*! \details Constructs a vector with \a count uninitialized items. */
    Vector(int count){
        resize(count);
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
    T & at(u32 pos){
        if( pos < count() ){
            return *(vector_data() + pos);
        }
        return *vector_data();
    }

    /*! \details Provides a read-only reference to an element in the Vector.
     *
     * The same limitations apply to this method as apply to the read-write version.
     *
     */
    const T & at(u32 pos) const {
        if( pos < count() ){
            return *(vector_data_const() + pos);
        }
        return *vector_data_const();
    }

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
            new((void*)(vector_data() + m_count++)) T(value);
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
    int resize(u32 count){
        if( Data::resize(count*sizeof(T)) < 0 ){
            return -1;
        }
        m_count = count;
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
    void reserve(u32 new_capacity){
        if( Data::capacity() < new_capacity*sizeof(T) ){
            Data::resize(new_capacity*sizeof(T));
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
        m_count = 0;
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

    T * vector_data(){ return (T*)data(); }
    const T * vector_data_const() const { return (const T*)data(); }

protected:


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
        if( count() >= capacity() ){
            if( Data::resize((m_count + jump_size()) * sizeof(T)) < 0 ){
                return -1;
            }
        }
        //construct the new items?

        return 0;
    }

    static u32 jump_size(){ return 16; }
    u32 m_count;

};

}

#endif // VECTOR_HPP
