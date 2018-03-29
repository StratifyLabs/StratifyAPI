#ifndef QUEUE_HPP
#define QUEUE_HPP


#include "LinkedList.hpp"

namespace var {

/*! \brief Queue Class
 * \details The Queue class is a FIFO data structure
 * that allows data to be pushed on the back
 * and popped from the front. It is similar to the
 * std::queue container class.
 *
 */
template<typename T> class Queue : public LinkedList {
public:

    Queue() : LinkedList(sizeof(T)*jump_size()){
        m_front_idx = 0;
        m_back_idx = 0;
    }

    T & back(){
        T * back_item = (T*)LinkedList::back();
        return back_item[m_back_idx];
    }

    const T & back() const {
        const T * back_item = (const T*)LinkedList::back();
        return back_item[m_back_idx];
    }

    const T & front() const {
        const T * front_item = (const T*)LinkedList::front();
        return front_item[m_front_idx];
    }

    T & front(){
        T * front_item = (T*)LinkedList::front();
        return front_item[m_front_idx];
    }

    void push(const T & value){
        if( m_back_idx == jump_size() ){
            if( push_back() == false ){
                return;
            }
            m_back_idx = 0;
        }
        back() = value;
        m_back_idx++;
    }

    void pop(){
        if( m_front_idx == jump_size() ){
            pop_front();
            m_front_idx = 0;
        }
        m_front_idx++;
    }

private:
    u16 m_front_idx;
    u16 m_back_idx;

    static int jump_size(){ return 16; }



};

}
#endif // QUEUE_HPP
