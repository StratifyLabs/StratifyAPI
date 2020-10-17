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
    result = String().format("%d %s", Status::c, MCU_STRINGIFY(c));            \
    break

var::String Http::to_string(Status status) {
  var::String result;
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

  result.replace(
    String::Replace().set_old_string("_").set_new_string(" "));

  return result;
}

#define API_HANDLE_METHOD_CASE(c)                                              \
  case Method::c:                                                              \
    result = String(MCU_STRINGIFY(c)).to_upper();                              \
    break

var::String Http::to_string(Method method) {
  var::String result;
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

  return result;
}

Http::Method Http::method_from_string(const var::String &string) {
  const var::String input = String(string).to_upper();
  if (input == "GET") {
    return Method::get;
  } else if (input == "POST") {
    return Method::post;
  } else if (input == "PUT") {
    return Method::put;
  } else if (input == "HEAD") {
    return Method::head;
  } else if (input == "DELETE") {
    return Method::delete_;
  } else if (input == "PATCH") {
    return Method::patch;
  } else if (input == "OPTIONS") {
    return Method::options;
  }
  return Method::null;
}

Http::Http(const Socket &socket) : m_socket(socket) {}

void Http::send(const Response &response) const {
  socket().write(response.to_string() + "\r\n");
  for (const auto &pair : m_response_attribute_list) {
    socket().write(pair.to_string() + "\r\n");
  }
  socket().write("\r\n");
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
  socket().write(request.to_string() + "\r\n");
  for (const auto &pair : m_request_attribute_list) {
    socket().write(pair.to_string() + "\r\n");
  }
  socket().write("\r\n");
}

int Http::get_chunk_size() const {
  String line = socket().gets();
  return line.to_integer();
}

void Http::receive_attributes(var::Vector<Attribute> &pair_list) {

  var::String line;
  pair_list.clear();
  bool is_first_line = true;
  socket().reset_error();

  do {
    line = socket().gets('\n');
    if (line.length() > 2) {

      m_header += line;
      AGGREGATE_TRAFFIC(String("> ") + line);
#if SHOW_HEADERS
      printf("> %s", line.cstring());
#endif

      pair_list.push_back(Http::Attribute::from_string(line));
      const Http::Attribute &pair = pair_list.back();
      const String title = std::move(String(pair.key()).to_upper());

      if (title == "CONTENT-LENGTH") {
        m_content_length = static_cast<unsigned int>(pair.value().to_integer());
      }

      if (title == "CONTENT-TYPE") {
        // check for evnt streams
        StringList tokens = pair.value().split(" ;");
        if (String(tokens.at(0)).to_upper() == "TEXT/EVENT-STREAM") {
          // accept data until the operation is cancelled
          m_content_length = static_cast<u32>(-1);
        }
      }

      if (
        title == "TRANSFER-ENCODING"
        && (String(pair.value()).to_upper() == "CHUNKED")) {
        m_is_transfer_encoding_chunked = true;
      }
    }

  } while ((line.length() > 2 || is_first_line)
           && (socket().is_success())); // while reading the header
}

void Http::receive(
  const fs::File &file,
  const api::ProgressCallback *progress_callback) const {

  if (is_transfer_encoding_chunked()) {
    // read chunk by chunk
    int bytes_received = 0;
    while (bytes_received < m_content_length) {
      int chunk_size = get_chunk_size();
      API_RETURN_IF_ERROR();
      file.write(
        socket(),
        fs::File::Write()
          .set_progress_callback(progress_callback)
          .set_location(bytes_received)
          .set_page_size(transfer_size())
          .set_size(chunk_size));

      bytes_received += chunk_size;
      socket().gets(); // read the \r\n at the end
    }

    return;
  }

  // write the bytes to the file
  file.write(
    socket(),
    fs::File::Write().set_page_size(512).set_size(m_content_length));
}

HttpClient::HttpClient(Socket &socket, var::StringView host, u16 port)
  : Http(socket) {
  if (host.is_empty() == false) {
    connect_to_server(host, port);
  }
}

HttpClient &HttpClient::execute_method(
  Method method,
  var::StringView path,
  const ExecuteMethod &options) {

  if (m_is_connected == false) {
    return *this;
  }

  m_content_length = 0;
  int get_file_pos = 0;
  if (options.response()) {
    get_file_pos = options.response()->location();
  }

  send(Request(method, path, version()));

  if (options.request()) {
    send(*options.request(), options.progress_callback());
  }

  m_response = Response(socket().gets());
  receive_attributes(m_response_attribute_list);
  API_RETURN_VALUE_IF_ERROR(*this);

  const bool is_redirected = m_is_follow_redirects && m_response.is_redirect();

  if (options.response() && (is_redirected == false)) {
    receive(*options.response(), options.progress_callback());
  } else {
    receive(NullFile(), options.progress_callback());
  }

  API_RETURN_VALUE_IF_ERROR(*this);

  if (is_redirected) {
    // close_connection();

    if (options.response()) {
      options.response()->seek(get_file_pos, File::Whence::set);
    }

    for (u32 i = 0; i < header_response_pairs().count(); i++) {

      String key = header_response_pairs().at(i).key();
      key.to_upper();
      if (key == "LOCATION") {
        return execute_method(
          method,
          header_response_pairs().at(i).value(),
          options);
      }
    }
  }

  return *this;
}

void HttpClient::connect_to_server(var::StringView domain_name, u16 port) {
  API_ASSERT(m_is_connected == false);

  m_is_connected = true;

  AddressInfo address_info(
    AddressInfo::Construct().set_node(domain_name).set_service(Ntos(port)));

  for (const SocketAddress &address : address_info.list()) {
    socket().connect(address);
    if (is_success()) {
      return;
    }
    API_RESET_ERROR();
  }

  API_RETURN_ASSIGN_ERROR(domain_name.cstring(), ECONNREFUSED);
}

Http::Attribute Http::Attribute::from_string(var::StringView string) {
  const size_t colon_pos = string.find(":");

  const String key = string.get_substring_with_length(colon_pos).to_upper();

  String value;
  if (colon_pos != String::npos) {
    value = string.get_substring_at_position(colon_pos + 1);
    if (value.at(0) == ' ') {
      value.pop_front();
    }

    value.replace(String::Replace().set_old_string("\r"))
      .replace(String::Replace().set_old_string("\n"));
  }
  return std::move(Http::Attribute(key.cstring(), value.cstring()));
}

HttpServer &HttpServer::listen(
  void *context,
  IsStop (*respond)(
    HttpServer *server_self,
    void *context,
    const Http::Request &request)) {

  // read socket data

  bool is_stop = false;
  while (is_stop) {
    const var::String line = socket().gets();
    if (line.length() > 0) {
      m_request = Request(line);
      receive_attributes(m_request_attribute_list);
      // execute the method
      if (respond) {
        if (respond(this, context, m_request) == IsStop::yes) {
          is_stop = true;
        }
      }
    }
  }

  return *this;
}
