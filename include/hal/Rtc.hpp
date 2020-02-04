/*! \file */ //Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#ifndef SAPI_HAL_RTC_HPP_
#define SAPI_HAL_RTC_HPP_

#include <sos/dev/rtc.h>
#include "Periph.hpp"


namespace hal {

class RtcFlags {
public:
	enum rtc_flags {
		ENABLE /*! Set the alarm */ = RTC_FLAG_ENABLE,
		DISABLE /*! Set the alarm */ = RTC_FLAG_DISABLE,
		IS_SOURCE_EXTERNAL_32768 /*! External 32.768KHz Crystal */ = RTC_FLAG_IS_SOURCE_EXTERNAL_32768,
		IS_SOURCE_INTERNAL_40000 /*! Internal 40KHz Oscillator */ = RTC_FLAG_IS_SOURCE_INTERNAL_40000,
		ENABLE_ALARM /*! Enable the alarm */ = RTC_FLAG_ENABLE_ALARM,
		DISABLE_ALARM /*! Enable the alarm */ = RTC_FLAG_DISABLE_ALARM,
		IS_ALARM_ONCE /*! One time alarm */ = RTC_FLAG_IS_ALARM_ONCE,
		IS_ALARM_MINUTE /*! Alarm every minute */ = RTC_FLAG_IS_ALARM_MINUTE,
		IS_ALARM_HOURLY /*! Alarm every hour */ = RTC_FLAG_IS_ALARM_HOURLY,
		IS_ALARM_DAILY /*! Daily alarm */ = RTC_FLAG_IS_ALARM_DAILY,
		IS_ALARM_WEEKLY /*! Weekly alarm */ = RTC_FLAG_IS_ALARM_WEEKLY,
		IS_ALARM_MONTHLY /*! Monthly alarm */ = RTC_FLAG_IS_ALARM_MONTHLY,
		IS_ALARM_YEARLY /*! Yearly alarm */ = RTC_FLAG_IS_ALARM_YEARLY,
		ENABLE_COUNT_EVENT /*! Enable a count event */ = RTC_FLAG_ENABLE_COUNT_EVENT,
		IS_COUNT_SECOND /*! One time alarm */ = RTC_FLAG_IS_COUNT_SECOND,
		IS_COUNT_MINUTE /*! One time alarm */ = RTC_FLAG_IS_COUNT_MINUTE,
		IS_COUNT_HOUR /*! One time alarm */ = RTC_FLAG_IS_COUNT_HOUR,
		IS_COUNT_DAY_OF_WEEK /*! One time alarm */ = RTC_FLAG_IS_COUNT_DAY_OF_WEEK,
		IS_COUNT_DAY_OF_MONTH /*! One time alarm */ = RTC_FLAG_IS_COUNT_DAY_OF_MONTH,
		IS_COUNT_DAY_OF_YEAR /*! One time alarm */ = RTC_FLAG_IS_COUNT_DAY_OF_YEAR,
		IS_COUNT_WEEK /*! One time alarm */ = RTC_FLAG_IS_COUNT_WEEK,
		IS_COUNT_MONTH /*! One time alarm */ = RTC_FLAG_IS_COUNT_MONTH,
		IS_COUNT_YEAR /*! One time alarm */ = RTC_FLAG_IS_COUNT_YEAR,
		DISABLE_COUNT_EVENT /*! Enable a count event */ = RTC_FLAG_DISABLE_COUNT_EVENT,
	};
};

class RtcAttributes :
		public PeriphAttributes<rtc_attr_t>,
		public RtcFlags {
public:

private:

};

/*! \brief RTC Class
 * \details This class implements an RTC. (Not yet implemented)
 */
class Rtc :
		public Periph<rtc_info_t, rtc_attr_t, RtcAttributes, 'r'>,
		public RtcFlags {
public:
	Rtc(port_t port);

	/*! \details Set RTC time. */
	int set_time(const rtc_time_t & time) const;

	/*! \details Gets RTC time. */
	int get_time(rtc_time_t & time) const;

private:

};

}

#endif /* SAPI_HAL_RTC_HPP_ */
