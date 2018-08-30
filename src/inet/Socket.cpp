#include <unistd.h>
#include "inet/Socket.hpp"

using namespace inet;

Socket::Socket(){}

int Socket::create(const SocketAddress & address){

/*
 * \tg
 *
 * Enclose all windows specific code in
 * #if defined __win32
 *
 * //windows code goes in here
 *
 * #endif
 *
 *
 */

#if defined __win32
    WSADATA wsadata;
    int result;
    // Initialize Winsock
    result = WSAStartup(MAKEWORD(2,2), &wsadata);
    if (result != 0) {
        printf("WSAStartup failed with error: %d\n", result);
        return WS_ERR_NET_SOCKET_FAILED;
    }
    memcpy((void*)&(m_sockaddress.m_address),(void*)&(address.m_address), sizeof(address.m_address));
    printf("Ip addr=%s\n",inet_ntoa(((struct sockaddr_in*)&(m_sockaddress.m_address))->sin_addr));
    return WS_OK;
#endif

    return -1;
}


/*
 * \tg
 *
 * Method names should be construct_sockaddr() but we
 * also don't want to abbreviate. So this should be
 * construct_socket_address() - but this will actually
 * go away because the SocketAddress() constructor should be used
 * to construct a socket address.
 *
 *
 */
#if defined __win32
void Socket::constructsockaddr(SocketAddress& address, const char* ipaddr, int port) {
    struct sockaddr* sockaddress = &(address.m_address);
    struct sockaddr_in* s_address = (struct sockaddr_in*)sockaddress;
    s_address->sin_family = AF_INET;
    s_address->sin_addr.s_addr = inet_addr(ipaddr);
    s_address->sin_port = htons(port);
    printf("Ip addr=%s\n",inet_ntoa(s_address->sin_addr));
}
#endif

int Socket::listen(){
    return -1;
}

int Socket::accept(){
    return -1;
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

    /*
     * Please only use CamelCase for class names like
     * SocketAddress. Variable names should be socket_address.
     *
     * So ConnectSocket should be connect_socket
     *
     *
     *
     */

        SOCKET ConnectSocket = INVALID_SOCKET;
        sockaddr addr=m_sockaddress.m_address;
        struct sockaddr_in *addr_in = (struct sockaddr_in *)&addr;

        ConnectSocket = socket(addr_in->sin_family, SOCK_STREAM,
                            IPPROTO_TCP);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return WS_ERR_NET_CONNECT_FAILED;
        }
        // Connect to server.
        int ret = ::connect( ConnectSocket, (SOCKADDR*)&addr, sizeof(addr));
        if (ret == SOCKET_ERROR) {
             closesocket(ConnectSocket);
             ConnectSocket = INVALID_SOCKET;
             printf("Unable to connect to server!\n");
             WSACleanup();
             return WS_ERR_NET_CONNECT_FAILED;
         }
         return ConnectSocket;
#endif

         return -1;

}


//TODO:1)Move target_link_libraries(lws2_32.a) from CMakefiles.txt of sl to StratifyAPI
//2)Add variables to store Address family, protocol to be used.
