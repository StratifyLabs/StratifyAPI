#include <unistd.h>
#include "inet/Socket.hpp"

using namespace inet;

Socket::Socket(){
#if defined __win32
    m_socket=INVALID_SOCKET;
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
#endif
}



int Socket::create(const SocketAttributes & socket_attributes){
#if defined __win32
    //SocketAddress address;
    m_socket = INVALID_SOCKET;

    init();

    //address = (SocketAddress&)socket_attributes.ipv4_address();
    //memcpy((void*)&(m_sockaddress.m_address),(void*)&(address.m_address), sizeof(address.m_address));
    //printf("Ip addr=%s\n",inet_ntoa(((struct sockaddr_in*)&(m_sockaddress.m_address))->sin_addr));
    //sockaddr addr=m_sockaddress.m_address;
    //struct sockaddr_in *addr_in = (struct sockaddr_in *)&address;
    m_socketattributes=socket_attributes;
    int family = socket_attributes.family() == m_socketattributes.FAMILY_INET ? AF_INET : AF_UNSPEC;
    int type = socket_attributes.type() == m_socketattributes.TYPE_STREAM ? SOCK_STREAM : SOCK_DGRAM;
    int protocol = socket_attributes.protocol() == m_socketattributes.PROTOCOL_TCP ? IPPROTO_TCP : IPPROTO_UDP;

    m_socket = socket(family, type, protocol);
    if (m_socket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        //WSACleanup();
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
            //WSACleanup();
            return WS_ERROR;
        }

        // Setup the TCP listening socket
        ret = ::bind( m_socket, result->ai_addr, (int)result->ai_addrlen);
        if (ret == SOCKET_ERROR) {
            printf("bind failed with error: %d\n", WSAGetLastError());
            freeaddrinfo(result);
//            closesocket(m_listen_socket);
//            WSACleanup();
            set_error_number(WS_ERR_NET_BIND_FAILED);
            return WS_ERROR;
        } else {
            memcpy((void*)&(m_socketattributes.ipv4_address().m_sockaddr),(void*)result->ai_addr, sizeof(result->ai_addrlen));
            //struct sockaddr* sockaddress = &(m_socketattributes.ipv4_address().m_sockaddr);
            //struct sockaddr* result_sockaddr = (result->ai_addr);
//            struct sockaddr_in* sockaddress_in = (struct sockaddr_in*)&(m_socketattributes.ipv4_address().m_sockaddr);
//            struct sockaddr_in* result_sockaddress_in = (struct sockaddr_in*)result->ai_addr;

//            sockaddress_in->sin_addr=result_sockaddress_in->sin_addr;
//            memcpy((void*)&(sockaddress_in->sin_addr.s_addr), (void*)&(result_sockaddress_in->sin_addr.s_addr),sizeof(result_sockaddress_in->sin_addr.s_addr));
//            sockaddress_in->sin_port = result_sockaddress_in->sin_port;
//            sockaddress_in->sin_family = result_sockaddress_in->sin_family;
        }

        freeaddrinfo(result);

#else
    return -1;
#endif
}

int Socket::listen(){
#if defined __win32
    int ret = ::listen(m_socket, SOMAXCONN);
        if (ret == SOCKET_ERROR) {
            printf("listen failed with error: %d\n", WSAGetLastError());
//            closesocket(m_listen_socket);
//            WSACleanup();
            set_error_number(WS_ERR_NET_LISTEN_FAILED);
            return WS_ERROR;
        }
        printf("ret=%d\n",ret);

        return WS_OK;
#else
    return -1;
#endif
}

int Socket::accept(){
#if defined __win32
    int ret;
    int send_result;
    char recv_buf[512];
    int recv_buf_len = 512;
    // Accept a client socket
    ::SOCKET client_socket = INVALID_SOCKET;
        client_socket = ::accept(m_socket, NULL, NULL);
        if (client_socket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(m_socket);
//            WSACleanup();
            set_error_number(WS_ERR_NET_ACCEPT_FAILED);
            return WS_ERROR;
        }

        // No longer need server socket
        //closesocket(m_listen_socket);
        printf("Sending and receiving data\n");
        // Receive until the peer shuts down the connection
        char* receive_buffer=new char[512];
            var::ConstString receive_buffer_str(receive_buffer);
        do {
            ret = receive(client_socket,receive_buffer_str, 512);
            if (ret > 0) {
                printf("Bytes received: %d\nMessage: %s\n", ret, receive_buffer_str.c_str());

            // Echo the buffer back to the sender
                send_result = send(client_socket,receive_buffer);
                if (send_result == SOCKET_ERROR) {
                    printf("send failed with error: %d\n", WSAGetLastError());
                    closesocket(client_socket);
//                    WSACleanup();
                    set_error_number(WS_ERR_NET_SEND_FAILED);
                    return WS_ERROR;
                }
            }
            else if (ret == 0)
                printf("Connection closing...\n");
            else  {
                printf("recv failed with error: %d\n", WSAGetLastError());
                closesocket(client_socket);
//                WSACleanup();
                set_error_number(WS_ERR_NET_RECV_FAILED);
                return WS_ERROR;
            }
        }while(ret>0);
//        // shutdown the connection since we're done
        ret = shutdown(client_socket, SD_SEND);
        if (ret == SOCKET_ERROR) {
            printf("client_socket:shutdown failed with error: %d\n", WSAGetLastError());
            closesocket(client_socket);
            //WSACleanup();
            set_error_number(WS_ERR_NET_SOCKET_FAILED);
            return WS_ERROR;
        }

//        // cleanup
//        closesocket(client_socket);
//        WSACleanup();

        return WS_OK;
#else
        return -1;
#endif
}


