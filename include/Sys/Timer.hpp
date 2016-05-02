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
	Timer();
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

	/*! \details The the usec's on the real-time clock */
	static int clock_usec(void);
	/*! \details The the msec's on the real-time clock */
	static int clock_msec(void);
	/*! \details The the sec's on the real-time clock */
	static int clock_sec(void);

	/*! \details Start the timer.
	 * \details This starts the timer.  It is used with value() and stop().
	 *
	 */
	void start(void); //start counting


	/*! \details This resumes counting after a stop().
	 *
	 */
	void resume(void); //resume counting

	/*! \details Check if timer is running */
	bool is_running();
	inline bool isrunning(){ return is_running(); }

	/*! \details Reset the value of the timer */
	void reset(void);

	/*! \details Timer value in milliseconds
	 * @return The number of milliseconds that have elapsed since start.
	 */
	u32 msec(void);

	/*! \details Timer value in microseconds
	 * @return The number of microseconds that have elapsed since start.
	 */
	u32 usec(void);
	u32 value(void){ return usec(); }

	/*! \details Timer value in seconds
	 * @return The number of seconds that have elapsed since start.
	 */
	u32 sec(void);

	/*! \details Stop the current timer.
	 * \details This stops the timer from counting.  Subsequent calls to value()
	 * will return the same number.
	 */
	void stop(void); //stop counting

private:
#ifdef __MCU_ONLY__
	static Tmr::port_t port;
	u32 start_;
	u32 stop_;
	u32 timeout_stop_;
#else
	struct timespec start_;
	struct timespec stop_;
#endif


};

};

#endif /* TIMER_HPP_ */
