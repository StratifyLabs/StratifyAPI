#ifndef SAPI_INET_IPADDRESS_HPP
#define SAPI_INET_IPADDRESS_HPP

#include "var/String.hpp"

namespace inet {

class Ipv4Address {
public:
  Ipv4Address() {}

  explicit Ipv4Address(u32 value) : m_value(value) {}

  explicit Ipv4Address(const var::String &value) {
    var::StringList value_list = value.split(".");
    if (value_list.count() != 4) {
      return;
    }
    m_value = (value_list.at(0).to_integer() << 24)
              | (value_list.at(1).to_integer() << 16)
              | (value_list.at(2).to_integer() << 8)
              | (value_list.at(3).to_integer() << 0);
  }

  Ipv4Address(u8 a, u8 b, u8 c, u8 d) {
    m_value = a << 24 | b << 16 | c << 8 | d;
  }

  u32 value() const { return m_value; }

  var::String to_string() const {
    return var::String().format(
      "%d.%d.%d.%d",
      ((m_value >> 24) & 0xff),
      ((m_value >> 16) & 0xff),
      ((m_value >> 8) & 0xff),
      ((m_value >> 0) & 0xff));
  }

  static Ipv4Address from_string(const var::String &value) {
    var::StringList list = value.split(".");

    if (list.count() < 4) {
      return Ipv4Address();
    }

    return Ipv4Address(
      list.at(0).to_integer(),
      list.at(1).to_integer(),
      list.at(2).to_integer(),
      list.at(3).to_integer());
  }

private:
  u32 m_value = 0;
};

} // namespace inet

#endif // IPADDRESS_HPP
