/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#include "inet/Http.hpp"
#include "var.hpp"
#include "sys.hpp"
#include "fs.hpp"
#include "inet/Url.hpp"

#define SHOW_HEADERS 0
#if defined __link
#define AGGREGATE_TRAFFIC(msg) (m_traffic << msg)
#else
#define AGGREGATE_TRAFFIC(msg)
#endif

using namespace inet;

Http::Http(Socket & socket) : m_socket(socket){
}

HttpClient::HttpClient(Socket & socket) : Http(socket){}

int HttpClient::get(const var::String & url,
						  ResponseFile response,
						  const sys::ProgressCallback * progress_callback
						  ){
	return query(
				"GET",
				url,
				SendFile(nullptr),
				GetFile(&response.argument()),
				progress_callback
				);
}

int HttpClient::post(
		const var::String & url,
		RequestString request,
		ResponseFile response,
		const sys::ProgressCallback * progress_callback
		){
	DataFile request_file;
	request_file.data().copy_contents(request.argument());
	return post(
				url,
				RequestFile(request_file),
				response,
				progress_callback
				);
}

int HttpClient::post(const var::String & url,
							RequestFile request,
							ResponseFile response,
							const sys::ProgressCallback * progress_callback
							){
	return query(
				"POST",
				url,
				SendFile(&request.argument()),
				GetFile(&response.argument()),
				progress_callback
				);
}

int HttpClient::put(const var::String & url,
						  RequestString request,
						  ResponseFile response,
						  const sys::ProgressCallback * progress_callback){
	DataFile request_file;

	request_file.data().copy_contents(request.argument());

	return put(
				url,
				RequestFile(request_file),
				response,
				progress_callback
				);
}

int HttpClient::put(const var::String & url,
						  RequestFile request,
						  ResponseFile response,
						  const sys::ProgressCallback * progress_callback
						  ){
	return query(
				"PUT",
				url,
				SendFile(&request.argument()),
				GetFile(&response.argument()),
				progress_callback
				);
}

int HttpClient::patch(const var::String & url,
							 RequestString request,
							 ResponseFile response,
							 const sys::ProgressCallback * progress_callback){
	DataFile request_file;
	request_file.data().copy_contents(request.argument());
	return patch(
				url,
				RequestFile(request_file),
				response,
				progress_callback);
}

int HttpClient::patch(const var::String & url, RequestFile data, ResponseFile response, const sys::ProgressCallback * progress_callback){
	return query(
				"PATCH",
				url,
				SendFile(&data.argument()),
				GetFile(&response.argument()),
				progress_callback);
}

int HttpClient::head(UrlEncodedString url){

	return 0;
}

int HttpClient::remove(
		const var::String & url,
		const var::String & data){

	return 0;
}


int HttpClient::query(const var::String & command,
							 const var::String & url,
							 SendFile send_file,
							 GetFile get_file,
							 const sys::ProgressCallback * progress_callback
							 ){
	m_status_code = -1;
	m_content_length = 0;
	int result;
	Url u(url);

	u32 get_file_pos = 0;
	if( get_file.argument() ){
		get_file_pos = static_cast<u32>(
					get_file.argument()->seek(
						fs::File::Location(0), File::whence_current
						)
					);
	}

	result = connect_to_server(u.domain_name(), u.port());
	if( result < 0 ){
		return result;
	}

	result = send_header(
				command,
				u.domain_name(),
				u.path(),
				send_file.argument(),
				progress_callback
				);
	if( result < 0 ){
		return result;
	}

#if 0
	if( send_file ){
		//send the file on the socket
		socket().write(*send_file, m_transfer_size, send_file->size(), progress_callback);
	}
#endif

	if( listen_for_header() < 0 ){
		return set_error_number_if_error(api::error_code_inet_failed_to_get_header);
	}
	bool is_redirected = false;

	if( is_follow_redirects() &&
		 (
			 (status_code() == 301) ||
			 (status_code() == 302) ||
			 (status_code() == 303) ||
			 (status_code() == 307) ||
			 (status_code() == 308) )
		 ){
		is_redirected = true;
	}

	const sys::ProgressCallback * callback = nullptr;
	//don't show the progress on the response if a file was transmitted
	if( send_file.argument() == nullptr ){
		callback = progress_callback;
	}

	if( get_file.argument() && (is_redirected == false)){
		result = listen_for_data(
					*(get_file.argument()),
					callback
					);
	} else {
		NullFile null_file;
		result = listen_for_data(
					null_file,
					callback
					);
	}


	if( result < 0 ){
		return -1;
	}

	if( is_redirected ){
		close_connection();

		if( get_file.argument() ){
			get_file.argument()->seek(
						fs::File::Location(
							static_cast<int>(get_file_pos)
							),
						File::whence_set
						);
		}

		for(u32 i=0; i < header_response_pairs().count(); i++){

			String key = header_response_pairs().at(i).key();
			key.to_lower();
			if( key == "location" ){
				return query(
							command,
							header_response_pairs().at(i).value(),
							send_file,
							get_file,
							progress_callback
							);
			}
		}

	}

	if( is_keep_alive() == false ){
		close_connection();
	}

	return 0;

}


