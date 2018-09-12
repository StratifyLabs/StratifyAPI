#include "inet/SocketAttributes.hpp"

using namespace inet;

SocketAddress::SocketAddress(){
}

SocketAddress::SocketAddress(const SocketAddress &socketaddress) {
    memcpy((void*)&(m_sockaddr),(void*)&(socketaddress.m_sockaddr), sizeof(m_sockaddr));
}

SocketAddress::SocketAddress(const var::ConstString & ipaddr, int port) {
#if defined __win32
    struct sockaddr* sockaddress = &m_sockaddr;
    struct sockaddr_in* s_address = (struct sockaddr_in*)sockaddress;
    s_address->sin_family = AF_INET;
    s_address->sin_addr.s_addr = inet_addr(ipaddr.c_str());
    s_address->sin_port = htons(port);
    printf("Constructor:Ip addr=%s\n",inet_ntoa(s_address->sin_addr));
#endif
}

void Ipv4Address::set_address(const var::ConstString & address) {


//    int pos=0;
//    var::ConstString substr[4];
//    int substr_count=0;
//    int temp_index = var::ConstString::npos;
//    do
//     {
//        temp_index = address.find('.',pos);
//        if(temp_index!=var::ConstString::npos) {
//            substr[substr_count++]=address.substr(pos,temp_index-pos);
//            pos=temp_index;
//        }
//    } while(temp_index>var::ConstString::npos);
//    substr[substr_count++]=address.substr(pos,address.length()-pos);

//    u32 a,b,c,d;
//    if(substr_count==4) {
//        a=substr[0].atoi();
//        b=substr[1].atoi();
//        c=substr[2].atoi();
//        d=substr[3].atoi();
//        set_address(a,b,c,d);
//    }
    var::ConstString delimiter(".");
    var::Token address_token(address,delimiter);
    int count= address_token.count();
    var::Vector<var::ConstString> address_list;
    for(int index=0;index<count;index++) {
        address_list.push_back(address_token.at(index));
    }
    u32 a=0,b=0,c=0,d=0;
    if(address_list.size()==4) {
        a=((var::ConstString)address_list[0]).atoi();
        b=((var::ConstString)address_list[1]).atoi();
        c=((var::ConstString)address_list[2]).atoi();
        d=((var::ConstString)address_list[3]).atoi();
    }
    set_address(a,b,c,d);

}

SocketAttributes::SocketAttributes(){
    m_family = FAMILY_NONE;
    m_type=TYPE_STREAM;
    m_protocol=PROTOCOL_TCP;
    m_flag=FLAG_PASSIVE;
}

bool SocketAttributes::is_valid() const {
    bool valid=true;
    if(m_family!=FAMILY_NONE && m_family!=FAMILY_INET) {
        valid=false;
        return valid;
    } else if(m_type!=TYPE_NONE && m_type!=TYPE_STREAM) {
        valid=false;
        return valid;
    } else if(m_protocol!=PROTOCOL_TCP && m_protocol!=PROTOCOL_UDP) {
        valid=false;
        return valid;
    }
    return valid;
}
