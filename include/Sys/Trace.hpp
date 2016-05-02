/*
 * Trace.hpp
 *
 *  Created on: Jul 31, 2014
 *      Author: tgil
 */

#ifndef TRACE_HPP_
#define TRACE_HPP_


#include <iface/link.h>
#include <trace.h>

#include "../Var/String.hpp"

namespace Sys {

class Trace : public Var::String {
public:
	Trace();

	Trace& operator=(const char * a){ String::operator=(a); return *this; }
	Trace& operator=(const String & a){ String::operator=(a); return *this; }

	inline void message() MCU_ALWAYS_INLINE { posix_trace_event(POSIX_TRACE_MESSAGE, c_str(), size()); }
	inline void warning() MCU_ALWAYS_INLINE { posix_trace_event(POSIX_TRACE_WARNING, c_str(), size()); }
	inline void critical() MCU_ALWAYS_INLINE { posix_trace_event(POSIX_TRACE_CRITICAL, c_str(), size()); }
	inline void fatal() MCU_ALWAYS_INLINE { posix_trace_event(POSIX_TRACE_FATAL, c_str(), size()); }

};

};

#endif /* TRACE_HPP_ */
