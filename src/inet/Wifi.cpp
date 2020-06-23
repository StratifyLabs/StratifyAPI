#include "chrono.hpp"

#include "inet/Wifi.hpp"


#if defined __link

int wifi_no_warning = 0;

#else
using namespace inet;

WifiApi Wifi::m_api;

Wifi::Wifi(){

}


var::Vector<WifiSsidInfo> Wifi::scan(
		const WifiScanAttributes & attributes,
		const chrono::MicroTime & timeout
		){
	if( start_scan(attributes) < 0 ){
		return return_value();
	}

	Timer t;
	t.start();

	while((t < timeout) && is_scan_busy() ){
		wait(Milliseconds(50));
	}

	return get_ssid_info_list();
}


var::Vector<WifiSsidInfo> Wifi::get_ssid_info_list(){
	var::Vector<WifiSsidInfo> result;

	int count = api()->get_scan_count(m_context);
	for(int i=0; i < count; i++){
		wifi_ssid_info_t info;
		if(
			 set_error_number_if_error(
				 api()->get_ssid_info(m_context, i, &info)
				 ) < 0 ){
			return result;
		}

		result.push_back(info);
	}

	return result;
}

WifiIpInfo Wifi::connect(
		const WifiSsidInfo & ssid_info,
		const WifiAuthInfo & auth,
		const chrono::MicroTime & timeout
		){
	int result
			= set_error_number_if_error(
				api()->connect(
					m_context,
					&ssid_info.info(),
					&auth.auth()
					)
				);

	if( result < 0 ){
		return WifiIpInfo();
	}

	chrono::Timer t;
	t.start();
	WifiInfo info;
	do {
		chrono::wait(chrono::Milliseconds(50));
		info = get_info();
	} while(
					(t < timeout)
					|| !info.is_connected()
					|| !info.get_ip_info().is_valid()
					);

	if( info.is_connected() && info.get_ip_info().is_valid() ){
		return info.get_ip_info();
	}

	return WifiIpInfo();
}


#endif