//int Socket::connect(const char *host,
//                    const char *port, int proto){
//    SOCKET ConnectSocket = INVALID_SOCKET;
//    //struct addrinfo *res;   // populated elsewhere in your code
//    struct sockaddr_in *ipv4 = (struct sockaddr_in *)m_sockaddress.m_address;
//    char ipAddress[INET_ADDRSTRLEN];
//    inet_ntop(AF_INET, &(ipv4->sin_addr), ipAddress, INET_ADDRSTRLEN);
//    port=ipv4->sin_port;
//    printf("The IP address is: %s\n", ipAddress);
//    struct addrinfo *result = NULL,
//                    *ptr = NULL,
//                    hints;
//    memset( &hints, 0, sizeof( hints ) );
//    hints.ai_family = ipv4->sin_family;
//    hints.ai_socktype = SOCK_STREAM;
//    hints.ai_protocol = IPPROTO_TCP;

//    // Resolve the server address and port

//    printf("host=%s port=%s\n",host,port);
//    int ret = getaddrinfo(host, port, &hints, &result);
//    if ( ret != 0 ) {
//        printf("getaddrinfo failed with error: %d\n", ret);
//        WSACleanup();
//        return WS_ERR_NET_CONNECT_FAILED;
//     }

//     // Attempt to connect to an address until one succeeds
//     for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {
//        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
//                        ptr->ai_protocol);
//        if (ConnectSocket == INVALID_SOCKET) {
//              printf("socket failed with error: %ld\n", WSAGetLastError());
//              WSACleanup();
//              return WS_ERR_NET_CONNECT_FAILED;
//         }

//         // Connect to server.
//        ret = ::connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
//         if (ret == SOCKET_ERROR) {
//            closesocket(ConnectSocket);
//            ConnectSocket = INVALID_SOCKET;
//            continue;
//         }
//         break;
//     }

//     freeaddrinfo(result);

//     if (ConnectSocket == INVALID_SOCKET) {
//         printf("Unable to connect to server!\n");
//         WSACleanup();
//         return WS_ERR_NET_CONNECT_FAILED;
//     }

//     return ConnectSocket;
//}

int Socket::connect() {

#if defined __win32
        // Connect to server.
        //SocketAddress& address = (SocketAddress&)m_socketattributes.ipv4_address();
        struct sockaddr_in* addr = (struct sockaddr_in*)&(m_socketattributes.ipv4_address().m_sockaddr);
        int ret = ::connect( m_socket, (SOCKADDR*)addr, sizeof(*addr));
        if (ret == SOCKET_ERROR) {
             //..closesocket(connect_socket);
             //..connect_socket = INVALID_SOCKET;
             printf("Unable to connect to server!\n");
             //..WSACleanup();
             set_error_number(WS_ERR_NET_CONNECT_FAILED);
             return -1;
         }

        // Send an initial buffer
        char* send_buffer=new char[512];
        var::ConstString send_buffer_str(send_buffer);
        printf("Enter data:");
        scanf("%[^\n]s",send_buffer);
        //strcpy(send_buffer,"Test message");
        printf("\nData entered:%s\n",send_buffer);
        ret = send( m_socket, send_buffer_str);
            if (ret == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
//                closesocket(ConnectSocket);
//                WSACleanup();
                set_error_number(WS_ERR_NET_SEND_FAILED);
                return -1;
            }

            printf("Bytes Sent: %ld\n", ret);

            // shutdown the connection since no more data will be sent
            ret = shutdown(m_socket, SD_SEND);
            if (ret == SOCKET_ERROR) {
                printf("shutdown failed with error: %d\n", WSAGetLastError());
//                closesocket(ConnectSocket);
//                WSACleanup();
                set_error_number(WS_ERR_NET_SHUTDOWN_FAILED);
                return -1;
            }

            // Receive until the peer closes the connection
            char* receive_buffer=new char[512];
            var::ConstString receive_buffer_str(receive_buffer);
            do {
                ret = receive(m_socket, receive_buffer_str, 512);
                if ( ret > 0 )
                    printf("Bytes received: %d\n%s\n", ret, receive_buffer_str.c_str());
                else if ( ret == 0 )
                    printf("Connection closed\n");
                else {
                    printf("recv failed with error: %d\n", WSAGetLastError());
                    set_error_number(WS_ERR_NET_RECV_FAILED);
                    ret=-1;
                    break;
                }

            }while(ret>0);


            return ret;
#else

         return -1;
#endif
}

int Socket::send(SOCKET send_socket, var::ConstString send_buffer) {
#if defined __win32
    printf("Send : buffer length=%d\n",send_buffer.length());
    printf("Send : message - %s\n",send_buffer.c_str());
    int ret = ::send( send_socket, send_buffer.c_str(), (int)send_buffer.length(), 0 );
    if (ret == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
//      closesocket(socket);
//      WSACleanup();
        return -1;
    }
    printf("Bytes Sent: %ld\n", ret);
    return ret;
#else

         return -1;
#endif
}


int Socket::receive(SOCKET receive_socket, var::ConstString receive_buffer, int buffer_length) {
#if defined __win32
    // Receive until the peer closes the connection
    int result = 0;
        //do {

            result = ::recv(receive_socket, (char*)receive_buffer.c_str(), buffer_length, 0);

        //} while( result > 0 );
    return result;
#else

         return -1;
#endif
}

int Socket::close() {
#if defined __win32
    // cleanup
    if(m_socket!=INVALID_SOCKET) {
        closesocket(m_socket);
        m_socket=INVALID_SOCKET;
    }
    WSACleanup();
#else

         return -1;
#endif
}


Socket::~Socket() {
    close();
}



//TODO:1)Move target_link_libraries(lws2_32.a) from CMakefiles.txt of sl to StratifyAPI
//2)Add variables to store Address family, protocol to be used.
