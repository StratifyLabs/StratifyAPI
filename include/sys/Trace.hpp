/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#ifndef SYS_TRACE_HPP_
#define SYS_TRACE_HPP_


#include <mcu/types.h>
#include <sos/link.h>

#if !defined __link
#include <sos/link/transport_usb.h>
#endif

#include "../var/String.hpp"

namespace sys {

/*! \brief Trace Class
 * \details The Trace class is used for writing messages, warnings and
 * errors to the system trace system.
 *
 * The trace includes:
 * - System time stamp
 * - Code address of trace
 * - Process ID
 * - Thread ID
 * - Type (message, waring, critical, or error)
 * - Message (limited to 20 bytes)
 *
 */
#if defined __link
class Trace {
#else
class Trace : public var::String {
#endif
public:
	Trace(){}

#if !defined __link
    Trace& operator=(const var::ConstString & a){ var::String::operator=(a); return *this; }
	/*! \details Sends trace as a message. */
	inline void trace_message() MCU_ALWAYS_INLINE { sos_trace_event(POSIX_TRACE_MESSAGE, c_str(), size()); }
	/*! \details Sends trace as a warning. */
	inline void trace_warning() MCU_ALWAYS_INLINE { sos_trace_event(POSIX_TRACE_WARNING, c_str(), size()); }
	/*! \details Sends trace as an error. */
	inline void trace_error() MCU_ALWAYS_INLINE { sos_trace_event(POSIX_TRACE_ERROR, c_str(), size()); }
	/*! \details Sends trace as a critical message. */
	inline void trace_critical() MCU_ALWAYS_INLINE { sos_trace_event(POSIX_TRACE_CRITICAL, c_str(), size()); }
	/*! \details Sends trace as a fatal message.
	 *
	 * Trace fatal will not cause the program to exit but for good practice, you must
	 * invoke exit() immediately after a trace_fatal() call.
	 *
	 */
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

}

#endif /* SYS_TRACE_HPP_ */
