/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_VAR_FLAGS_HPP_
#define SAPI_VAR_FLAGS_HPP_

/*! \cond */

#include "../api/VarObject.hpp"

namespace var {

class Flags : public api::InfoObject {
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

}

/*! \endcond */


#endif /* SAPI_VAR_FLAGS_HPP_ */
