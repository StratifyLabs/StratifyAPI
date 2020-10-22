/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#include "inet/Http.hpp"
#include "fs.hpp"
#include "inet/Url.hpp"
#include "printer.hpp"
#include "var.hpp"

#define SHOW_HEADERS 1
#if defined __link
#define AGGREGATE_TRAFFIC(msg) (m_traffic += msg)
#else
#define AGGREGATE_TRAFFIC(msg)
#endif

using namespace inet;

#define API_HANDLE_STATUS_CASE(c)                                              \
  case Status::c:                                                              \
    result = StackString64().format("%d %s", Status::c, MCU_STRINGIFY(c));     \
    break

var::StackString64 Http::to_string(Status status) {
  var::StackString64 result;
  switch (status) {
    API_HANDLE_STATUS_CASE(null);
    API_HANDLE_STATUS_CASE(continue_);
    API_HANDLE_STATUS_CASE(switching_protocols);
    API_HANDLE_STATUS_CASE(processing);
    API_HANDLE_STATUS_CASE(early_hints);
    API_HANDLE_STATUS_CASE(ok);
    API_HANDLE_STATUS_CASE(created);
    API_HANDLE_STATUS_CASE(accepted);
    API_HANDLE_STATUS_CASE(non_authoritative_information);
    API_HANDLE_STATUS_CASE(no_content);
    API_HANDLE_STATUS_CASE(reset_content);
    API_HANDLE_STATUS_CASE(partial_content);
    API_HANDLE_STATUS_CASE(multi_status);
    API_HANDLE_STATUS_CASE(already_reported);
    API_HANDLE_STATUS_CASE(im_used);
    API_HANDLE_STATUS_CASE(multiple_choices);
    API_HANDLE_STATUS_CASE(moved_permanently);
    API_HANDLE_STATUS_CASE(found);
    API_HANDLE_STATUS_CASE(see_other);
    API_HANDLE_STATUS_CASE(not_modified);
    API_HANDLE_STATUS_CASE(use_proxy);
    API_HANDLE_STATUS_CASE(switch_proxy);
    API_HANDLE_STATUS_CASE(temporary_redirect);
    API_HANDLE_STATUS_CASE(permanent_redirect);
    API_HANDLE_STATUS_CASE(bad_request);
    API_HANDLE_STATUS_CASE(unauthorized);
    API_HANDLE_STATUS_CASE(payment_required);
    API_HANDLE_STATUS_CASE(forbidden);
    API_HANDLE_STATUS_CASE(not_found);
    API_HANDLE_STATUS_CASE(method_not_allowed);
    API_HANDLE_STATUS_CASE(not_acceptable);
    API_HANDLE_STATUS_CASE(proxy_authentication_required);
    API_HANDLE_STATUS_CASE(request_timeout);
    API_HANDLE_STATUS_CASE(conflict);
    API_HANDLE_STATUS_CASE(gone);
    API_HANDLE_STATUS_CASE(length_required);
    API_HANDLE_STATUS_CASE(precondition_failed);
    API_HANDLE_STATUS_CASE(payload_too_large);
    API_HANDLE_STATUS_CASE(uri_too_long);
    API_HANDLE_STATUS_CASE(unsupported_media_type);
    API_HANDLE_STATUS_CASE(range_not_satisfiable);
    API_HANDLE_STATUS_CASE(expectation_failed);
    API_HANDLE_STATUS_CASE(misdirected_request);
    API_HANDLE_STATUS_CASE(unprocessable_entity);
    API_HANDLE_STATUS_CASE(locked);
    API_HANDLE_STATUS_CASE(failed_dependency);
    API_HANDLE_STATUS_CASE(too_early);
    API_HANDLE_STATUS_CASE(upgrade_required);
    API_HANDLE_STATUS_CASE(precondition_required);
    API_HANDLE_STATUS_CASE(too_many_requests);
    API_HANDLE_STATUS_CASE(request_header_fields_too_large);
    API_HANDLE_STATUS_CASE(unavailable_for_legal_reasons);
    API_HANDLE_STATUS_CASE(internal_server_error);
    API_HANDLE_STATUS_CASE(not_implemented);
    API_HANDLE_STATUS_CASE(bad_gateway);
    API_HANDLE_STATUS_CASE(service_unavailable);
    API_HANDLE_STATUS_CASE(gateway_timeout);
    API_HANDLE_STATUS_CASE(http_version_not_supported);
    API_HANDLE_STATUS_CASE(variant_also_negociates);
    API_HANDLE_STATUS_CASE(insufficient_storage);
    API_HANDLE_STATUS_CASE(loop_detected);
    API_HANDLE_STATUS_CASE(not_extended);
    API_HANDLE_STATUS_CASE(network_authentication_required);
  }

  result(
    StackString64::Replace().set_old_character('_').set_new_character(' '));

  return result;
}

#define API_HANDLE_METHOD_CASE(c)                                              \
  case Method::c:                                                              \
    result = KeyString(MCU_STRINGIFY(c)).to_upper();                           \
    break

