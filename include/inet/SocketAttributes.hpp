#ifndef SOCKETATTRIBUTES_HPP
#define SOCKETATTRIBUTES_HPP

#if defined __win32
#define _BSD_SOURCE
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#endif

#include "../api/InetObject.hpp"
#include "../var/ConstString.hpp"
#include "../var/Vector.hpp"

/* \tg
 *
 * We don't want to expose any Winsocket API's in any hpp files.
 *
 *
 */

namespace inet {

class Socket;


}

#endif // SOCKETATRRIBUTES_HPP
