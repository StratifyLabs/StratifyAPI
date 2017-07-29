/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#ifndef HAL_PINASSIGNMENT_HPP_
#define HAL_PINASSIGNMENT_HPP_

#include "../var/Item.hpp"

namespace hal {

template<typename pin_assignment_type> class PinAssignment {
public:
	PinAssignment(){ clear(); }

	operator const pin_assignment_type * () const { return &m_pin_assignment; }
	pin_assignment_type * operator ->(){ return &m_pin_assignment; }

	void clear(){
		memset(&m_pin_assignment,
				0xff,
				sizeof(m_pin_assignment));
	}

	u32 size() const { return sizeof(pin_assignment_type); }

	u32 count() const { return size() / sizeof(mcu_pin_t); }


private:
	pin_assignment_type m_pin_assignment;
};

} /* namespace hal */

#endif /* HAL_PINASSIGNMENT_HPP_ */