int HttpClient::send_string(const var::String & str){
	if( !str.is_empty() ){
		return socket().write(
					str.cstring(),
					Socket::Size(str.length())
					);
	}
	return 0;
}


int HttpClient::close_connection(){
	return socket().close();
}


int HttpClient::connect_to_server(
		const var::String & domain_name,
		u16 port
		){
	SocketAddressInfo address_info;

	if( (socket().fileno() >= 0) && is_keep_alive() ){
		//already connected
		if( m_alive_domain == domain_name ){
			return 0;
		} else {
			m_header.format("socket is 0x%X, domain is %s", socket().fileno(), m_alive_domain.cstring());
			return set_error_number_if_error(api::error_code_inet_failed_wrong_domain);
		}
	}

	m_alive_domain.clear();

	var::Vector<SocketAddressInfo> address_list = address_info.fetch_node(domain_name);
	if( address_list.count() > 0 ){
		m_address = SocketAddress(address_list.at(0));
		m_address.set_port(port);

		if( socket().create(m_address)  < 0 ){
			return set_error_number_if_error(
						api::error_code_inet_failed_to_create_socket
						);
		}

		if( socket().connect(m_address) < 0 ){
			socket().close();
			return set_error_number_if_error(
						api::error_code_inet_failed_to_connect_to_socket
						);
		}
		m_alive_domain = domain_name;
		return 0;
	}

	m_header.format(
				"failed to find address with result (%d)",
				address_info.error_number()
				);

	return set_error_number_if_error(
				api::error_code_inet_failed_to_find_address
				);
}

int HttpClient::build_header(const var::String & method, const var::String & host, const var::String & path, u32 length){
	bool is_user_agent_present = false;
	bool is_accept_present = false;
	bool is_keep_alive_present = false;
	m_header.clear();
	m_header << method << " " << path << " HTTP/1.1\r\n";
	m_header << "Host: " << host << "\r\n";

	for(u32 i = 0; i < header_request_pairs().count(); i++){
		String key = header_request_pairs().at(i).key();
		if( key.is_empty() == false ){
			String entry;
			entry << key << ": " << header_request_pairs().at(i).value() << "\r\n";
			m_header << entry;
			key.to_lower();
			if( key == "user-Agent" ){ is_user_agent_present = true; }
			if( key == "accept" ){ is_accept_present = true; }
			if( key == "connection" ){ is_keep_alive_present = true; }
		}
	}

	if( !is_keep_alive_present ){
		if( is_keep_alive() ){
			m_header << "Connection: keep-alive\r\n";
		}
	}
	if( !is_user_agent_present ){ m_header << "User-Agent: StratifyOS\r\n"; }
	if( !is_accept_present ){ m_header << "Accept: */*\r\n"; }

	if( length > 0 ){
		m_header << "Content-Length: " << String::number(length) << "\r\n";
	}
	m_header << "\r\n";

	return 0;
}

