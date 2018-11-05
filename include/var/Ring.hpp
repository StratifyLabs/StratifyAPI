/*
 * Ring.hpp
 *
 *  Created on: Apr 11, 2017
 *      Author: tgil
 */

#ifndef VAR_RING_HPP_
#define VAR_RING_HPP_

#include <new>
#include <sos/dev/fifo.h>
#include <sys/types.h>
#include "Data.hpp"

namespace var {

/*! \brief Ring Buffer
 * \details Ring is a ring buffer (or circular buffer)
 * that uses a local memory buffer for the data.
 *
 * The Ring can handle items of any type.
 *
 * \code
 *
 * Ring<u32> ring(32); //32 32-bit word ring buffer
 * u32 next = 20;
 * ring.write(&next, 1);
 * ring.read(&next, 1); //read into next variable
 *
 *
 * \endcode
 *
 *
 *
 */
template<typename T> class Ring : public Data {
public:
	/*! \details Constructs a new ring buffer.
	 *
	 * @param buf A pointer to the data
	 * @param size The number of bytes in the new ring buffer
	 */
	Ring(T * buf, u32 count) : Data(buf, count*sizeof(T)){
		m_count = count;
		m_head = 0;
		m_tail = 0;
		m_is_overflow_allowed = true;
	}

	/*! \details Constructs a new ring buffer.
	  *
	  * @param size The number of bytes to allocate for the new buffer.
	  *
	  */
	Ring(u32 count) : Data(count * sizeof(T)){
		m_count = count;
		m_head = 0;
		m_tail = 0;
		m_is_overflow_allowed = true;
	}

	u32 count() const { return m_count; }

	/*! \details Returns the number of items in the Ring
	 * that are ready to be popped.
	 *
	 * If this value is zero, then front() and back()
	 * have undefined results.  Also at() should
	 * not be passed parameters that are greater
	 * than or equaly to count_ready().
	 *
	 */
	u32 count_ready() const {
		//how many item are available
		if( m_tail == m_count ){
			return m_count; //ring is full
		}

		if( m_head > m_tail ){
			return m_head - m_tail;
		}

		return m_count - (m_tail - m_head);
	}

	void set_overflow_allowed(bool value = true){
		m_is_overflow_allowed = value;
	}

	bool is_overflow_allowed() const { return m_is_overflow_allowed; }


	T & at(u32 pos){
		u32 offset = m_head + pos;
		if( offset >= count() ){
			offset -= count();
		}
		return Data::at<T>(offset);
	}

	const T & at(u32 pos) const {
		u32 offset = m_head/sizeof(T) + pos;
		if( offset >= count() ){
			offset -= count();
		}
		return Data::at<T>(pos);
	}

	T & back(){
		if( m_tail == m_count ){
			return Data::at<T>(m_head);
		}
		return Data::at<T>(m_tail);
	}
	const T & back() const {
		if( m_tail == m_count ){
			return Data::at<T>(m_head);
		}
		return Data::at<T>(m_tail);
	}

	T & front()	{
		if( m_head ){
			return Data::at<T>(m_head-1);
		}
		return Data::at<T>(m_count-1);
	}

	const T & front()	const {
		if( m_head ){
			return Data::at<T>(m_head-1);
		}
		return Data::at<T>(m_count-1);
	}

	/*! \details Pushes a value on the Ring buffer.
	  *
	  * The ring buffer is a first-in first-out buffer. So
	  * The first object pushed is the first one popped.
	  *
	  */
	int push(const T & value){
		if( m_tail == m_count ){
			if( m_is_overflow_allowed ){
				back().~T(); //destruct item that will be lost
			} else {
				//can't overflow
				return -1;
			}
		}

		//construct a new item at head
		new((void*)(to<T>() + m_head)) T(value);

		//increment the head
		m_head++;
		if( m_head == m_count ){ m_head = 0; }

		if( m_head == m_tail ){ m_tail = m_count; } //ring is full
		return 0;
	}

	/*! \details Pops a value from the Ring buffer.
	  *
	  * \sa push()
	  *
	  */
	void pop(){
		if( m_head == m_tail ){
			//Ring is empty
			return;
		}
		back().~T();
		if( m_tail == m_count ){
			//ring is full
			m_tail = m_head;
		}
		m_tail++;
		if( m_tail == m_count ){ m_tail = 0; }
	}

private:

	u32 m_head;
	u32 m_tail;
	u32 m_count;
	bool m_is_overflow_allowed;

	u32 frame_size(){ return sizeof(T); }

};

} /* namespace var */

#endif /* VAR_RING_HPP_ */
