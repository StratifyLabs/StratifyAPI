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

var::String Http::to_string(Method status) {
  var::String result;
  switch (status) {
    API_HANDLE_METHOD_CASE(invalid);
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
  return Method::invalid;
}

Http::Http(Socket &socket) : m_socket(socket) {}

HttpClient::HttpClient(Socket &socket) : Http(socket) {}

HttpClient &HttpClient::execute_method(
  Method method,
  var::StringView url,
  const ExecuteMethod &options) {
  m_status_code = -1;
  m_content_length = 0;
  int result;
  Url u(url);

  u32 get_file_pos = 0;
  if (options.response()) {
    get_file_pos = options.response()->location();
  }

  result = connect_to_server(u.domain_name().cstring(), u.port());
  if (result < 0) {
    return *this;
  }

  result = send_header(
    to_string(method).cstring(),
    u.domain_name().cstring(),
    u.path().cstring(),
    options.request(),
    options.progress_callback());

  if (result < 0) {
    return *this;
  }

#if 0
	if( send_file ){
		//send the file on the socket
		socket().write(*send_file, m_transfer_size, send_file->size(), progress_callback);
	}
#endif

  if (listen_for_header() < 0) {
    return *this;
  }
  bool is_redirected = false;

  if (
    is_follow_redirects()
    && ((status_code() == 301) || (status_code() == 302) || (status_code() == 303) || (status_code() == 307) || (status_code() == 308))) {
    is_redirected = true;
  }

  const api::ProgressCallback *callback = nullptr;
  // don't show the progress on the response if a file was transmitted
  if (options.progress_callback() == nullptr) {
    callback = options.progress_callback();
  }

  if (options.response() && (is_redirected == false)) {
    result = listen_for_data(options.response(), callback);
  } else {
    NullFile null_file;
    result = listen_for_data(&null_file, callback);
  }

  if (result < 0) {

    return *this;
  }

  if (is_redirected) {
    // close_connection();

    if (options.response()) {
      options.response()->seek(get_file_pos, File::Whence::set);
    }

    for (u32 i = 0; i < header_response_pairs().count(); i++) {

      String key = header_response_pairs().at(i).key();
      key.to_lower();
      if (key == "location") {
        return execute_method(
          method,
          header_response_pairs().at(i).value().cstring(),
          options);
      }
    }
  }

  if (is_keep_alive() == false) {
    // close_connection();
  }

  return *this;
}

int HttpClient::send_string(var::StringView str) {
  if (!str.is_empty()) {
    return socket().write(var::View(str)).return_value();
  }
  return 0;
}

int HttpClient::connect_to_server(var::StringView domain_name, u16 port) {

  if ((socket().fileno() >= 0) && is_keep_alive()) {
    // already connected
    if (m_alive_domain == domain_name) {
      return 0;
    } else {
      m_header.format(
        "socket is 0x%X, domain is %s",
        socket().fileno(),
        m_alive_domain.cstring());
      return -1;
    }
  }

  m_alive_domain.clear();

  AddressInfo address_info(
    AddressInfo::Construct().set_node(domain_name).set_port(port));

  if (address_info.list().count() > 0) {
    m_address = address_info.list().at(0);
    socket().connect(m_address);
    m_alive_domain = var::String(domain_name);
    return 0;
  }

  return -1;
}

void HttpClient::build_header(
  var::StringView method,
  var::StringView host,
  var::StringView path,
  u32 length) {

  bool is_user_agent_present = false;
  bool is_accept_present = false;
  bool is_keep_alive_present = false;
  m_header.clear();
  m_header += method + " " + path + " HTTP/1.1\r\n";
  m_header += String("Host: ") + host + "\r\n";

  for (u32 i = 0; i < header_request_pairs().count(); i++) {
    String key = header_request_pairs().at(i).key();
    if (key.is_empty() == false) {
      String entry = key + ": " + header_request_pairs().at(i).value() + "\r\n";
      m_header += entry;
      key.to_lower();
      if (key == "user-Agent") {
        is_user_agent_present = true;
      }
      if (key == "accept") {
        is_accept_present = true;
      }
      if (key == "connection") {
        is_keep_alive_present = true;
      }
    }
  }

  if (!is_keep_alive_present) {
    if (is_keep_alive()) {
      m_header += "Connection: keep-alive\r\n";
    }
  }
  if (!is_user_agent_present) {
    m_header += "User-Agent: StratifyOS\r\n";
  }
  if (!is_accept_present) {
    m_header += "Accept: */*\r\n";
  }

  if (length > 0) {
    m_header += String("Content-Length: ") + String::number(length) + "\r\n";
  }
  m_header += "\r\n";
}

int HttpClient::send_header(
  var::StringView method,
  var::StringView host,
  var::StringView path,
  fs::File *file,
  const api::ProgressCallback *progress_callback) {

  u32 data_length = file ? file->size() : 0;

  build_header(method, host, path, data_length);

  AGGREGATE_TRAFFIC(String(">> ") + m_header);
#if SHOW_HEADERS
  printf(">> %s", m_header.cstring());
  printf("Sending %lu data bytes\n", file ? file->size() : 0);
#endif

  if (socket().write(m_header).is_error()) {
    return -1;
  }

  if (file) {
    if (socket()
          .write(
            *file,
            fs::File::Write()
              .set_page_size(m_transfer_size)
              .set_size(file->size())
              .set_progress_callback(progress_callback))
          .is_error()) {
      return -1;
    }
  }

  return 0;
}

int HttpClient::listen_for_header() {

  var::String line;
  header_response_pairs().clear();
  bool is_first_line = true;
  m_transfer_encoding = "";
  socket().reset_error();

  do {
    line = socket().gets('\n');
    if (line.length() > 2) {

      m_header += line;
      AGGREGATE_TRAFFIC(String("> ") + line);
#if SHOW_HEADERS
      printf("> %s", line.cstring());
#endif

      header_response_pairs().push_back(
        Http::HeaderPair::from_string(line.cstring()));
      const Http::HeaderPair &pair = header_response_pairs().back();
      String title = pair.key();
      title.to_upper();

      if (title.find("HTTP/") == 0) {
        StringList tokens = title.split(" ");
        is_first_line = false;
        if (tokens.count() < 2) {
          m_status_code = -1;
          return -1;
        }
        m_status_code = String(tokens.at(1)).to_integer();
      }

      if (title == "CONTENT-LENGTH") {
        m_content_length = static_cast<unsigned int>(pair.value().to_integer());
      }

      if (title == "CONTENT-TYPE") {
        // check for evnt streams
        StringList tokens = pair.value().split(" ;");
        if (String(tokens.at(0)) == "text/event-stream") {
          m_content_length = static_cast<u32>(
            -1); // accept data until the operation is cancelled
        }
      }

      if (title == "TRANSFER-ENCODING") {
        m_transfer_encoding = pair.value();
        m_transfer_encoding.to_upper();
      }
    }

  } while (
    (line.length() > 2 || is_first_line)
    && (socket().is_success() == 0)); // while reading the header

  if (socket().is_error()) {
    return -1;
  }

  return 0;
}

int HttpClient::listen_for_data(
  fs::File *destination,
  const api::ProgressCallback *progress_callback) {
  if (m_transfer_encoding == "CHUNKED") {
    u32 bytes_incoming = 0;
    do {
      String line = socket().gets();
      // convert line from hex
      bytes_incoming = line.to_unsigned_long(String::Base::hexidecimal);

      // read bytes_incoming from the socket and write it to the output file
      destination->write(
        socket(),
        fs::File::Write()
          .set_page_size(bytes_incoming)
          .set_size(bytes_incoming));

      // need to call the progress callback -- what is the total?

    } while ((bytes_incoming > 0) && (destination->is_success()));

  } else {
    // read the response from the socket
    if (m_content_length > 0) {

      destination->write(
        socket(),
        fs::File::Write()
          .set_page_size(m_transfer_size)
          .set_size(m_content_length)
          .set_progress_callback(progress_callback));

      if (destination->is_error()) {
        return -1;
      }
    }
  }
  return 0;
}

Http::HeaderPair Http::HeaderPair::from_string(var::StringView string) {
  const String string_copy = var::String(string);
  const size_t colon_pos = string_copy.find(":");

  const String key = string_copy
                       .get_substring(
                         String::GetSubstring().set_length(colon_pos))
                       .to_upper();
  String value;
  if (colon_pos != String::npos) {
    value = string_copy.get_substring(
      String::GetSubstring().set_position(colon_pos + 1));
    if (value.at(0) == ' ') {
      value.pop_front();
    }
    value.replace(String::Replace().set_old_string("\r"))
      .replace(String::Replace().set_old_string("\n"));
  }
  return Http::HeaderPair(key.cstring(), value.cstring());
}

int HttpServer::run() {

  // read socket data

  while (is_running()) {
    const var::String line = socket().gets();

    if (line.length() > 0) {
      StringList tokens = line.split(" ");
      if (tokens.count() < 3) {
        // bad request
        send_bad_request();
      } else {
        const Method method = method_from_string(tokens.at(0));
        if (method == Method::invalid) {
          send_bad_request();
        } else {
          u32 content_length = 0;
          set_transfer_encoding_chunked(false);
          var::String header;
          do {
            header = socket().gets();
            if (header.length() > 2) {
              header_request_pairs().push_back(
                Http::HeaderPair::from_string(header.cstring()));

              const Http::HeaderPair &pair = header_request_pairs().back();
              if (pair.key() == "CONTENT-LENGTH") {
                content_length = pair.value().to_integer();
              } else if (
                pair.key() == "TRANSFER-ENCODING"
                && String(pair.value()).to_upper() == "CHUNKED") {
                set_transfer_encoding_chunked();
              }
            }
          } while (header.length() > 2);

          printf("response with %d bytes incoming\n", content_length);
          respond(method, tokens.at(1), content_length);
        }
      }

      // execute the method
    }
  }
  return 0;
}

void HttpServer::send_bad_request() {
  socket().write(m_version + to_string(Status::bad_request) + "\r\n");
}

int HttpServer::send_header(Status status) {

  printf("sending '%s'\n", to_string(status).cstring());
  if (socket()
        .write(m_version + to_string(status) + "\r\n")
        .is_error()) {
    return -1;
  }

  for (const auto &pair : header_response_pairs()) {
    const auto s = pair.to_string();
    printf("Sending response pair %s\n", s.cstring());
    if (socket().write(s + "\r\n").return_value() != s.length() + 2) {
      printf("failed to write pair %s\n", s.cstring());
    }
  }

  if (socket().write("\r\n").return_value() != 2) {
    printf("Failed to write terminator\n");
  }

  return 0;
}

int HttpServer::send_chunk(var::View chunk) {
  socket().write(var::String().format("%d\r\n", chunk.size()));
  socket().write(chunk);
  socket().write("\r\n");
  return chunk.size();
}

int HttpServer::get_chunk_size() {
  String line = socket().gets();
  return line.to_integer();
}

int HttpServer::send(var::View chunk) {
  return socket().write(chunk).return_value();
}

int HttpServer::receive(fs::File &file, int content_length) {
  if (is_transfer_encoding_chunked()) {
    // read chunk by chunk
    int bytes_received = 0;
    while (bytes_received < content_length) {
      int chunk_size = get_chunk_size();

      if (chunk_size <= 0) {
        return bytes_received;
      }

      file.write(
        socket(),
        fs::File::Write()
          .set_location(bytes_received)
          .set_page_size(512)
          .set_size(chunk_size));

      bytes_received += chunk_size;

      socket().gets(); // read the \r\n at the end
    }

    return bytes_received;
  }

  // write the bytes to the file
  return file
    .write(
      socket(),
      fs::File::Write().set_location(0).set_page_size(512).set_size(
        content_length))
    .return_value();
}
