#ifndef VECTOR_HPP
#define VECTOR_HPP

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
    Vector(){
        m_count = 0;
    }

    /*! \details Returns a referece to the element
     * at the specified position.
     *
     * @param pos The position in the vector to access.
     *
     * If \a pos is not a valid value, reference to
     * the first eleemnt will be returned.
     */
    T & at(u32 pos){
        if( pos < count() ){
            return *(vector_data() + pos);
        }
        return *vector_data();
    }

    /*! \details Provides read only access to an element in the Vector. */
    const T & at(u32 pos) const {
        return at(pos);
    }

    /*! \details Provides un-bounded access to the specified element. */
    T & operator[](u32 idx) { return vector_data()[idx]; }
    /*! \details Provides un-bounded access to the specified element. */
    const T & operator[](u32 idx) const { return vector_data()[idx]; }

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
    void push_back(const T & value){
        if( add_space() ){
            vector_data()[m_count++] = value;
        }
    }

    /*! \details Removes the last element of the vector. */
    void pop_back(){
        m_count--;
    }

    /*! \details Frees unused memory that is reserved for this Vector. */
    void shrink_to_fit(){
        resize(m_count*sizeof(T));
    }

    /*! \details Removes all elements from the vector. */
    void clear(){ m_count = 0; }

    /*! \details Inserts an element at the specified position.
     *
     * @param pos The position to insert the object.
     * @param value The element to insert
     *
     * If \a pos is at or past the end, \a value
     * will be push_back() is used to add the element.
     *
     */
    void insert(u32 pos, const T & value){
        if( pos >= count() ){
            push_back(value);
        } else if( add_space() ){
            //move elements from pos to end back one
            for(u32 i=count(); i > pos; i-- ){
                vector_data()[i] = vector_data()[i-1];
            }
            vector_data()[pos] = value;
        }
    }

    /*! \details Returns the number of elemens in the Vector. */
    u32 count() const { return m_count; }

    /*! \details Returns the number of bytes used by the Vector.
     *
     * This accounts only for valid objects. Reserved space is not
     * counted.
     *
     */
    u32 size() const { return count()*sizeof(T); }

    /*! \details Reserves enoough data for the specified number of
     * elements.
     *
     * @param new_cap The number of elements to make room for.
     *
     */
    void reserve(u32 new_cap){
        if( Data::capacity() < new_cap*sizeof(T) ){
            resize(new_cap*sizeof(T));
        }
    }

private:

    bool add_space(){
        if( count() >= capacity() ){
            if( resize((m_count + jump_size()) * sizeof(T)) < 0 ){
                return false;
            }
        }
        return true;
    }

    T * vector_data(){
        return (T*)data();
    }

    static u32 jump_size(){ return 16; }
    u32 m_count;

};

}

#endif // VECTOR_HPP