int HttpClient::send_header(
		const var::String & method,
		const var::String & host,
		const var::String & path,
		const fs::File * file,
		const sys::ProgressCallback * progress_callback){


	u32 data_length = file ? file->size() : 0;

	build_header(method, host, path, data_length);

	AGGREGATE_TRAFFIC(String(">> ") + m_header);
#if SHOW_HEADERS
	printf(">> %s", m_header.cstring());
	printf("Sending %d data bytes\n", file ? file->size() : 0);
#endif

	if( socket().write(m_header) != static_cast<int>(m_header.length()) ){
		return set_error_number_if_error(api::error_code_inet_failed_to_write_header);
	}

	if( file ){
		if( socket().write(
				 *file,
				 fs::File::PageSize(m_transfer_size),
				 fs::File::Size(file->size()),
				 progress_callback
				 ) < 0 ){
			return set_error_number_if_error(api::error_code_inet_failed_to_write_data);
		}
	}

	return 0;
}


int HttpClient::listen_for_header(){

	var::String line;
	m_header_response_pairs.clear();
	bool is_first_line = true;
	m_transfer_encoding = "";
	socket().clear_error_number();
	do {
		line = socket().gets('\n');
		if( line.length() > 2 ){

			m_header << line;
			AGGREGATE_TRAFFIC(String("> ") + line);
#if SHOW_HEADERS
			printf("> %s", line.cstring());
#endif

			HttpHeaderPair pair = HttpHeaderPair::from_string(line);
			m_header_response_pairs.push_back(pair);

			String title = pair.key();
			title.to_upper();

			if( title.find("HTTP/") == 0 ){
				Tokenizer tokens(
							title,
							var::Tokenizer::Delimeters(" ")
							);
				is_first_line = false;
				if( tokens.count() < 2 ){
					m_status_code = -1;
					return set_error_number_if_error(api::error_code_inet_failed_to_get_status_code);
				}
				m_status_code = String(tokens.at(1)).to_integer();
			}

			if( title == "CONTENT-LENGTH" ){
				m_content_length = static_cast<unsigned int>(pair.value().to_integer());
			}

			if( title == "CONTENT-TYPE" ){
				//check for evnt streams
				Tokenizer tokens(
							pair.value(),
							var::Tokenizer::Delimeters(" ;")
							);
				if( String(tokens.at(0)) == "text/event-stream" ){
					m_content_length = static_cast<u32>(-1); //accept data until the operation is cancelled
				}
			}

			if( title == "TRANSFER-ENCODING" ){
				m_transfer_encoding = pair.value();
				m_transfer_encoding.to_upper();
			}
		}


	} while( (line.length() > 2 || is_first_line) && (socket().error_number() == 0) ); //while reading the header

	if( socket().error_number() != 0 ){
		return -1;
	}

	return 0;
}

int HttpClient::listen_for_data(
		const fs::File & destination,
		const sys::ProgressCallback * progress_callback
		){
	if( m_transfer_encoding == "CHUNKED" ){
		u32 bytes_incoming = 0;
		do {
			String line = socket().gets();
			//convert line from hex
			bytes_incoming = line.to_unsigned_long(String::base_16);

			//read bytes_incoming from the socket and write it to the output file
			if( destination.write(
					 socket(),
					 fs::File::PageSize(bytes_incoming),
					 fs::File::Size(bytes_incoming)
					 ) != static_cast<int>(bytes_incoming) ){
				return set_error_number_if_error(api::error_code_inet_failed_to_write_incoming_data_to_file);
			}

			//need to call the progress callback -- what is the total?

		} while( bytes_incoming > 0 );

	} else {
		//read the response from the socket
		if( m_content_length != 0 ){
			int result = destination.write(
						socket(),
						fs::File::PageSize(m_transfer_size),
						fs::File::Size(m_content_length),
						progress_callback
						);
			if( result != static_cast<int>(m_content_length) ){
				return -1;
			}
		}
	}
	return 0;
}

HttpHeaderPair HttpHeaderPair::from_string(
		const var::String & string
		){
	String string_copy = string;
	size_t colon_pos = string_copy.find(":");

	String key = string_copy.create_sub_string(
				String::Position(0),
				String::Length(colon_pos)
				);
	String value;
	if( colon_pos != String::npos ){
		value = string_copy.create_sub_string(
					String::Position(colon_pos+1)
					);
		if( value.at(0) == ' ' ){
			value.erase(
						String::Position(0),
						String::Length(1)
						);
		}
		value.replace(
					String::ToErase("\r"),
					String::ToInsert("")
					);
		value.replace(
					String::ToErase("\n"),
					String::ToInsert("")
					);
	}
	return HttpHeaderPair(key, value);
}

