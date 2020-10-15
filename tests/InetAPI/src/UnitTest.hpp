
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

    if (!server_api_case()) {
      return false;
    }

    return true;
  }

  bool server_api_case() {

    AddressInfo address_info(AddressInfo::Construct()
                               .set_service("3000")
                               .set_flags(AddressInfo::Flags::passive)
                               .set_port(3000));

    printer().object("addressInfo", address_info);

    TEST_ASSERT(address_info.list().count() > 0);

    Socket server = std::move(
      Socket(address_info.list().at(0).family())
        .set_option(SocketOption(
          S::Level::socket,
          S::NameFlags::socket_reuse_address | S::NameFlags::socket_reuse_port,
          true)));
    TEST_ASSERT(is_success());

    PRINTER_TRACE(printer(), "");
    printer().object("serverAddress", server_address);
    PRINTER_TRACE(printer(), "port is " + Ntos(server_address.port()));
    TEST_ASSERT(server.bind_and_listen(server_address).is_success());
    PRINTER_TRACE(printer(), "");
    Socket incoming = server.accept(server_address);
    TEST_ASSERT(is_success());
    PRINTER_TRACE(printer(), "");

    // receive the header
    var::Data incoming_data(4);

    incoming.read(View(incoming_data));
    PRINTER_TRACE(printer(), "");

    incoming.write("Hello\n");
    PRINTER_TRACE(printer(), "");

    return true;
  }

  bool socket_api_case() {

    Array<Mutex, 2> wait_server_mutex;
    wait_server_mutex.at(0).lock();

    Thread server_thread
      = Thread(Thread::Construct()
                 .set_argument(&wait_server_mutex)
                 .set_function([](void *args) -> void * {
                   Array<Mutex, 2> *wait_server_mutex
                     = reinterpret_cast<Array<Mutex, 2> *>(args);
                   wait_server_mutex->at(1).lock();

                   Socket server = std::move(
                     Socket(S::Family::inet, S::Type::stream, S::Protocol::ip)
                       .set_option(SocketOption(
                         S::Level::socket,
                         S::NameFlags::socket_reuse_address
                           | S::NameFlags::socket_reuse_port,
                         true)));

                   SocketAddress server_address
                     = SocketAddress4().set_port(3000);
                   server.bind_and_listen(server_address);

                   wait_server_mutex->at(0).lock();
                   wait_server_mutex->at(1).unlock();
                   Socket incoming = server.accept(server_address);

                   return nullptr;
                 }));

    while (wait_server_mutex.at(1).try_lock()) {
      // stay here until server is ready to accept sockets
      wait_server_mutex.at(1).unlock();
      wait(5_milliseconds);
    }

    // let the server accept sockets
    wait_server_mutex.at(0).unlock();
    Sched().yield();

    {
      bool is_server_error = server_thread.error().error_number() > 0;
      TEST_EXPECT(!is_server_error);
      if (is_server_error) {
        server_thread.join();
        return false;
      }
    }

    {
      // IpAddress4 address();
      // Socket s;
    }

    server_thread.join();

    return true;
  }

private:
};