var::KeyString Http::to_string(Method method) {
  var::KeyString result;
  switch (method) {
    API_HANDLE_METHOD_CASE(null);
    API_HANDLE_METHOD_CASE(get);
    API_HANDLE_METHOD_CASE(post);
    API_HANDLE_METHOD_CASE(put);
    API_HANDLE_METHOD_CASE(head);
    API_HANDLE_METHOD_CASE(delete_);
    API_HANDLE_METHOD_CASE(patch);
    API_HANDLE_METHOD_CASE(options);
    API_HANDLE_METHOD_CASE(trace);
  }

  return std::move(result);
}

Http::Method Http::method_from_string(var::StringView string) {
  const var::KeyString input = KeyString(string).to_upper();
  if (StringView(input.cstring()) == "GET") {
    return Method::get;
  } else if (StringView(input.cstring()) == "POST") {
    return Method::post;
  } else if (StringView(input.cstring()) == "PUT") {
    return Method::put;
  } else if (StringView(input.cstring()) == "HEAD") {
    return Method::head;
  } else if (StringView(input.cstring()) == "DELETE") {
    return Method::delete_;
  } else if (StringView(input.cstring()) == "PATCH") {
    return Method::patch;
  } else if (StringView(input.cstring()) == "OPTIONS") {
    return Method::options;
  }
  return Method::null;
}

Http::Http(var::StringView http_version) : m_http_version(http_version) {
  API_ASSERT(http_version.find("HTTP/") == 0);
}

void Http::add_header_field(var::StringView key, var::StringView value) {
  if (m_is_header_dirty) {
    m_header_fields.clear();
    m_is_header_dirty = false;
  }

  (m_header_fields += key + ": " + value + "\r\n").to_upper();
}

void Http::add_header_fields(var::StringView fields) {
  if (m_is_header_dirty) {
    m_header_fields.clear();
    m_is_header_dirty = false;
  }
  (m_header_fields += fields).to_upper();
}

var::StringView Http::get_header_field(var::StringView key) const {

  const size_t key_position
    = header_fields().find(StackString64(key).to_upper());
  if (key_position == StringView::npos) {
    return var::StringView();
  }

  const size_t value_position = header_fields().find(":", key_position);
  if (value_position == StringView::npos) {
    return var::StringView();
  }

  const size_t end_position = header_fields().find("\r", value_position);
  if (end_position == StringView::npos) {
    return var::StringView();
  }

  const size_t adjusted_value_position
    = (header_fields().at(value_position + 1) == ' ') ? value_position + 2
                                                      : value_position + 1;

  return header_fields()(StringView::GetSubstring()
                           .set_position(adjusted_value_position)
                           .set_length(end_position - adjusted_value_position));
}

void Http::send(const Response &response) const {
  socket().write(
    response.to_string() + "\r\n" + header_fields() + "\r\n"
    + (header_fields().is_empty() ? "\r\n" : ""));
}

void Http::send(
  const fs::File &file,
  const api::ProgressCallback *progress_callback) const {
  if (is_transfer_encoding_chunked()) {
    const size_t size = file.size();
    for (size_t i = 0; i < size; i++) {
      const size_t page_size
        = transfer_size() > (size - i) ? size - i : transfer_size();
      socket()
        .write(var::String().format("%d\r\n", page_size))
        .write(
          file,
          Socket::Write().set_page_size(page_size).set_size(page_size))
        .write("\r\n");
      i += transfer_size();
      API_RETURN_IF_ERROR();
    }
    return;
  }

  socket().write(
    file,
    Socket::Write()
      .set_page_size(transfer_size())
      .set_progress_callback(progress_callback));
}

void Http::send(const Request &request) const {
  socket().write(request.to_string() + "\r\n" + header_fields() + "\r\n");
}

int Http::get_chunk_size() const {
  String line = socket().gets();
  return line.to_integer();
}

var::String Http::receive_header_fields() {
  var::String result;
  var::String line;

  do {
    line = std::move(socket().gets('\n'));

    AGGREGATE_TRAFFIC(String("> ") + line);
#if SHOW_HEADERS
    printf("> %s", line.cstring());
#endif
    if (line.length() > 2) {
      result += line;
      const Http::HeaderField attribute = HeaderField::from_string(line);

      if (attribute.key() == "CONTENT-LENGTH") {
        m_content_length
          = static_cast<unsigned int>(attribute.value().to_integer());
      }

      if (attribute.key() == "CONTENT-TYPE") {
        // check for evnt streams
        StringViewList tokens = attribute.value().split(" ;");
        if (KeyString(tokens.at(0)).to_upper() == "TEXT/EVENT-STREAM") {
          // accept data until the operation is cancelled
          m_content_length = static_cast<u32>(-1);
        }
      }

      if (
        attribute.key() == "TRANSFER-ENCODING"
        && (attribute.value() == "CHUNKED")) {
        m_is_transfer_encoding_chunked = true;
      }
    }

  } while (line.length() > 2
           && (socket().is_success())); // while reading the header

  m_is_header_dirty = true;
  return std::move(result.to_upper());
}

