/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef SAPI_INET_HTTP_HPP_
#define SAPI_INET_HTTP_HPP_

#include "api/api.hpp"

#include "SecureSocket.hpp"
#include "Socket.hpp"
#include "fs/File.hpp"
#include "var/Array.hpp"
#include "var/String.hpp"
#include "var/Tokenizer.hpp"

namespace inet {

class Http : public api::ExecutionContext {
public:
  explicit Http(const Socket &socket);

  enum class IsStop { no, yes };

  enum class Status {
    null = 0,
    continue_ = 100,
    switching_protocols = 101,
    processing = 102,
    early_hints = 103,
    ok = 200,
    created = 201,
    accepted = 202,
    non_authoritative_information = 203,
    no_content = 204,
    reset_content = 205,
    partial_content = 206,
    multi_status = 207,
    already_reported = 208,
    im_used = 226,
    multiple_choices = 300,
    moved_permanently = 301,
    found = 302,
    see_other = 303,
    not_modified = 304,
    use_proxy = 305,
    switch_proxy = 306,
    temporary_redirect = 307,
    permanent_redirect = 308,
    bad_request = 400,
    unauthorized = 401,
    payment_required = 402,
    forbidden = 403,
    not_found = 404,
    method_not_allowed = 405,
    not_acceptable = 406,
    proxy_authentication_required = 407,
    request_timeout = 408,
    conflict = 409,
    gone = 410,
    length_required = 411,
    precondition_failed = 412,
    payload_too_large = 413,
    uri_too_long = 414,
    unsupported_media_type = 415,
    range_not_satisfiable = 416,
    expectation_failed = 417,
    misdirected_request = 421,
    unprocessable_entity = 422,
    locked = 423,
    failed_dependency = 424,
    too_early = 425,
    upgrade_required = 426,
    precondition_required = 428,
    too_many_requests = 429,
    request_header_fields_too_large = 431,
    unavailable_for_legal_reasons = 451,
    internal_server_error = 500,
    not_implemented = 501,
    bad_gateway = 502,
    service_unavailable = 503,
    gateway_timeout = 504,
    http_version_not_supported = 505,
    variant_also_negociates = 506,
    insufficient_storage = 507,
    loop_detected = 508,
    not_extended = 510,
    network_authentication_required = 511
  };

  enum class Method {
    null,
    get,
    post,
    put,
    head,
    delete_,
    patch,
    options,
    trace
  };

  class ExecuteMethod {
    API_AF(ExecuteMethod, fs::File *, response, nullptr);
    API_AF(ExecuteMethod, fs::File *, request, nullptr);
    API_AF(
      ExecuteMethod,
      const api::ProgressCallback *,
      progress_callback,
      nullptr);
  };

  using Get = ExecuteMethod;
  using Put = ExecuteMethod;
  using Patch = ExecuteMethod;
  using Post = ExecuteMethod;
  using Remove = ExecuteMethod;

  static var::String to_string(Status status);
  static var::String to_string(Method method);
  static Method method_from_string(const var::String &string);

  class Attribute : public var::Pair<var::String> {
  public:
    Attribute() {}
    Attribute(var::StringView key, var::StringView value)
      : var::Pair<var::String>(var::String(key), var::String(value)) {}

    static Attribute from_string(var::StringView string);
    var::String to_string() const { return key() + ": " + value(); }
  };

  var::Vector<Attribute> &header_request_pairs() {
    return m_request_attribute_list;
  }
  const var::Vector<Attribute> &header_request_pairs() const {
    return m_request_attribute_list;
  }

  var::Vector<Attribute> &header_response_pairs() {
    return m_response_attribute_list;
  }
  const var::Vector<Attribute> &header_response_pairs() const {
    return m_response_attribute_list;
  }

  class Request {
  public:
    Request() = default;
    Request(Method method, var::StringView path, var::StringView version)
      : m_method(method), m_path(path), m_version(version) {}

    explicit Request(var::StringView plain_test) {
      var::StringList list = plain_test.split(" \r");
      if (list.count() != 3) {
        API_RETURN_ASSIGN_ERROR("", EINVAL);
      }
      m_method = method_from_string(list.at(0));
      m_path = list.at(1);
      m_version = list.at(2);
    }

    var::String to_string() const {
      return std::move(
        Http::to_string(m_method) + " " + m_path + " " + m_version);
    }

  private:
    API_RAF(Request, Method, method, Method::null);
    API_RAC(Request, var::String, version);
    API_RAC(Request, var::String, path);
  };

  class Response {
  public:
    Response() = default;
    Response(var::StringView version, Status status)
      : m_status(status), m_version(version) {}
    explicit Response(var::StringView plain_test) {
      var::StringList list = plain_test.split(" \r");

      if (list.count() < 2) {
        API_RETURN_ASSIGN_ERROR("", EINVAL);
      }
      m_version = list.at(0);
      m_status = static_cast<Status>(list.at(1).to_integer());
    }

