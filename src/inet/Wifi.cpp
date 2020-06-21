#include "inet/Wifi.hpp"


#if defined __link

int wifi_no_warning = 0;

#else
using namespace inet;

WifiApi Wifi::m_api;

Wifi::Wifi(){

}

#endif
