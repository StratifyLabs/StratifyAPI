#include <unistd.h>
#include "inet/Socket.hpp"

using namespace inet;

Socket::Socket(){
#if defined __win32
    m_socket=INVALID_SOCKET;
    m_connectedsocket=INVALID_SOCKET;
#endif
}

int Socket::init() {
#if defined __win32
    WSADATA wsadata;
    int result;

    // Initialize Winsock
    result = WSAStartup(MAKEWORD(2,2), &wsadata);
    if (result != 0) {
        printf("WSAStartup failed with error: %d\n", result);
        set_error_number(WS_ERR_NET_INIT_FAILED);
        return WS_ERROR;
    }

    return WS_OK;
#else
	return 0;
#endif
}



int Socket::create(const SocketAttributes & socket_attributes){
#if defined __win32
    //SocketAddress address;
    m_socket = INVALID_SOCKET;
    init();
    m_socketattributes=socket_attributes;
    int family = socket_attributes.family() == m_socketattributes.FAMILY_INET ? AF_INET : AF_UNSPEC;
    int type = socket_attributes.type() == m_socketattributes.TYPE_STREAM ? SOCK_STREAM : SOCK_DGRAM;
    int protocol = socket_attributes.protocol() == m_socketattributes.PROTOCOL_TCP ? IPPROTO_TCP : IPPROTO_UDP;

    m_socket = socket(family, type, protocol);
    if (m_socket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        set_error_number(WS_ERR_NET_SOCKET_FAILED);
        return WS_ERROR;
    }


    return WS_OK;
#else
    return -1;
#endif
}


int Socket::bind() {
#if defined __win32
    struct addrinfo *result = NULL;
    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = m_socketattributes.family() == m_socketattributes.FAMILY_INET ? AF_INET : AF_UNSPEC;
    hints.ai_socktype = m_socketattributes.type() == m_socketattributes.TYPE_STREAM ? SOCK_STREAM : SOCK_DGRAM;
    hints.ai_protocol = m_socketattributes.protocol() == m_socketattributes.PROTOCOL_TCP ? IPPROTO_TCP : IPPROTO_UDP;;
    int flags = m_socketattributes.flags();
    if(flags&m_socketattributes.FLAG_PASSIVE){
        hints.ai_flags = AI_PASSIVE;
    }
    // Resolve the server address and port
    int ret = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( ret != 0 ) {
        printf("getaddrinfo failed with error: %d\n", ret);
        return WS_ERROR;
    }
    // Setup the TCP listening socket
    ret = ::bind( m_socket, result->ai_addr, (int)result->ai_addrlen);
    if (ret == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        set_error_number(WS_ERR_NET_BIND_FAILED);
        return WS_ERROR;
    } else {
        memcpy((void*)&(m_socketattributes.ipv4_address().m_sockaddr),(void*)result->ai_addr, sizeof(result->ai_addrlen));
    }
    freeaddrinfo(result);
    return WS_OK;
#else
    return -1;
#endif
}

int Socket::listen(){
#if defined __win32
    int ret = ::listen(m_socket, SOMAXCONN);
        if (ret == SOCKET_ERROR) {
            printf("listen failed with error: %d\n", WSAGetLastError());
            set_error_number(WS_ERR_NET_LISTEN_FAILED);
            return WS_ERROR;
        }
        return WS_OK;
#else
    return -1;
#endif
}

int Socket::accept(){
#if defined __win32
    int ret = WS_OK;

    // Accept a client socket
    m_connectedsocket = INVALID_SOCKET;
    m_connectedsocket = ::accept(m_socket, NULL, NULL);
    if (m_connectedsocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(m_connectedsocket);
        set_error_number(WS_ERR_NET_ACCEPT_FAILED);
        return WS_ERROR;
    }
    return m_connectedsocket;
#else
        return -1;
#endif
}
int Socket::connect() {
#if defined __win32
        // Connect to server.
    int ret = WS_OK;
    struct sockaddr_in* addr = (struct sockaddr_in*)&(m_socketattributes.ipv4_address().m_sockaddr);
    ret = ::connect( m_socket, (SOCKADDR*)addr, sizeof(*addr));
    if (ret == SOCKET_ERROR) {
        printf("Unable to connect to server!\n");
        set_error_number(WS_ERR_NET_CONNECT_FAILED);
        ret = WS_ERROR;
    }
    return m_socket;
#else

    return -1;
#endif
}

int Socket::send(var::ConstString send_buffer) {
#if defined __win32
    printf("Send : buffer length=%d\n",send_buffer.length());
    printf("Send : message - %s\n",send_buffer.c_str());
    printf("Connected socket=%d\n",connected_socket);
    int ret = ::send(connected_socket, send_buffer.c_str(), (int)send_buffer.length(), 0 );
    if (ret == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        return WS_ERROR;
    }
    printf("Bytes Sent: %ld\n", ret);
    return ret;
#else

         return -1;
#endif
}


int Socket::receive(var::ConstString receive_buffer, int buffer_length) {
#if defined __win32
    return ::recv(connected_socket, (char*)receive_buffer.c_str(), buffer_length, 0);
#else

         return -1;
#endif
}

int Socket::close() {
#if defined __win32
    // cleanup
    int ret = WS_OK;
    if(m_connectedsocket!=INVALID_SOCKET) {
        ret = ::shutdown(m_connectedsocket, SD_SEND);
        if (ret == SOCKET_ERROR) {
            printf("shutdown failed with error: %d\n", WSAGetLastError());
            set_error_number(WS_ERR_NET_SHUTDOWN_FAILED);
            return WS_ERROR;
        }

        closesocket(m_connectedsocket);
        m_connectedsocket=INVALID_SOCKET;
        ret = WS_ERROR;
    }

    if(m_socket!=INVALID_SOCKET) {
        closesocket(m_socket);
        m_socket=INVALID_SOCKET;
        ret = WS_ERROR;
    }
    WSACleanup();
    return ret;
#else

         return -1;
#endif
}


Socket::~Socket() {
    close();
}



//TODO:1)Move target_link_libraries(lws2_32.a) from CMakefiles.txt of sl to StratifyAPI
//2)Add variables to store Address family, protocol to be used.