    var::String to_string() const {
      return std::move(m_version + " " + Http::to_string(m_status));
    }

    bool is_redirect() const { return (static_cast<int>(status()) / 100) == 3; }

  private:
    API_RAF(Response, Status, status, Status::null);
    API_RAC(Response, var::String, version);
  };

  /*! \details Returns a reference to the header that is returned
   * by the request.
   *
   */
  const var::String &header() const { return m_header; }
  const var::String &traffic() const { return m_traffic; }

protected:
  var::String m_header;
  var::String m_traffic;
  int m_content_length = 0;

  Request m_request;
  Response m_response;

  var::String m_header_request;

  var::Vector<Attribute> m_request_attribute_list;
  var::Vector<Attribute> m_response_attribute_list;

  const Socket &socket() const { return m_socket; }
  void send(const Response &response) const;
  void send(const Request &request) const;
  void receive_attributes(var::Vector<Attribute> &pair_list);

  void send(
    const fs::File &file,
    const api::ProgressCallback *progress_callback = nullptr) const;
  void receive(
    const fs::File &file,
    const api::ProgressCallback *progress_callback = nullptr) const;

private:
  API_AB(Http, transfer_encoding_chunked, false);
  API_AC(Http, var::String, version);
  API_AF(Http, size_t, transfer_size, 1024);
  const Socket &m_socket;

  int get_chunk_size() const;
  void send_chunk(var::View chunk) const;
};

template <class Derived> class HttpAccess : public Http {
public:
  HttpAccess(const Socket &socket) : Http(socket) {}
};

/*!
 * \brief The HTTP Client class
 * \details The HTTP client class
 * can be used to execute HTTP requests
 * to HTTP servers.
 *
 *
 *	This first example will download a file from
 * the internet.
 *
 * ```
 * #include <sapi/inet.hpp>
 * #include <sapi/sys.hpp>
 *
 * SecureSocket socket; //or just use Socket for http
 * HttpClient http_client(socket);
 * File response;
 * response.create("/home/response.html");
 * http.get("https://stratifylabs.co", response);
 * response.close();
 * ```
 *
 *
 */
class HttpClient : public Http {
public:
  HttpClient(Socket &socket) : Http(socket) {}
  HttpClient(Socket &socket, var::StringView host, u16 port);

  HttpClient &get(var::StringView path, const Get &options) {
    return execute_method(Method::get, path, options);
  }

  HttpClient &post(var::StringView path, const Post &options) {
    return execute_method(Method::post, path, options);
  }
  HttpClient &put(var::StringView path, const Put &options) {
    return execute_method(Method::put, path, options);
  }

  HttpClient &patch(var::StringView path, const Patch &options) {
    return execute_method(Method::patch, path, options);
  }

  // http delete
  HttpClient &remove(var::StringView path, const Remove &options) {
    return execute_method(Method::delete_, path, options);
  }

  /*! \cond */
  HttpClient &options(var::StringView path) {
    return execute_method(Method::options, path, ExecuteMethod());
  }
  HttpClient &trace(var::StringView path) {
    return execute_method(Method::trace, path, ExecuteMethod());
  }
  HttpClient &connect(var::StringView path) {
    // return query(Method::connect, path, MethodOptions());
    return *this;
  }

  HttpClient &execute_method(
    Method method,
    var::StringView path,
    const ExecuteMethod &options);

private:
  SocketAddress m_address;
  var::String m_alive_domain;
  bool m_is_keep_alive = false;
  bool m_is_follow_redirects = true;
  bool m_is_connected = false;

  void connect_to_server(var::StringView domain_name, u16 port);
};

/*! \cond */
class HttpServer : public Http {
public:
  // socket should already have accepted a new connection
  HttpServer(var::StringView version, const Socket &socket)
    : Http(socket), m_version(version) {
    API_ASSERT(version.find("HTTP/") == 0);
  }

  HttpServer &listen(
    void *context,
    IsStop (*respond)(
      HttpServer *server_self,
      void *context,
      const Http::Request &request));

  void send_header(Status status) const;

  const HttpServer &send(const Response &response) const {
    Http::send(response);
    return *this;
  }

  const HttpServer &send(
    const fs::File &file,
    const api::ProgressCallback *progress_callback = nullptr) const {
    Http::send(file, progress_callback);
    return *this;
  }

  const HttpServer &receive(
    const fs::File &file,
    const api::ProgressCallback *progress_callback = nullptr) const {
    Http::receive(file, progress_callback);
    return *this;
  }

protected:
private:
  API_AB(HttpServer, running, true);
  API_AC(HttpServer, var::String, version);

  var::Data m_incoming;
};

/*! \endcond */

} // namespace inet

#endif // SAPI_INET_HTTP_HPP_
