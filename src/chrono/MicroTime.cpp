//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include "chrono/MicroTime.hpp"
#include "chrono/Timer.hpp"

using namespace chrono;


MicroTime::MicroTime(const Timer & timer){
	m_value_microseconds = timer.microseconds();
}



