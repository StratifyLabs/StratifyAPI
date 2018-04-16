#ifndef ARRAY_HPP
#define ARRAY_HPP

#include "../api/SObject.hpp"

namespace var {

/*! \brief Array Class
 * \details The Array class is a template
 * used to contain a standard C array.
 *
 * It provides methods to access the array
 * safely.
 *
 */
template <typename T, u32 size_value> class Array : public api::SObject {
public:

    Array(){}

    /*! \details Returns the number of objects in the array. */
    u32 count() const { return size_value; }
    u32 size() const { return size_value; }

    /*! \details Returns the first object in the array. */
    T front(){ return m_array[0]; }
    /*! \details Returns the first object in the array (read-only). */
    const T & front() const { return m_array[0]; }

    /*! \details Returns the last object in the array. */
    T back(){
        if( size_value ){
            return m_array[size_value-1];
        } else {
            return m_array[0];
        }
    }
    /*! \details Returns the last object in the array (read-only). */
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
        for(i=0; i < size(); i++){
            m_array[i] = value;
        }
    }

    /*! \details Returns a read-only item of the index value specified.
     *
     * @param idx The object index
     *
     * If the idx exceeds the size of the array, SObject::exit_fatal()
     * is called.
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

}

#endif // ARRAY_HPP