void Http::receive(
  const fs::File &file,
  const api::ProgressCallback *progress_callback) const {

  if (is_transfer_encoding_chunked()) {
    // read chunk by chunk
    int chunk_size = 0;
    do {
      chunk_size = get_chunk_size();
      int bytes_received = 0;
      file.write(
        socket(),
        fs::File::Write()
          .set_progress_callback(progress_callback)
          .set_location(bytes_received)
          .set_page_size(transfer_size())
          .set_size(chunk_size));

      bytes_received += chunk_size;
      socket().gets(); // read the \r\n at the end
      API_RETURN_IF_ERROR();
    } while (chunk_size);

    return;
  }

  // write the bytes to the file
  file.write(
    socket(),
    fs::File::Write().set_page_size(512).set_size(m_content_length));
}

HttpClient::HttpClient(var::StringView http_version) : Http(http_version) {}

HttpClient &HttpClient::execute_method(
  Method method,
  var::StringView path,
  const ExecuteMethod &options) {

  if (
    (m_is_connected == false) && (m_host.is_empty() == true)
    && connect(m_host).is_error()) {
    return *this;
  }

  m_content_length = 0;
  int get_file_pos = 0;
  if (options.response()) {
    get_file_pos = options.response()->location();
  }

  add_header_field("Host", m_host);

  if (get_header_field("accept").is_empty()) {
    add_header_field("Accept", "*/*");
  }

  if (get_header_field("user-agent").is_empty()) {
    add_header_field("User-Agent", "StratifyAPI");
  }

  if (get_header_field("connection").is_empty()) {
    add_header_field("connection", "keep-alive");
  }

  if (options.request()) {
    add_header_field("content-length", NumberString(options.request()->size()));
  }

  send(Request(method, path, http_version()));

  if (options.request()) {
    send(*options.request(), options.progress_callback());
  }

  m_response = Response(socket().gets());
  set_header_fields(receive_header_fields());
  API_RETURN_VALUE_IF_ERROR(*this);

  if (KeyString(get_header_field("Connection")).to_upper() == "CLOSE") {
    renew_socket();
    m_is_connected = false;
  }

  const bool is_redirected = m_is_follow_redirects && m_response.is_redirect();

  if (m_content_length || is_transfer_encoding_chunked()) {
    if (options.response() && (is_redirected == false)) {
      receive(*options.response(), options.progress_callback());
    } else {
      receive(NullFile(), options.progress_callback());
    }
  }

  API_RETURN_VALUE_IF_ERROR(*this);

  if (is_redirected) {

    if (options.response()) {
      options.response()->seek(get_file_pos, File::Whence::set);
    }

    var::StringView location = get_header_field("LOCATION");
    if (location.is_empty() == false) {

      if (location.find("/") != 0) {
        // connect to another server
      }

      return execute_method(method, location, options);
    }
  }

  return *this;
}

HttpClient &HttpClient::connect(var::StringView domain_name, u16 port) {
  AddressInfo address_info(
    AddressInfo::Construct()
      .set_node(domain_name)
      .set_service(port != 0xffff ? StringView(NumberString(port)) : StringView(""))
      .set_family(Socket::Family::inet)
      .set_flags(AddressInfo::Flags::canon_name));

  for (const SocketAddress &address : address_info.list()) {
    renew_socket();
    socket().connect(address);
    if (is_success()) {
      m_is_connected = true;
      m_host = String(domain_name);
      return (*this);
    }
    API_RESET_ERROR();
  }

  API_RETURN_VALUE_ASSIGN_ERROR(
    *this,
    var::GeneralString(domain_name).cstring(),
    ECONNREFUSED);
}

Http::HeaderField Http::HeaderField::from_string(var::StringView string) {
  const size_t colon_pos = string.find(":");

  const KeyString key = std::move(
    KeyString(string.get_substring_with_length(colon_pos)).to_upper());

  String value;
  if (colon_pos != String::npos) {
    value = String(string.get_substring_at_position(colon_pos + 1));
    if (value.at(0) == ' ') {
      value.pop_front();
    }

    value.replace(String::Replace().set_old_string("\r"))
      .replace(String::Replace().set_old_string("\n"))
      .to_upper();
  }
  return std::move(Http::HeaderField(var::String(key.cstring()), value));
}

HttpServer &HttpServer::listen(
  void *context,
  IsStop (*respond)(
    HttpServer *server_self,
    void *context,
    const Http::Request &request)) {

  // read socket data

  bool is_stop = false;
  while (is_stop == false) {
    m_request = Request(socket().gets());
    set_header_fields(receive_header_fields());
    // execute the method
    if (respond) {
      if (respond(this, context, m_request) == IsStop::yes) {
        is_stop = true;
      }
    }
  }

  return *this;
}
