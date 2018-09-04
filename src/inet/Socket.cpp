#include <unistd.h>
#include "inet/Socket.hpp"

using namespace inet;

SocketAddress::SocketAddress(const var::ConstString& ipaddr, int port) {
#if defined _WIN32
    struct sockaddr* sockaddress = &(m_address);
    struct sockaddr_in* s_address = (struct sockaddr_in*)sockaddress;
    s_address->sin_family = AF_INET;
    s_address->sin_addr.s_addr = inet_addr(ipaddr.c_str());
    s_address->sin_port = htons(port);
    printf("Constructor:Ip addr=%s\n",inet_ntoa(s_address->sin_addr));
#endif
}

Socket::Socket(){}

#if defined _WIN32
int Socket::create() {
    WSADATA wsadata;
    int result;
    // Initialize Winsock
    result = WSAStartup(MAKEWORD(2,2), &wsadata);
    if (result != 0) {
        printf("WSAStartup failed with error: %d\n", result);
        return -1;
    }
    return 0;
}
#endif

int Socket::create(const SocketAddress & address){
#if defined _WIN32
    create();
    memcpy((void*)&(m_sockaddress.m_address),(void*)&(address.m_address), sizeof(address.m_address));
    printf("Ip addr=%s\n",inet_ntoa(((struct sockaddr_in*)&(m_sockaddress.m_address))->sin_addr));
    return WS_OK;
#else

    return -1;
#endif
}

int Socket::listen(){
#if defined _WIN32
    struct addrinfo *result = NULL;
        struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        // Resolve the server address and port
        int ret = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
        if ( ret != 0 ) {
            printf("getaddrinfo failed with error: %d\n", ret);
            WSACleanup();
            return -1;
        }

        // Create a SOCKET for connecting to server
        m_listen_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (m_listen_socket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            return -1;
        }

        // Setup the TCP listening socket
        ret = bind( m_listen_socket, result->ai_addr, (int)result->ai_addrlen);
        if (ret == SOCKET_ERROR) {
            printf("bind failed with error: %d\n", WSAGetLastError());
            freeaddrinfo(result);
            closesocket(m_listen_socket);
            WSACleanup();
            return -1;
        } else {
            struct sockaddr* sockaddress = &(m_sockaddress.m_address);
            struct sockaddr* result_sockaddr = (result->ai_addr);
            struct sockaddr_in* sockaddress_in = (struct sockaddr_in*)sockaddress;
            struct sockaddr_in* result_sockaddress_in = (struct sockaddr_in*)result_sockaddr;
            sockaddress_in->sin_addr=result_sockaddress_in->sin_addr;
            memcpy((void*)&(sockaddress_in->sin_addr.s_addr), (void*)&(result_sockaddress_in->sin_addr.s_addr),sizeof(result_sockaddress_in->sin_addr.s_addr));
            sockaddress_in->sin_port = result_sockaddress_in->sin_port;
            sockaddress_in->sin_family = result_sockaddress_in->sin_family;
        }

        freeaddrinfo(result);

        ret = ::listen(m_listen_socket, SOMAXCONN);
        if (ret == SOCKET_ERROR) {
            printf("listen failed with error: %d\n", WSAGetLastError());
            closesocket(m_listen_socket);
            WSACleanup();
            return -1;
        }
        printf("ret=%d\n",ret);


#else
    return -1;
#endif
}

int Socket::accept(){
#if defined _WIN32
    // Accept a client socket
    SOCKET client_socket = INVALID_SOCKET;
        client_socket = ::accept(m_listen_socket, NULL, NULL);
        if (client_socket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(m_listen_socket);
            WSACleanup();
            return -1;
        }

        // No longer need server socket
        closesocket(m_listen_socket);

//        // Receive until the peer shuts down the connection
//        do {

//            iResult = recv(client_socket, recvbuf, recvbuflen, 0);
//            if (iResult > 0) {
//                printf("Bytes received: %d\n", iResult);

//            // Echo the buffer back to the sender
//                iSendResult = send( client_socket, recvbuf, iResult, 0 );
//                if (iSendResult == SOCKET_ERROR) {
//                    printf("send failed with error: %d\n", WSAGetLastError());
//                    closesocket(client_socket);
//                    WSACleanup();
//                    return -1;
//                }
//                printf("Bytes sent: %d\n", iSendResult);
//            }
//            else if (iResult == 0)
//                printf("Connection closing...\n");
//            else  {
//                printf("recv failed with error: %d\n", WSAGetLastError());
//                closesocket(client_socket);
//                WSACleanup();
//                return -1;
//            }

//        } while (iResult > 0);

//        // shutdown the connection since we're done
//        iResult = shutdown(client_socket, SD_SEND);
//        if (iResult == SOCKET_ERROR) {
//            printf("shutdown failed with error: %d\n", WSAGetLastError());
//            closesocket(client_socket);
//            WSACleanup();
//            return 1;
//        }

        // cleanup
        closesocket(client_socket);
        WSACleanup();

        return 0;
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

#if defined _WIN32
        SOCKET connect_socket = INVALID_SOCKET;
        sockaddr addr=m_sockaddress.m_address;
        struct sockaddr_in *addr_in = (struct sockaddr_in *)&addr;

        connect_socket = socket(addr_in->sin_family, SOCK_STREAM,
                            IPPROTO_TCP);
        if (connect_socket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return -1;
        }
        // Connect to server.
        int ret = ::connect( connect_socket, (SOCKADDR*)&addr, sizeof(addr));
        if (ret == SOCKET_ERROR) {
             closesocket(connect_socket);
             connect_socket = INVALID_SOCKET;
             printf("Unable to connect to server!\n");
             WSACleanup();
             return -1;
         }
         return connect_socket;
#else

         return -1;
#endif
}


//TODO:1)Move target_link_libraries(lws2_32.a) from CMakefiles.txt of sl to StratifyAPI
//2)Add variables to store Address family, protocol to be used.
