/*
 * Trace.hpp
 *
 *  Created on: Jul 31, 2014
 *      Author: tgil
 */

#ifndef TRACE_HPP_
#define TRACE_HPP_


#include <mcu/types.h>
#include <iface/link.h>

#if !defined __link
#include <iface/stratify_link_transport_usb.h>
#endif

#include "../var/String.hpp"

namespace sys {

#if defined __link
class Trace {
#else
class Trace : public var::String {
#endif
public:
	Trace(){}

#if !defined __link
	Trace& operator=(const char * a){ var::String::operator=(a); return *this; }
	Trace& operator=(const var::String & a){ var::String::operator=(a); return *this; }
	inline void message() MCU_ALWAYS_INLINE { stratify_trace_event(POSIX_TRACE_MESSAGE, c_str(), size()); }
	inline void warning() MCU_ALWAYS_INLINE { stratify_trace_event(POSIX_TRACE_WARNING, c_str(), size()); }
	inline void error() MCU_ALWAYS_INLINE { stratify_trace_event(POSIX_TRACE_ERROR, c_str(), size()); }
	inline void critical() MCU_ALWAYS_INLINE { stratify_trace_event(POSIX_TRACE_CRITICAL, c_str(), size()); }
	inline void fatal() MCU_ALWAYS_INLINE { stratify_trace_event(POSIX_TRACE_FATAL, c_str(), size()); }
#else
	Trace& operator=(const char * a){ a = 0; return *this; }
	char * cdata(){ return m_cdata; }
	void assign(const char * str){ str = 0; } //suppress warnings
	inline void message(){}
	inline void warning(){}
	inline void error(){}
	inline void critical(){}
	inline void fatal(){}
private:
	char m_cdata[256];
#endif

};

};

#endif /* TRACE_HPP_ */
