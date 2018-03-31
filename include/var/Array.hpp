#ifndef ARRAY_HPP
#define ARRAY_HPP

#include "../api/SObject.hpp"

namespace var {

template <typename T, u32 size_value> class Array : public api::SObject {
public:
    Array(){}

    int size() const { return size_value; }

    T front(){ return m_array[0]; }
    const T & front() const { return m_array[0]; }

    T back(){ return m_array[size_value-1]; }
    const T & back() const { return m_array[size_value-1]; }


    void fill(const T & value){
        u32 i;
        for(i=0; i < size(); i++){
            m_array[i] = value;
        }
    }

    const T & at(u32 idx) const {
        if( idx < size_value ){
            return m_array[idx];
        }
        //fatal error
        return 0;
    }

    T & at(u32 idx){
        if( idx < size_value ){
            return m_array[idx];
        }
        //fatal error
        return 0;
    }

    T * data(){ return m_array; }
    const T * data() const { return m_array; }

private:
    T m_array[size_value];
};

}

#endif // ARRAY_HPP
