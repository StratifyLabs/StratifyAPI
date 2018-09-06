/*
 * Ring.hpp
 *
 *  Created on: Apr 11, 2017
 *      Author: tgil
 */

#ifndef VAR_RING_HPP_
#define VAR_RING_HPP_

#include <sos/dev/fifo.h>
#include <sys/types.h>
#include "Data.hpp"

namespace var {

class RingBuffer : public Data {
public:

    RingBuffer(void * buf, u32 size);
    RingBuffer(u32 size);

    /*! \details Accesses the size of the buffer. */
    u32 size() const { return m_size; }

    /*! \details Calculates the number of bytes calc_used in the buffer. */
    u32 calc_used() const;

    /*! \details Calculates the number of free bytes in the buffer. */
    u32 calc_free() const;

    /*! \details Flushes the buffer. */
    void flush(void);

    /*! \details Accesses whether or not buffer allows overwriting
     * The default value for overflow allowed is true. Use
     * set_overflow_allowed(false) to disable overflow.
     */
    bool is_overflow_allowed() const { return m_is_overflow_allowed; }

    /*! \details Sets whether or not overflow is allowed in the buffer.
     *
     * If overflow is allowed, write() will always write and return \a nbytes
     * as specified. If overflow is not allowed, write() will only write until
     * the buffer is full.
     *
     * @param value True to allow overflow or false to disallow it.
     */
    void set_overflow_allowed(bool value = true){
        m_is_overflow_allowed = value;
    }

protected:

    void inc_tail();
    void inc_head();
    u32 get_free_count(u32 count) const;

    u32 m_size;
    u32 m_head;
    u32 m_tail;
    bool m_is_overflow_allowed;
};


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
template<typename T> class Ring : public RingBuffer {
public:
	/*! \details Constructs a new ring buffer.
	 *
	 * @param buf A pointer to the data
	 * @param size The number of bytes in the new ring buffer
	 */
    Ring(T * buf, u32 count) : RingBuffer(buf, count*sizeof(T)){}


    /*! \details Constructs a new ring buffer.
     *
     * @param size The number of bytes to allocate for the new buffer.
     *
     */
    Ring(u32 count) : RingBuffer(count * sizeof(T)){}

	/*! \details Writes data to the ring buffer.
	 *
	 * @param buf Pointer to the data to write
     * @param count The number of bytes to write
	 * @return The number of bytes written
	 */
    int write(const T * buf, u32 count){
        u32 i;
        count = get_free_count(count);
        for(i=0; i < count; i++){
            type_data()[m_head] = buf[i];
            inc_head();

        }
        return count;
    }

    /*! \details Pushes a value on the Ring buffer.
     *
     * The ring buffer is a first-in first-out buffer. So
     * The first object pushed is the first one popped.
     *
     */
    int push(const T & value){
        return write(&value, 1);
    }

    /*! \details Pops a value from the Ring buffer.
     *
     * \sa push()
     *
     */
    T pop(){
        T value;
        read(&value, 1);
        return value;
    }

	/*! \details Reads data from the ring buffer.
	 *
	 * @param buf A pointer to the destination data buffer
     * @param count The number of bytes to read
	 * @return
	 */
    int read(T * buf, u32 count){
        u32 i;

        for(i=0; i < count; i++){
            if( m_tail != m_head ){
                if( m_tail == m_size ){
                    m_tail = m_head;
                }
                buf[i] = type_data()[m_tail];
                inc_tail();
            } else {
                return i;
            }
        }

        return i;
    }

private:


    T * type_data(){ return (T*)data(); }

};

} /* namespace var */

#endif /* VAR_RING_HPP_ */
