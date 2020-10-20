
#include <cstdio>

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

  static bool http_server(UnitTest *self) {

    AddressInfo address_info(AddressInfo::Construct()
                               .set_family(S::Family::inet)
                               .set_service(Ntos(self->m_server_port))
                               .set_type(Socket::Type::stream)
                               .set_flags(AddressInfo::Flags::passive));

    TEST_SELF_ASSERT(address_info.list().count() > 0);

    const SocketAddress &server_listen_address = address_info.list().at(0);
    Socket server_listen_socket
      = std::move(Socket(server_listen_address)
                    .set_option(SocketOption(
                      Socket::Level::socket,
                      Socket::NameFlags::socket_reuse_address))
                    .bind_and_listen(server_listen_address));

    SocketAddress accept_address;

    self->printer().object("listening", server_listen_address);
    self->m_is_listening = true;

    HttpServer(server_listen_socket.accept(accept_address))
      .listen(
        self,
        [](HttpServer *server, void *context, const Http::Request &request)
          -> Http::IsStop {
          // handle the request
          UnitTest *self = reinterpret_cast<UnitTest *>(context);

          self->printer().key(
            "requestMethod",
            Http::to_string(request.method()).string_view());

          const StringView hello_world = "Hello World";
          DataFile incoming;

          const bool is_connection_close
            = server->get_header_field("CONNECTION") == "CLOSE";

          printer().key("CONNECTION", server->get_header_field("CONNECTION"));

          switch (request.method()) {
          case Http::Method::null:
            server->receive(NullFile())
              .send(Http::Response(
                server->http_version(),
                Http::Status::bad_request));
            break;

          case Http::Method::get:
            server->receive(NullFile())
              .add_header_field("content-length", Ntos(hello_world.length()))
              .send(Http::Response(server->http_version(), Http::Status::ok))
              .send(ViewFile(View(hello_world)));

            break;

          case Http::Method::post:
            server->receive(incoming)
              .add_header_field("content-length", Ntos(incoming.size()))
              .send(Http::Response(server->http_version(), Http::Status::ok))
              .send(incoming.seek(0));

            break;

          case Http::Method::put:
            server->receive(incoming)
              .add_header_field("content-length", Ntos(incoming.size()))
              .send(Http::Response(server->http_version(), Http::Status::ok))
              .send(incoming.seek(0));
            break;

          case Http::Method::patch:
            server->receive(incoming)
              .add_header_field("content-length", Ntos(incoming.size()))
              .send(Http::Response(server->http_version(), Http::Status::ok))
              .send(incoming.seek(0));
            break;

          case Http::Method::delete_:
            break;
          case Http::Method::head:
            break;
          case Http::Method::options:
            break;
          case Http::Method::trace:
            break;
          }

          self->printer().key("close", is_connection_close);
          return is_connection_close ? Http::IsStop::yes : Http::IsStop::no;
        });

    self->printer().key("connection", StringView("--close--"));
    return true;
  }

  bool http_client_case() {
    randomize_server_port();

    {
      m_is_listening = false;
      PrinterObject po(printer(), "httpClient/Server");
      Thread server_thread = start_server(http_server);
      TEST_ASSERT(is_success());

      PRINTER_TRACE(printer(), "httpGet");
      HttpClient http_client;
      http_client.connect("localhost", m_server_port);

      {
        DataFile response;
        TEST_ASSERT(
          http_client.get("index.html", Http::Get().set_response(&response))
            .is_success());

        printer().key(
          "serverResponse",
          StringView(response.data().add_null_terminator()));
        TEST_ASSERT(
          StringView(response.data().add_null_terminator()) == "Hello World");
      }

      {

        DataFile response;
        DataFile request;
        request.data().copy(View(StringView("Special Request")));

        TEST_ASSERT(
          http_client
            .post(
              "index.html",
              Http::Post().set_request(&request).set_response(&response))
            .is_success());

        printer().key("request", request.data().to_string());
        printer().key("response", response.data().to_string());

        TEST_ASSERT(request.data() == response.data());
      }

      {

        DataFile response;
        DataFile request;
        request.data().copy(View(StringView("Special Request Post")));

        TEST_ASSERT(
          http_client.add_header_field("connection", "close")
            .post(
              "index.html",
              Http::Post().set_request(&request).set_response(&response))
            .is_success());

        printer().key("request", request.data().to_string());
        printer().key("response", response.data().to_string());

        TEST_ASSERT(request.data() == response.data());
      }

      wait(500_milliseconds);

      TEST_ASSERT(is_success());
    }

    {

      PrinterObject po(printer(), "http://httpbin.org/get");
      HttpClient http_client;

      printer().key("is", StringView("connecting"));
      TEST_ASSERT(http_client.connect("httpbin.org").is_success());

      {
        DataFile response;
        printer().key("is", StringView("getting"));
        TEST_EXPECT(
          http_client.get("/get", Http::ExecuteMethod().set_response(&response))
            .is_success());
        printer().key("response", response.data().add_null_terminator());
      }

      {
        printer().key("is", StringView("putting"));
        DataFile request;
        request.data().copy(View(StringView("HelloWorld")));
        DataFile response;
        TEST_EXPECT(
          http_client.put("/put", Http::ExecuteMethod().set_response(&response))
            .is_success());
        printer().key("response", response.data().add_null_terminator());
      }
    }

    if (1) {
      PrinterObject po(printer(), "ip.jsontest.com");

      DataFile response;
      TEST_ASSERT(HttpClient()
                    .connect("ip.jsontest.com")
                    .get("/", Http::ExecuteMethod().set_response(&response))
                    .is_success());

#if 0
      TEST_ASSERT(
        HttpSecureClient()
          .connect("ip.jsontest.com")
          .get("/", Http::ExecuteMethod().set_response(&(response.seek(0))))
          .is_success());
#endif
    }

    {
      PrinterObject po(printer(), "https://httpbin.org/get");
      HttpSecureClient http_client;

      TEST_ASSERT(http_client.connect("httpbin.org").is_success());

      {
        DataFile response;
        TEST_ASSERT(
          http_client.get("/get", Http::ExecuteMethod().set_response(&response))
            .is_success());
        if (response.size()) {
          printer().key("response", response.data().add_null_terminator());
        }
      }
    }

    {
      PrinterObject po(printer(), "https://httpbin.org/redirect");

      HttpClient http_client;
      TEST_ASSERT(http_client.connect("httpbin.org").is_success());
      {
        DataFile response;
        TEST_ASSERT(http_client
                      .get(
                        "/redirect-to?url=httpbin.org%2Fget&status_code=200",
                        Http::ExecuteMethod().set_response(&response))
                      .is_success());
        if (response.size()) {
          printer().key("response", response.data().add_null_terminator());
        }
      }
      return true;
    }

    {
      // https://github.com/StratifyLabs/StratifyAPI/blob/master/src/inet/Socket.cpp

      PrinterObject po(printer(), "https://github.com");
      HttpSecureClient http_client;

      TEST_ASSERT(http_client.connect("github.com").is_success());

      {
        DataFile response;
        TEST_ASSERT(
          http_client
            .get(
              "/StratifyLabs/StratifyAPI/blob/master/src/inet/Socket.cpp",
              Http::ExecuteMethod().set_response(&response))
            .is_success());
        TEST_ASSERT(response.size() > 0);
      }
    }

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
        .get_address_string()
        .string_view());
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
        .get_address_string()
        .string_view());
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
                               .set_protocol(Socket::Protocol::tcp)
                               .set_flags(AddressInfo::Flags::passive));

    self->printer().object("tcpAddress", address_info);

    TEST_SELF_ASSERT(address_info.list().count() > 0);

    const SocketAddress &server_address = address_info.list().at(0);
    Socket server
      = std::move(Socket(server_address.family(), server_address.type())
                    .set_option(SocketOption(
                      S::Level::socket,
                      S::NameFlags::socket_reuse_address,
                      true)));
    // api::ExecutionContext::reset_error();
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

    randomize_server_port();

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

  void randomize_server_port() {
    Random().seed().randomize(View(m_server_port));
    m_server_port = (m_server_port % (65535 - 49152)) + 49152;
  }
};
