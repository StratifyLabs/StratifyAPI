
#include <cstdio>
#include <mcu/types.h>

#include "api/api.hpp"
#include "chrono.hpp"
#include "crypto.hpp"
#include "fs.hpp"
#include "inet.hpp"
#include "printer.hpp"
#include "sys.hpp"
#include "test/Test.hpp"
#include "thread.hpp"
#include "var.hpp"

using S = Socket;

class UnitTest : public test::Test {
public:
  UnitTest(var::StringView name) : test::Test(name) {}

  bool execute_class_api_case() {

    if (!socket_case()) {
      return false;
    }

    if (!socket_address_case()) {
      return false;
    }

    if (!http_client_case()) {
      return false;
    }

    return true;
  }

  bool http_client_case() {

    {}

    return true;
  }

  bool socket_address_case() {

    printer::PrinterObject po(printer(), "socketAddress");
    TEST_ASSERT(SocketAddress4().family() == S::Family::inet);
    TEST_ASSERT(SocketAddress6().family() == S::Family::inet6);

    TEST_ASSERT(
      SocketAddress4().set_protocol(S::Protocol::raw).protocol()
      == S::Protocol::raw);

    TEST_ASSERT(
      SocketAddress4().set_protocol(S::Protocol::ip).protocol()
      == S::Protocol::ip);

    TEST_ASSERT(
      SocketAddress4().set_protocol(S::Protocol::udp).protocol()
      == S::Protocol::udp);

    TEST_ASSERT(
      SocketAddress4().set_protocol(S::Protocol::tcp).protocol()
      == S::Protocol::tcp);

    TEST_ASSERT(
      SocketAddress4().set_address(IpAddress4("1.0.0.127")).get_address_string()
      == "1.0.0.127");

    TEST_ASSERT(
      SocketAddress4().set_address(IpAddress4(0)).get_address_string()
      == "0.0.0.0");
    printer().key(
      "address",
      SocketAddress4()
        .set_address(IpAddress4(0x12345678))
        .get_address_string());
    TEST_ASSERT(
      SocketAddress4().set_address(IpAddress4(0x12345678)).get_address_string()
      == "18.52.86.120");

    TEST_ASSERT(SocketAddress4().set_port(3000).port() == 3000);
    TEST_ASSERT(SocketAddress6().set_port(3000).port() == 3000);

    TEST_ASSERT(
      SocketAddress6().set_protocol(S::Protocol::raw).protocol()
      == S::Protocol::raw);

    TEST_ASSERT(
      SocketAddress6().set_protocol(S::Protocol::ip).protocol()
      == S::Protocol::ip);

    TEST_ASSERT(
      SocketAddress6().set_protocol(S::Protocol::udp).protocol()
      == S::Protocol::udp);

    TEST_ASSERT(
      SocketAddress6().set_protocol(S::Protocol::tcp).protocol()
      == S::Protocol::tcp);

    printer().key(
      "address6",
      SocketAddress6()
        .set_address(IpAddress6("12:34:56:78:ab:cd:ef:01"))
        .get_address_string());
    TEST_ASSERT(
      SocketAddress6()
        .set_address(IpAddress6("12:34:56:78:ab:cd:ef:01"))
        .get_address_string()
      == "0012:0034:0056:0078:00ab:00cd:00ef:0001");

    return true;
  }

  bool socket_case() {

    if (!socket_tcp_reflection_case(S::Family::inet)) {
      return false;
    }

    if (!socket_tcp_reflection_case(S::Family::inet6)) {
      return false;
    }

    if (!socket_tcp_reflection_case(S::Family::unspecified)) {
      return false;
    }

    if (!socket_udp_reflection_case(S::Family::inet)) {
      return false;
    }

    if (!socket_udp_reflection_case(S::Family::inet6)) {
      return false;
    }

    if (!socket_udp_reflection_case(S::Family::unspecified)) {
      return false;
    }

    return true;
  }

  static bool socket_tcp_server(UnitTest *self) {

    AddressInfo address_info(AddressInfo::Construct()
                               .set_family(self->m_family)
                               .set_node("")
                               .set_service(Ntos(self->m_server_port))
                               .set_type(Socket::Type::stream)
                               .set_flags(AddressInfo::Flags::passive));

    self->printer().object("tcpAddress", address_info);

    TEST_SELF_ASSERT(address_info.list().count() > 0);

    const SocketAddress &server_address = address_info.list().at(0);
    Socket server = std::move(
      Socket(server_address.family(), server_address.type())
        .set_option(SocketOption(
          S::Level::socket,
          S::NameFlags::socket_reuse_address | S::NameFlags::socket_reuse_port,
          true)));
    TEST_SELF_ASSERT(is_success());

    self->printer().object("serverAddress", address_info.list().at(0));
    TEST_SELF_ASSERT(server.bind_and_listen(server_address).is_success());
    SocketAddress accept_address;
    self->m_is_listening = true;
    self->printer().key("listening", self->m_is_listening);
    Socket incoming = server.accept(accept_address);
    TEST_SELF_ASSERT(is_success());

    // data for incoming header
    var::Data incoming_data(64);

    // echo incoming data
    incoming.read(incoming_data)
      .write(View(incoming_data).truncate(return_value()));

    return true;
  }

