/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#ifndef TRACE_HPP_
#define TRACE_HPP_


#include <mcu/types.h>
#include <sos/link.h>

#if !defined __link
#include <sos/link/transport_usb.h>
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
	inline void trace_message() MCU_ALWAYS_INLINE { sos_trace_event(POSIX_TRACE_MESSAGE, c_str(), size()); }
	inline void trace_warning() MCU_ALWAYS_INLINE { sos_trace_event(POSIX_TRACE_WARNING, c_str(), size()); }
	inline void trace_error() MCU_ALWAYS_INLINE { sos_trace_event(POSIX_TRACE_ERROR, c_str(), size()); }
	inline void trace_critical() MCU_ALWAYS_INLINE { sos_trace_event(POSIX_TRACE_CRITICAL, c_str(), size()); }
	inline void trace_fatal() MCU_ALWAYS_INLINE { sos_trace_event(POSIX_TRACE_FATAL, c_str(), size()); }
#else
	Trace& operator=(const char * a){ a = 0; return *this; }
	char * cdata(){ return m_cdata; }
	void assign(const char * str){ str = 0; } //suppress warnings
	inline void trace_message(){}
	inline void trace_warning(){}
	inline void trace_error(){}
	inline void trace_critical(){}
	inline void trace_fatal(){}
private:
	char m_cdata[256];
#endif

};

};

#endif /* TRACE_HPP_ */
