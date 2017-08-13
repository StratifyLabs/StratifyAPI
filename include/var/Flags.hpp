/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef FLAGS_HPP_
#define FLAGS_HPP_

#include <mcu/types.h>

namespace var {

class Flags {
public:
	Flags();

	/*! \details Sets the value of the specified flag.
	 *
	 * @param flag The flag to set
	 * @param v The value to set for the flag
	 */
	void set_value(u32 flag, bool v = true){
		if( v ){
			m_flags |= (1<<flag);
		} else {
			m_flags &= ~(1<<flag);
		}
	}

	/*! \details Return the value of the flag
	 *
	 * @param flag The flag to read
	 * @return The flag value (true or false)
	 */
	bool value(u32 flag) const {
		return (m_flags & (1<<flag)) != 0;
	}

private:
	u32 m_flags;
};

};

#endif /* FLAGS_HPP_ */
