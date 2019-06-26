/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#ifndef SAPI_SYS_TRACE_HPP_
#define SAPI_SYS_TRACE_HPP_

#include <sos/link/types.h>

#if !defined __link
#include <sos/sos.h>
#endif

#include "../api/SysObject.hpp"
#include "../var/String.hpp"
#include "../chrono/ClockTime.hpp"

namespace sys {


/*! \brief TraceEvent class
 * \details The TraceEvent class
 * holds the information that was fired
 * in from a Trace.
 *
 */
class TraceEvent : public api::SysInfoObject {
public:
	TraceEvent(){
		memset(&m_event, 0, sizeof(m_event));
	}

	TraceEvent(const link_trace_event_t & event){
		m_event = event;
	}

	/*! \details Returns the trace id. */
	u16 id() const{ return m_event.posix_trace_event.posix_event_id; }
	/*! \details Returns the thread id. */
	u16 thread_id() const{ return m_event.posix_trace_event.posix_thread_id; }
	/*! \details Returns the process id. */
	u16 pid() const{ return m_event.posix_trace_event.posix_pid; }
	/*! \details Returns the program address where the trace was fired. */
	u32 program_address() const{ return m_event.posix_trace_event.posix_prog_address; }
	/*! \details Returns the timestamp when the trace was fired. */
	chrono::ClockTime timestamp() const{
		return chrono::ClockTime(m_event.posix_trace_event.posix_timestamp_tv_sec, m_event.posix_trace_event.posix_timestamp_tv_nsec);
	}

	/*! \details Returns the trace's message. */
	var::String message() const {
		char buffer[LINK_POSIX_TRACE_DATA_SIZE];
		memcpy(buffer, m_event.posix_trace_event.data, LINK_POSIX_TRACE_DATA_SIZE);
		buffer[LINK_POSIX_TRACE_DATA_SIZE-1] = 0;
		return var::String(var::ConstString(buffer));
	}

	/*! \cond */
	link_trace_event_t & event(){ return m_event; }
	const link_trace_event_t & event() const { return m_event; }

	void * info_to_void(){ return &m_event; }
	const void * info_to_void() const { return &m_event; }
	u32 info_size() const { return sizeof(m_event); }
	/*! \endcond */

private:
	link_trace_event_t m_event;
};

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
	inline void trace_message() MCU_ALWAYS_INLINE { sos_trace_event(LINK_POSIX_TRACE_MESSAGE, c_str(), size()); }
	/*! \details Sends trace as a warning. */
	inline void trace_warning() MCU_ALWAYS_INLINE { sos_trace_event(LINK_POSIX_TRACE_WARNING, c_str(), size()); }
	/*! \details Sends trace as an error. */
	inline void trace_error() MCU_ALWAYS_INLINE { sos_trace_event(LINK_POSIX_TRACE_ERROR, c_str(), size()); }
	/*! \details Sends trace as a critical message. */
	inline void trace_critical() MCU_ALWAYS_INLINE { sos_trace_event(LINK_POSIX_TRACE_CRITICAL, c_str(), size()); }
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

#endif /* SAPI_SYS_TRACE_HPP_ */
