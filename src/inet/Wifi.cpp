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


#endif
