/*
 * Ring.hpp
 *
 *  Created on: Apr 11, 2017
 *      Author: tgil
 */

#ifndef VAR_RING_HPP_
#define VAR_RING_HPP_

#include <sys/types.h>
#include <mcu/types.h>

namespace var {


/*! \brief Ring Buffer class
 * \details Ring is a ring buffer (or circular buffer)
 * that uses a local memory buffer for the data.
 */
class Ring {
public:
	/*! \details Constructs a new ring buffer.
	 *
	 * @param buf A pointer to the data
	 * @param size The number of bytes in the new ring buffer
	 */
	Ring(u8 * buf, u32 size);

	/*! \details Writes data to the ring buffer.
	 *
	 * @param buf Pointer to the data to write
	 * @param nbytes The number of bytes to write
	 * @return The number of bytes written
	 */
	int write(const u8 * buf, u32 nbytes);

	/*! \details Reads data from the ring buffer.
	 *
	 * @param buf A pointer to the destination data buffer
	 * @param nbytes The number of bytes to read
	 * @return
	 */
	int read(u8 * buf, u32 nbytes);

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

private:
	u8 * m_buf;
	u32 m_size;

	u32 m_head;
	u32 m_tail;

	bool m_is_overflow_allowed;
};

} /* namespace var */

#endif /* VAR_RING_HPP_ */
