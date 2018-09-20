#include "inet/SocketAttributes.hpp"
#include "var/Token.hpp"
#include "var/Vector.hpp"

using namespace inet;

SocketAddress::SocketAddress(){
    struct sockaddr_in* address_in = (struct sockaddr_in*)&m_sockaddr;
    memset(address_in,0,sizeof(*address_in));
}

SocketAddress::SocketAddress(const SocketAddress &socketaddress) {
    memcpy((void*)&(m_sockaddr),(void*)&(socketaddress.m_sockaddr), sizeof(m_sockaddr));
}

SocketAddress::SocketAddress(u32 & ipaddr, int port) {
#if defined __win32
    set_socketipaddress(ipaddr);
    set_socketipaddress(port);
#endif
}

void SocketAddress::set_socketipaddress(u32 ipaddress) {
    struct sockaddr_in* s_address = (struct sockaddr_in*)(&m_sockaddr);
    s_address->sin_family = AF_INET;
    s_address->sin_addr.s_addr = ipaddress;
	 //printf("Constructor:Ip addr=%s\n",inet_ntoa(s_address->sin_addr));
}

void SocketAddress::set_port(u32 port) {
    struct sockaddr_in* s_address = (struct sockaddr_in*)(&m_sockaddr);
    s_address->sin_port = htons(port);
    printf("Constructor:port=%d\n",s_address->sin_port);
}

void Ipv4Address::set_address(const var::ConstString & address) {
    var::ConstString delimiter(".");
    var::Token address_token(address,delimiter);
    int count= address_token.count();
    var::Vector<var::ConstString> address_list;
    for(int index=0;index<count;index++) {
        address_list.push_back(address_token.at(index));
    }
    u32 a=0,b=0,c=0,d=0;
    if(address_list.count()==4) {
        a=((var::ConstString)address_list[0]).atoi();
        b=((var::ConstString)address_list[1]).atoi();
        c=((var::ConstString)address_list[2]).atoi();
        d=((var::ConstString)address_list[3]).atoi();
    }
    set_address(a,b,c,d);

}

SocketAttributes::SocketAttributes(){
    m_family = FAMILY_INET;
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
