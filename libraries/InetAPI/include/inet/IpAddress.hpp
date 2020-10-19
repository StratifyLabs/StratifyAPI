#ifndef INETAPI_INET_IPADDRESS_HPP
#define INETAPI_INET_IPADDRESS_HPP

#if defined __win32
#define _BSD_SOURCE
#include <stdint.h>
#include <winsock2.h>
#include <ws2tcpip.h>
typedef uint32_t in_addr_t;

#define SOCKET_T ::SOCKET

#else
#define SOCKET_T int
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include "var/String.hpp"
#include "var/View.hpp"

namespace inet {

class IpAddress {
public:
private:
};

class IpAddress4 {
public:
  IpAddress4() {}

  static IpAddress4 broadcast() { return IpAddress4(INADDR_BROADCAST); }

  explicit IpAddress4(in_addr_t value) : m_address(value) {}

  explicit IpAddress4(var::StringView value) {
    var::StringViewList value_list = value.split(".");
    if (value_list.count() != 4) {
      return;
    }
    m_address = (var::NumberString(value_list.at(0)).to_integer() << 24)
                | (var::NumberString(value_list.at(1)).to_integer() << 16)
                | (var::NumberString(value_list.at(2)).to_integer() << 8)
                | (var::NumberString(value_list.at(3)).to_integer() << 0);
  }

  IpAddress4(u8 a, u8 b, u8 c, u8 d) {
    m_address = a << 24 | b << 16 | c << 8 | d;
  }

  var::NumberString to_string() const {
    return std::move(var::NumberString().format(
      "%d.%d.%d.%d",
      ((m_address >> 24) & 0xff),
      ((m_address >> 16) & 0xff),
      ((m_address >> 8) & 0xff),
      ((m_address >> 0) & 0xff)));
  }

  static IpAddress4 from_string(const var::String &value) {
    var::StringViewList list = value.split(".");

    if (list.count() < 4) {
      return IpAddress4();
    }

    return IpAddress4(
      var::NumberString(list.at(0)).to_integer(),
      var::NumberString(list.at(1)).to_integer(),
      var::NumberString(list.at(2)).to_integer(),
      var::NumberString(list.at(3)).to_integer());
  }

  const in_addr_t &address() const { return m_address; }

private:
  in_addr_t m_address = INADDR_ANY;
};

class IpAddress6 {
public:
  IpAddress6(const struct in6_addr &address) : m_address(address) {}

  IpAddress6(var::StringView value) {
    var::StringViewList value_list = value.split(":");
    API_ASSERT(value_list.count() == 8);
    var::View view(m_address);
    size_t i = 0;
    for (const auto &value : value_list) {
      view.to_u16()[i++]
        = value.to_unsigned_long(var::String::Base::hexidecimal);
    }
  }

  IpAddress6(const var::Array<u16, 8> address) {
    var::View(m_address).copy(var::View(address));
  }

  var::NumberString to_string() const {
    const var::View view(m_address);
    var::NumberString result;
    for (size_t i = 0; i < 8; i++) {
      result.append(var::NumberString(view.at_const_u16(i), "%04x").cstring());
      if (i < 7) {
        result.append(":");
      }
    }
    return std::move(result);
  }

  static IpAddress6 from_string(const var::String &value) {
    var::StringViewList list = value.split(":");

    API_ASSERT(list.count() == 8);

    return IpAddress6(var::Array<u16, 8>(std::array<u16, 8>(
      {static_cast<u16>(
         list.at(0).to_unsigned_long(var::String::Base::hexidecimal)),
       static_cast<u16>(
         list.at(1).to_unsigned_long(var::String::Base::hexidecimal)),
       static_cast<u16>(
         list.at(2).to_unsigned_long(var::String::Base::hexidecimal)),
       static_cast<u16>(
         list.at(3).to_unsigned_long(var::String::Base::hexidecimal)),
       static_cast<u16>(
         list.at(5).to_unsigned_long(var::String::Base::hexidecimal)),
       static_cast<u16>(
         list.at(6).to_unsigned_long(var::String::Base::hexidecimal)),
       static_cast<u16>(
         list.at(7).to_unsigned_long(var::String::Base::hexidecimal))})));
  }

  const struct in6_addr &address() const { return m_address; }

private:
  struct in6_addr m_address;
};

} // namespace inet

#endif // INETAPI_INET_IPADDRESS_HPP
