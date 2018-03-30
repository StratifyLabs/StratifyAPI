#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstdio>
#include "LinkedList.hpp"

namespace var {

/*! \brief Queue Class
 * \details The Queue class is a FIFO data structure
 * that allows data to be pushed on the back
 * and popped from the front. It is similar to the
 * std::queue container class.
 *
 */
template<typename T> class Queue {
public:

    /*! \details Constructs a new Queue. */
    Queue() : m_linked_list(sizeof(T)*jump_size()) {
        set_initial_values();
    }


    /*! \details Returns a reference to the back item.
     *
     * The back item is the one that has most recently
     * been pushed using push().
     *
     */
    T & back(){
        T * back_item = (T*)m_linked_list.back();
        //fatal error if back_item is null
        return back_item[m_back_idx];
    }

    /*! \details Returns a read-only reference to the back item.
     *
     * The back item is the one that has most recently
     * been pushed using push().
     *
     */
    const T & back() const {
        const T * back_item = (const T*)m_linked_list.back();
        //fatal error if back_item is null
        return back_item[m_back_idx];
    }

    /*! \details Returns a read-only reference to the front item.
     *
     * The front item is the one that has been in
     * the queue the longest. It will be popped
     * on the next call to pop().
     */
    const T & front() const {
        const T * front_item = (const T*)m_linked_list.front();
        //fatal error if front_item is null
        return front_item[m_front_idx];
    }

    /*! \details Returns a reference to the front item.
     *
     * The front item is the one that has been in
     * the queue the longest. It will be popped
     * on the next call to pop().
     */
    T & front(){
        T * front_item = (T*)m_linked_list.front();
        //fatal error if front_item is null
        return front_item[m_front_idx];
    }

    /*! \details Pushes an item on the back of the queue.
     *
     * @param value The item to push
     *
     */
    void push(const T & value){
        m_back_idx++;
        if( m_back_idx == jump_size() ){
            if( m_linked_list.push_back() == false ){
                return;
            }
            m_back_idx = 0;
        }
        back() = value;
    }

    /*! \details Pops an item from the front fo the queue. */
    void pop(){
        if( m_linked_list.is_empty() ){
            return;
        }

        //if the last item was popped -- clear the list (ie make empty)
        if( (m_linked_list.front() == m_linked_list.back())
                && (m_front_idx == m_back_idx) ){
            m_linked_list.clear();
            set_initial_values();
        } else {

            m_front_idx++;

            //if all items in the front array have been popped free it
            if( m_front_idx == jump_size() ){
                m_linked_list.pop_front();
                m_front_idx = 0;
            }
        }
    }


    /*! \details Returns true if the queue is empty. */
    bool is_empty() const { return m_linked_list.is_empty(); }

    /*! \details Returns the number of items in the queue. */
    int count() const {
        int linked_count = m_linked_list.count();
        if( linked_count ){

            if( linked_count == 1 ){
                return m_back_idx - m_front_idx + 1;
            }

            return m_back_idx +
                    jump_size() - m_front_idx +
                    (linked_count-2)*jump_size() +
                    1;
        }
        return 0;
    }

    /*! \details Clears the contents of the queue. */
    void clear(){
        m_linked_list.clear();
        set_initial_values();
    }


private:
    u16 m_front_idx;
    u16 m_back_idx;

    void set_initial_values(){
        m_front_idx = 0;
        m_back_idx = jump_size()-1;
    }

    static int jump_size(){ return 16; }

    LinkedList m_linked_list;



};

}
#endif // QUEUE_HPP
