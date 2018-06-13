//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include <unistd.h>
#include <fcntl.h>
#include "hal/Core.hpp"
using namespace hal;

SerialNumber::SerialNumber(){
    memset(&m_serial_number, 0, sizeof(mcu_sn_t));
}

SerialNumber SerialNumber::from_string(const char * str){
    SerialNumber ret;
    u32 len = strnlen(str, 8*4);
    if( len == 8*4 ){
#if defined __link
        sscanf(str, "%08X%08X%08X%08X",
#else
        sscanf(str, "%08lX%08lX%08lX%08lX",
#endif
               &ret.m_serial_number.sn[3],
                &ret.m_serial_number.sn[2],
                &ret.m_serial_number.sn[1],
                &ret.m_serial_number.sn[0]);
    }
    return ret;
}

SerialNumber& SerialNumber::operator = (const char * str){
    SerialNumber serial_number = from_string(str);
    memcpy(&m_serial_number, &serial_number.m_serial_number, sizeof(mcu_sn_t));
    return *this;
}

bool SerialNumber::operator == (const SerialNumber & serial_number){
    return memcmp(&serial_number.m_serial_number, &m_serial_number, sizeof(mcu_sn_t)) == 0;
}

int SerialNumber::get(){
    int ret = -1;
    Core core(0);
    core_info_t info;
    if( core.open() >= 0 ){
        if( core.get_info(info) >= 0 ){
            memcpy(&m_serial_number, &info.serial_number, sizeof(mcu_sn_t));
            ret = 0;
        }
        core.close();
    }
    return ret;
}

void SerialNumber::print() const {
#if defined __link
    ::printf("SN:%X%X%X%X\n",
#else
    ::printf("SN:%lX%lX%lX%lX\n",
#endif
            m_serial_number.sn[3],
            m_serial_number.sn[2],
            m_serial_number.sn[1],
            m_serial_number.sn[0]);
}

var::String SerialNumber::to_string() const {
    var::String ret;
    ret.set_transfer_ownership();
#if defined __link
    ret.sprintf("SN:%X%X%X%X\n",
#else
    ret.sprintf("SN:%lX%lX%lX%lX\n",
#endif
            m_serial_number.sn[3],
            m_serial_number.sn[2],
            m_serial_number.sn[1],
            m_serial_number.sn[0]);
    return ret;
}


Core::Core(port_t port) : Periph(CORE_PERIPH_CORE, port){}


int Core::set_pin_function(const core_pinfunc_t & req){
    return ioctl(I_CORE_SETPINFUNC, &req);
}

int Core::get_mcu_board_config(mcu_board_config_t & config){
    return ioctl(I_CORE_GETMCUBOARDCONFIG, &config);
}

int Core::set_clock_divide(u32 value){
    return ioctl(I_CORE_SETCLKDIVIDE, value);
}

void Core::reset(){
    core_attr_t attr;
    attr.o_flags = CORE_FLAG_EXEC_RESET;
    set_attr(attr);
}

void Core::invoke_bootloader(){
    core_attr_t attr;
    attr.o_flags = CORE_FLAG_EXEC_INVOKE_BOOTLOADER;
    set_attr(attr);
}