  bool socket_tcp_reflection_case(Socket::Family family) {

    Random().seed().randomize(View(m_server_port));

    m_server_port = (m_server_port % (65535 - 49152)) + 49152;
    m_family = family;

    Thread server_thread = start_server(socket_tcp_server);
    TEST_ASSERT(server_thread.is_running());

    PRINTER_TRACE(printer(), "get address info");
    AddressInfo address_info(AddressInfo::Construct()
                               .set_family(AddressInfo::Family::inet)
                               .set_node("localhost")
                               .set_service(Ntos(m_server_port))
                               .set_type(Socket::Type::stream)
                               .set_flags(AddressInfo::Flags::passive));

    TEST_ASSERT(address_info.list().count() > 0);
    Socket socket(address_info.list().at(0));

    PRINTER_TRACE(printer(), "connect");
    TEST_ASSERT(socket.connect(address_info.list().at(0)).is_success());

    const StringView outgoing = "hello";
    Data incoming_data(64);
    TEST_ASSERT(socket.write(View(outgoing)).read(incoming_data).is_success());
    PRINTER_TRACE(
      printer(),
      "incoming " + String(View(incoming_data).truncate(return_value())));
    TEST_ASSERT(View(incoming_data).truncate(return_value()) == View(outgoing));
    PRINTER_TRACE(printer(), "done");
    return true;
  }

  static bool socket_udp_server(UnitTest *self) {

    auto bind_socket = [self]() {
      AddressInfo address_info(AddressInfo::Construct()
                                 .set_family(self->m_family)
                                 .set_node("")
                                 .set_service(Ntos(self->m_server_port))
                                 .set_type(Socket::Type::datagram)
                                 .set_flags(AddressInfo::Flags::passive));

      self->printer().object("udpAddressInfo", address_info);

      for (const auto &a : address_info.list()) {
        Socket socket = std::move(Socket(a).bind(a));
        if (is_success()) {
          return std::move(socket);
        }

        API_RESET_ERROR();
      }

      API_RETURN_VALUE_ASSIGN_ERROR(
        std::move(Socket(self->m_family)),
        "",
        EINVAL);
    };

    Socket server_socket = bind_socket();
    SocketAddress server_address = server_socket.get_sock_name();

    self->printer().object("udpServerAddress", server_address);

    TEST_SELF_ASSERT(is_success());

    // data for incoming header
    var::Data incoming_data(64);

    PRINTER_TRACE(self->printer(), "udp server receive data");
    self->m_is_listening = true;
    // echo incoming data
    SocketAddress client_address = server_socket.receive_from(incoming_data);
    if (is_success()) {
      self->printer().object("clientAddress", client_address);
      server_socket.send_to(
        client_address,
        View(incoming_data).truncate(return_value()));
    }

    PRINTER_TRACE(self->printer(), "data received");

    return true;
  }

  bool socket_udp_reflection_case(Socket::Family family) {

    Random().seed().randomize(View(m_server_port));

    m_server_port = (m_server_port % (65535 - 49152)) + 49152;
    m_family = family;
    m_is_listening = false;

    Thread server_thread = start_server(socket_udp_server);
    TEST_ASSERT(server_thread.is_running());

    PRINTER_TRACE(printer(), "get address info");
    AddressInfo address_info(AddressInfo::Construct()
                               .set_family(AddressInfo::Family::inet)
                               .set_node("localhost")
                               .set_service(Ntos(m_server_port))
                               .set_type(Socket::Type::datagram)
                               .set_flags(AddressInfo::Flags::passive));

    TEST_ASSERT(address_info.list().count() > 0);
    Socket socket = std::move(Socket(address_info.list().at(0))
                                .set_option(SocketOption(
                                  S::Level::socket,
                                  S::NameFlags::socket_reuse_address,
                                  true)));

    const SocketAddress &address = address_info.list().at(0);

    const StringView outgoing = "hello";
    Data incoming_data(64);
    printer().object("udpSendToAddress", address);

    printer().key("udpProtocol", address.protocol() == S::Protocol::udp);

    SocketAddress server_address
      = socket.send_to(address, View(outgoing)).receive_from(incoming_data);

    TEST_ASSERT(is_success());
    printer().object("serverAddress", server_address);
    PRINTER_TRACE(
      printer(),
      "incoming " + String(View(incoming_data).truncate(return_value())));
    TEST_ASSERT(View(incoming_data).truncate(return_value()) == View(outgoing));
    PRINTER_TRACE(printer(), "done");
    return true;
  }

private:
  Socket::Family m_family;
  u16 m_server_port;
  volatile bool m_is_listening;
  bool (*m_server_function)(UnitTest *);

  Thread start_server(bool (*server_function)(UnitTest *self)) {
    m_server_function = server_function;
    m_is_listening = false;
    Thread server_thread
      = Thread(Thread::Construct().set_argument(this).set_function(
        [](void *args) -> void * {
          UnitTest *self = reinterpret_cast<UnitTest *>(args);
          self->m_server_function(self);
          return nullptr;
        }));

    PRINTER_TRACE(printer(), "wait listening");
    while (!m_is_listening && server_thread.is_running()) {
      wait(25_milliseconds);
    }
    return std::move(server_thread);
  }
};
