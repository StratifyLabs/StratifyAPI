/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef STFY_APP_RTC_HPP_
#define STFY_APP_RTC_HPP_

#include <iface/dev/rtc.h>

#include "Periph.hpp"

namespace Hal {


/*! \brief RTC Class
 * \details This class implements an RTC.
 */
class Rtc : public Periph {
public:
	Rtc(port_t port);
	/*! \details Get RTC attributes */
	int attr(rtc_attr_t * attr);
	/*! \details Set RTC attributes */
	int setattr(rtc_attr_t * attr);
	/*! \details Set RTC action */
	int setaction(rtc_action_t * action);
	/*! \details Set RTC alarm */
	int setalarm(rtc_alarm_t * alarm);
	/*! \details Get RTC alarm */
	int getalarm(rtc_alarm_t * alarm);
	/*! \details Disable RTC alarm */
	int disablealarm(void);
	/*! \details Set RTC time */
	int set(rtc_time_t * time);
	/*! \details Get RTC time */
	int get(rtc_time_t * time);

#ifdef __MCU_ONLY__
	int close();
#endif


private:

};

};

#endif /* STFY_APP_RTC_HPP_ */
