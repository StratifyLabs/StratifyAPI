/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <mcu/types.h>
#include <time.h>

#ifdef __MCU_ONLY__
#include "hal/Tmr.hpp"
#endif

namespace sys {

/*! \brief Timer Class
 * \details This class implements a logical timer based on the Stratify OS
 * system timer.
 *
 *
 *
 * Physical timers are controlled using the hal::Tmr class.
 *
 * Here is an example of using the timer to time events.
 * \code
 * #include <sapi/sys.hpp>
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

	/*! \details This method will delay the specified number of seconds.
	 *
	 * @param timeout Time to wait in seconds
	 */
	static void wait_sec(u32 timeout);

	/*! \details This method will delay the specified number of milliseconds.
	 *
	 * @param timeout Time to wait in milliseconds
	 */
	static void wait_msec(u32 timeout);

	/*! \details This method will delay the specified number of microseconds.
	 *
	 * @param timeout Time to wait in microseconds
	 */
	static void wait_usec(u32 timeout);


#if !defined __link

	Timer();


	/*! \details Returns the microseconds on the real-time clock (starts counting on reset) */
	static int get_clock_usec();

	/*! \details Returns the milliseconds on the real-time clock (starts counting on reset) */
	static int get_clock_msec();

	/*! \details Returns the seconds on the real-time clock (starts counting on reset) */
	static int get_clock_sec();

	/*! \details Starts the timer.
	 *
	 * If the timer is currently running, this method has no effect.  If the timer has been
	 * stopped, it will restart.  Use resume() to resume running a stopped timer.
	 *
	 */
	void start(); //start counting

	/*! \details Restarts the timer.
	 *
	 * If the timer is currently running,
	 * it starts over.  If it is not running, it is started.
	 *
	 */
	void restart();


	/*! \details Resumes counting after a stop().
	 *
	 * If the timer is currently running
	 * this method has no effect.  If the timer has not been started, this method will
	 * start the timer.
	 */
	void resume();

	/*! \details Returns true if the timer is currently counting meaning
	 * it has been started but has not been stopped.
	 *
	 * If the timer has been stopped and resumed, this method will return true;
	 *
	 */
	bool is_running() const { return !is_stopped(); }

	/*! \details Returns true if the timer has been started.
	 *
	 * It the timer has been started and stopped, this method will return true.
	 * If the timer has been reset() or never started, this method will return false.
	 *
	 */
	bool is_started() const { return (m_start.tv_nsec + m_start.tv_sec) != 0; }

	/*! \details Returns true if the timer is stopped.
	 *
	 * If the timer has not yet been started or has been reset(), this method will return true.
	 * If the timer is currently running, this method will return false.
	 *
	 *
	 */
	bool is_stopped() const { return (m_stop.tv_sec != (time_t)-1) || (m_stop.tv_sec == 0); }


	/*! \details Returns true if the timer is in a reset state.
	 *
	 */
	bool is_reset() const { return m_stop.tv_sec == 0; }



	/*! \details Resets the value of the timer.
	 *
	 *  After calling this method, is_running(), and is_started() will both
	 *  all return false;
	 *
	 */
	void reset();

	/*! \details Calculates the timer value in milliseconds.
	 *
	 * @return The number of milliseconds that have elapsed since start.  This value
	 * can be read when the timer is running to get a live value or after it has
	 * been stopped to get the time elapsed between start() and stop()
	 */
	u32 calc_msec() const;

	/*! \details Is equivalent to calc_msec(). */
	u32 msec() const { return calc_msec(); }

	/*! \details Calculates the timer value in microseconds.
	 *
	 * This is similar to calc_msec() but returns
	 * the value in microseconds rather than milliseconds.
	 *
	 * @return The number of microseconds that have elapsed since start.
	 */
	u32 calc_usec() const;

	/*! \details Is equivalent to calc_usec(). */
	u32 usec() const { return calc_usec(); }


	/*! \details Calculates the timer value in seconds.
	 *
	 * This is similar to calc_msec() but returns
	 * the value in seconds rather than milliseconds.
	 *
	 * @return The number of seconds that have elapsed since start.
	 */
	u32 calc_sec() const;

	/*! \details Is equivalent to calc_sec(). */
	u32 sec() const { return calc_sec(); }

	/*! \details Stops the timer from counting.
	 *
	 * Subsequent calls to value() will return the same number.  Once the timer has been stopped(),
	 * a call to resume() will resume counting and a call to start() or restart() will restart
	 * counting from zero.
	 */
	void stop();

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

}

#endif /* TIMER_HPP_ */
