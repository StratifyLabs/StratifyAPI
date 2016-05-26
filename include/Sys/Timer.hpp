/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <mcu/types.h>
#include <time.h>

#ifdef __MCU_ONLY__
#include "hal/Tmr.hpp"
#endif

namespace Sys {

/*! \brief Timer Class
 * \details This class implements a logical timer based on the Stratify OS
 * system timer.
 *
 *
 *
 * Physical timers are controlled using the Tmr class.
 *
 * Here is an example of using the timer to time events.
 * \code
 * #include <stfy/Sys.hpp>
 *
 *
 * int main(int argc, char * argv[]){
 * 	Timer t;
 * 	t.start(); //start
 * 	Timer::wait_usec(500);
 * 	t.stop();
 * 	printf("Timer value after 500usec is %d\n", t.usec());
 *
 * }
 * \endcode
 *
 * The output of the above code varies depending on the MCU clock cycles and the
 * scheduler.  The following is a sample output.
 *
 *     Timer value after 500usec is 502
 *
 *
 */
class Timer {
public:
#ifdef __MCU_ONLY__
	static int init(Tmr::port_t port, u32 heartbeat = 10000);
#endif

	/*! \details Wait for a number of seconds.
	 *
	 * @param timeout Time to wait in seconds
	 */
	static void wait_sec(u32 timeout);

	/*! \details Wait for a number of milliseconds.
	 *
	 * @param timeout Time to wait in milliseconds
	 */
	static void wait_msec(u32 timeout);
	/*! \details Wait for a number of microseconds.
	 *
	 * @param timeout Time to wait in microseconds
	 */
	static void wait_usec(u32 timeout);


#if !defined __link

	Timer();


	/*! \details The the usec's on the real-time clock */
	static int get_clock_usec();
	/*! \details The the msec's on the real-time clock */
	static int get_clock_msec();
	/*! \details The the sec's on the real-time clock */
	static int get_clock_sec();

	/*! \details Start the timer.
	 * \details This starts the timer.  It is used with value() and stop().
	 *
	 */
	void start(); //start counting


	/*! \details This resumes counting after a stop().
	 *
	 */
	void resume(); //resume counting

	/*! \details Check if timer is running */
	bool is_running();

	/*! \details Reset the value of the timer */
	void reset();

	/*! \details Timer value in milliseconds
	 * @return The number of milliseconds that have elapsed since start.
	 */
	u32 calc_msec();

	/*! \details Shortcut for calc_msec(); */
	u32 msec(){ return calc_msec(); }

	/*! \details Timer value in microseconds
	 * @return The number of microseconds that have elapsed since start.
	 */
	u32 calc_usec();

	/*! \details Shortcut for calc_usec(); */
	u32 usec(){ return calc_usec(); }

	u32 value(){ return calc_usec(); }

	/*! \details Timer value in seconds
	 * @return The number of seconds that have elapsed since start.
	 */
	u32 calc_sec();

	/*! \details Shortcut for calc_sec(); */
	u32 sec(){ return calc_sec(); }

	/*! \details Stop the current timer.
	 * \details This stops the timer from counting.  Subsequent calls to value()
	 * will return the same number.
	 */
	void stop(); //stop counting
#endif

private:
#ifdef __MCU_ONLY__
	static Tmr::port_t port;
	u32 m_start;
	u32 m_stop;
	u32 timeout_stop_;
#elif !defined __link
	struct timespec m_start;
	struct timespec m_stop;
#endif


};

};

#endif /* TIMER_HPP_ */
