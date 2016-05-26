/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef STFY_APP_RTC_HPP_
#define STFY_APP_RTC_HPP_

#include <iface/dev/rtc.h>
#include "Periph.hpp"


namespace hal {


/*! \brief RTC Class
 * \details This class implements an RTC.
 */
class Rtc : public Periph {
public:
	Rtc(port_t port);
	/*! \details Get RTC attributes */
	int get_attr(rtc_attr_t * attr);
	/*! \details Set RTC attributes */
	int set_attr(rtc_attr_t * attr);
	/*! \details Set RTC action */
	int set_action(rtc_action_t * action);
	/*! \details Set RTC alarm */
	int set_alarm(rtc_alarm_t * alarm);
	/*! \details Get RTC alarm */
	int get_alarm(rtc_alarm_t * alarm);
	/*! \details Disable RTC alarm */
	int disable_alarm();
	/*! \details Set RTC time */
	int set_time(rtc_time_t * time);
	/*! \details Get RTC time */
	int get_time(rtc_time_t * time);

#ifdef __MCU_ONLY__
	int close();
#endif


private:

};

};

#endif /* STFY_APP_RTC_HPP_ */
