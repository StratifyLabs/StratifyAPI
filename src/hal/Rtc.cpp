#include "hal/Rtc.hpp"

using namespace hal;

Rtc::Rtc(port_t port) : Periph(CORE_PERIPH_RTC, port){

}


int Rtc::set_time(const rtc_time_t & time) const {
    return set_error_number_if_error(ioctl(I_RTC_SET, &time));
}

int Rtc::get_time(rtc_time_t & time) const {
    return set_error_number_if_error(ioctl(I_RTC_GET, &time));
}
