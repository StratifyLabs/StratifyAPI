#include "hal/Switchboard.hpp"

using namespace hal;

Switchboard::Switchboard(){}


int Switchboard::open(const char * name){
    int ret;
    ret = File::open(name, RDWR);
    if (ret < 0 ){ return ret; }

    switchboard_info_t info;
    //see if the device responds to
    return ioctl(I_SWITCHBOARD_GETINFO, &info);
}

SwitchboardConnection Switchboard::get_connection(u16 idx) const {
    SwitchboardConnection connection;
    set_error_number_if_error( read(idx*sizeof(switchboard_connection_t), &connection.m_connection, sizeof(switchboard_connection_t)) );
    return connection;
}

int Switchboard::get_available_connection() const {
    int ret = seek(0);
    int idx = 0;
    if( ret < 0 ){
        return ret;
    }

    switchboard_status_t status;
    do {
        ret = read(&status, sizeof(status));
        if( status.o_flags != 0 ){
            idx++; //this one is busy
        }
    } while( (ret == sizeof(status)) && (status.o_flags != 0) );

    if( ret != sizeof(status) ){
        //no connections
        return -1;
    }

    return idx;
}


SwitchboardInfo Switchboard::get_info() const {
    SwitchboardInfo info;
    get_info(info.m_info);
    return info;
}

int Switchboard::get_info(switchboard_info_t & info) const {
    return ioctl(I_SWITCHBOARD_GETINFO, &info);
}

int Switchboard::set_attr(switchboard_attr_t & attr) const {
    return ioctl(I_SWITCHBOARD_SETATTR, &attr);
}

int Switchboard::create_persistent_connection(
        const SwitchboardTerminal & input,
        const SwitchboardTerminal & output,
        s32 nbyte) const {
    int idx = get_available_connection();

    if( idx < 0 ){
        set_error_number(ENOSPC);
        return -1;
    }

    switchboard_attr_t attr;
    attr.idx = idx;
    attr.o_flags = CONNECT | IS_PERSISTENT;
    attr.input = input.m_terminal;
    attr.output = output.m_terminal;
    attr.nbyte = nbyte; //max packet size
    return set_attr(attr);
}

int Switchboard::create_fixed_size_connection(
        const SwitchboardTerminal & input,
        const SwitchboardTerminal & output,
        s32 nbyte
        ) const {
    int idx = get_available_connection();

    if( idx < 0 ){
        set_error_number(ENOSPC);
        return -1;
    }

    switchboard_attr_t attr;
    attr.idx = idx;
    attr.o_flags = CONNECT | IS_FIXED_SIZE;
    attr.input = input.m_terminal;
    attr.output = output.m_terminal;
    attr.nbyte  = nbyte; //max packet size
    return set_attr(attr);
}


int Switchboard::destroy_connection(u16 idx) const {
    switchboard_attr_t attr;
    attr.idx = idx;
    attr.o_flags = DISCONNECT;
    return set_attr(attr);
}

int Switchboard::destroy_connection(SwitchboardConnection & connection){
    u16 idx = connection.idx();
    memset(&connection.m_connection, 0, sizeof(switchboard_connection_t));
    return destroy_connection(idx);
}

