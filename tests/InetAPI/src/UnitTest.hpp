
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

    if (!socket_api_case()) {
      return false;
    }

    return true;
  }

  static bool socket_server(UnitTest *self) {

    AddressInfo address_info(AddressInfo::Construct()
                               .set_family(AddressInfo::Family::inet)
                               .set_service("3000")
                               .set_flags(AddressInfo::Flags::passive)
                               .set_port(3000));

    self->printer().object("addressInfo", address_info);

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
    incoming.read(View(incoming_data))
      .write(View(incoming_data).truncate(return_value()));

    return true;
  }

  bool socket_api_case() {

    m_is_listening = false;
    Thread server_thread
      = Thread(Thread::Construct().set_argument(this).set_function(
        [](void *args) -> void * {
          socket_server(reinterpret_cast<UnitTest *>(args));
          return nullptr;
        }));

    PRINTER_TRACE(printer(), "wait listening");
    while (!m_is_listening && server_thread.is_running()) {
      wait(25_milliseconds);
    }

    {
      bool is_server_error = server_thread.error().error_number() > 0;
      TEST_EXPECT(!is_server_error);
      if (is_server_error) {
        server_thread.join();
        return false;
      }
    }

    PRINTER_TRACE(printer(), "get address info");
    AddressInfo address_info(AddressInfo::Construct()
                               .set_family(AddressInfo::Family::inet)
                               .set_service("3000")
                               .set_flags(AddressInfo::Flags::passive)
                               .set_port(3000));

    TEST_ASSERT(address_info.list().count() > 0);
    Socket socket(address_info.list().at(0));

    PRINTER_TRACE(printer(), "connect");
    TEST_ASSERT(socket.connect(address_info.list().at(0)).is_success());

    const StringView outgoing = "hello\n";
    Data incoming_data(64);
    TEST_ASSERT(socket.write(View(outgoing)).read(incoming_data).is_success());
    TEST_ASSERT(View(incoming_data).truncate(return_value()) == View(outgoing));
    PRINTER_TRACE(printer(), "done");
    return true;
  }

private:
  volatile bool m_is_listening;
};
