/*
 * Trace.hpp
 *
 *  Created on: Jul 31, 2014
 *      Author: tgil
 */

#ifndef TRACE_HPP_
#define TRACE_HPP_

#ifndef __link


#include <mcu/types.h>
#include <iface/link.h>
#include <iface/stratify_link_transport_usb.h>

#include "../var/String.hpp"

namespace sys {

class Trace : public var::String {
public:
	Trace(){}

	Trace& operator=(const char * a){ var::String::operator=(a); return *this; }
	Trace& operator=(const var::String & a){ var::String::operator=(a); return *this; }

	inline void message() MCU_ALWAYS_INLINE { stratify_trace_event(POSIX_TRACE_MESSAGE, c_str(), size()); }
	inline void warning() MCU_ALWAYS_INLINE { stratify_trace_event(POSIX_TRACE_WARNING, c_str(), size()); }
	inline void error() MCU_ALWAYS_INLINE { stratify_trace_event(POSIX_TRACE_ERROR, c_str(), size()); }
	inline void critical() MCU_ALWAYS_INLINE { stratify_trace_event(POSIX_TRACE_CRITICAL, c_str(), size()); }
	inline void fatal() MCU_ALWAYS_INLINE { stratify_trace_event(POSIX_TRACE_FATAL, c_str(), size()); }

};

};

#endif

#endif /* TRACE_HPP_ */
