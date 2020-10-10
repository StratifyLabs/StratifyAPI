/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef SAPI_INET_HTTP_HPP_
#define SAPI_INET_HTTP_HPP_

#include "api/api.hpp"

#include "Socket.hpp"
#include "fs/File.hpp"
#include "var/Array.hpp"
#include "var/String.hpp"

namespace inet {

class Http : public api::Object {
public:

  explicit Http(Socket &socket);

  enum class Status {
    Continue = 100,
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
    invalid,
    get,
    post,
    put,
    head,
    Delete,
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

  class HeaderPair : public var::Pair<var::String> {
  public:
    HeaderPair() {}
    HeaderPair(var::StringView key, var::StringView value)
      : var::Pair<var::String>(var::String(key), var::String(value)) {}

    static HeaderPair from_string(var::StringView string);
    var::String to_string() const { return key() + ": " + value(); }
  };

  var::Vector<HeaderPair> &header_request_pairs() {
    return m_header_request_pairs;
  }
  const var::Vector<HeaderPair> &header_request_pairs() const {
    return m_header_request_pairs;
  }

  var::Vector<HeaderPair> &header_response_pairs() {
    return m_header_response_pairs;
  }
  const var::Vector<HeaderPair> &header_response_pairs() const {
    return m_header_response_pairs;
  }

protected:
  Socket &socket() { return m_socket; }

private:
  Socket &m_socket;

  var::Vector<HeaderPair> m_header_request_pairs;
  var::Vector<HeaderPair> m_header_response_pairs;
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
  /*! \details Constructs a new HttpClient object.
   *
   * @param socket A reference to the socket to use
   *
   * The socket can be Socket for http or SecureSocket for https
   * connections. The OS build must support https for
   * SecureSocket to work correctly.
   *
   */
  explicit HttpClient(Socket &socket);

  /*! \details Keeps the connection alive between requests.
   *
   * @param value If true, the connection is kept alive between requests
   *
   *
   */
  void set_keep_alive(bool value = true) { m_is_keep_alive = value; }

  /*! \details Returns true if the connection should be kept alive. */
  bool is_keep_alive() const { return m_is_keep_alive; }

  void set_follow_redirects(bool value = true) {
    m_is_follow_redirects = value;
  }
  bool is_follow_redirects() const { return m_is_follow_redirects; }

  /*! \details Executes a HEAD request.
   *
   * @param url target URL for request.
   */
  int head(var::StringView url);

  /*! \details Executes an HTTP GET request.
   *
   * @param url The URL to get (like https://stratifylabs.co)
   * @param response An open file that can accept the data that is returned
   * @param progress_callback An optional callback that can be used to update
   * the user on the progress of the download
   *
   * If the URL uses https, then this object should be constructed
   * with a SecureSocket rather than a simple Socket.
   *
   * ```
   * #include <sapi/inet.hpp>
   * #include <sapi/sys.hpp>
   *
   * SecureSocket socket; //or just use Socket for http
   * HttpClient http_client(socket);
   * DataFile response(File::APPEND);
   * http.get("https://stratifylabs.co", response);
   * ```
   *
   */
  HttpClient &get(var::StringView url, const Get &options) {
    return execute_method(Method::get, url, options);
  }

  HttpClient &post(var::StringView url, const Post &options) {
    return execute_method(Method::post, url, options);
  }
  HttpClient &put(var::StringView url, const Put &options) {
    return execute_method(Method::put, url, options);
  }

  HttpClient &patch(var::StringView url, const Patch &options) {
    return execute_method(Method::patch, url, options);
  }

  // http delete
  HttpClient &remove(var::StringView url, const Remove &options) {
    return execute_method(Method::Delete, url, options);
  }

  /*! \cond */
  HttpClient &options(var::StringView url) {
    return execute_method(Method::options, url, ExecuteMethod());
  }
  HttpClient &trace(var::StringView url) {
    return execute_method(Method::trace, url, ExecuteMethod());
  }
  HttpClient &connect(var::StringView url) {
    // return query(Method::connect, url, MethodOptions());
    return *this;
  }

  HttpClient &execute_method(
    Method method,
    var::StringView url,
    const ExecuteMethod &options);

  /*! \endcond */

  /*! \details Returns a reference to the header that is returned
   * by the request.
   *
   */
  const var::String &header() const { return m_header; }

  /*! \details Returns the status code of the last request.
   *
   * The status code will be 200 for a successful request.
   *
   */
  int status_code() const { return m_status_code; }

  /*! \details Returns the current transfer chunk size. */
  u32 transfer_size() const { return m_transfer_size; }

  /*! \details Sets the transfer chunk size.
   *
   * @param value Transfer size in bytes
   *
   * This sets the maximum chunk size used when downloading a file. This amount
   * will be read from the socket then written to the file before
   * another chunkc is ready from the socket.
   *
   */
  Http &set_transfer_size(u32 value) {
    m_transfer_size = value;
    return *this;
  }

  /*! \details Sets the transfer encoding to chunked.
   *
   * @param value true if the transfer is to be chunked.
   *
   */
  Http &set_chunked_transfer_encoding_enabled(bool value = true) {
    m_is_chunked_transfer_encoding = value;
    return *this;
  }

  /*! \details Closes the socket.
   *
   * The connection is automatically closed unless set_keep_alive()
   * has been executed.
   *
   */
  int close_connection();

  const var::String &traffic() const { return m_traffic; }

private:
  /*! \cond */

  SocketAddress m_address;
  var::String m_transfer_encoding;
  var::String m_header;
  var::String m_alive_domain;
  int m_status_code = 0;
  int m_content_length = 0;
  bool m_is_keep_alive = false;
  bool m_is_follow_redirects = true;
  bool m_is_chunked_transfer_encoding = false;
  u32 m_transfer_size = 1024;
  var::String m_traffic;

  int connect_to_server(var::StringView domain_name, u16 port);

  int send_string(var::StringView str);

  class Header {
    API_AC(Header, var::String, method);
    API_AC(Header, var::String, host);
    API_AC(Header, var::String, path);
  };

  void build_header(
    var::StringView method,
    var::StringView host,
    var::StringView path,
    u32 length);

  int send_header(
    var::StringView method,
    var::StringView host,
    var::StringView path,
    fs::File *file,
    const api::ProgressCallback *progress_callback);

  int listen_for_header();
  int listen_for_data(
    fs::File *data,
    const api::ProgressCallback *progress_callback);
  /*! \endcond */
};

/*! \cond */
class HttpServer : public Http {
public:
  // socket should already have accepted a new connection
  HttpServer(var::StringView version, Socket &socket) : Http(socket) {
    m_version = "HTTP/" + version + " ";
  }

  int run();
  virtual int respond(Method method, const var::String &url, int bytes_incoming)
    = 0;

protected:
  int send_header(Status status);
  int send_chunk(var::View chunk);
  int receive(fs::File &file, int content_length);
  int send(var::View chunk);

private:
  API_AB(HttpServer, running, true);
  API_AB(HttpServer, transfer_encoding_chunked, true);
  var::Data m_incoming;
  var::String m_version;
  int get_chunk_size();

  void send_bad_request();
};
/*! \endcond */

} // namespace inet

#endif // SAPI_INET_HTTP_HPP_
