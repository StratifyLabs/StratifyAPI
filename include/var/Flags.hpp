/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef FLAGS_HPP_
#define FLAGS_HPP_

#include <mcu/types.h>

namespace var {

class Flags {
public:
	Flags();

	inline void set(int flag, bool v){
		if( v ){
			_flags |= (1<<flag);
		} else {
			_flags &= ~(1<<flag);
		}
	}

	inline bool value(int flag) const {
		return (_flags & (1<<flag)) != 0;
	}

private:
	u32 _flags;
};

};

#endif /* FLAGS_HPP_ */
