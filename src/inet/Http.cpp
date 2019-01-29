/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "inet/Http.hpp"
#include "var.hpp"
#include "sys.hpp"
#include "inet/Url.hpp"

#define SHOW_HEADERS 0

using namespace inet;

Http::Http(Socket & socket) : m_socket(socket){
}

HttpClient::HttpClient(Socket & socket) : Http(socket){
	m_transfer_size = 1024;
	m_is_chunked_transfer_encoding = false;
	m_is_keep_alive = false;
}

int HttpClient::get(const var::ConstString & url, const sys::File & response, const sys::ProgressCallback * progress_callback){
	return query("GET", url, 0, &response, progress_callback);
}

int HttpClient::post(const var::ConstString & url, const var::ConstString & request, const sys::File & response, const sys::ProgressCallback * progress_callback){
	DataFile request_file;
	request_file.data().copy_cstring(request.cstring());
	return post(url, request_file, response, progress_callback);
}

int HttpClient::post(const var::ConstString & url, const sys::File & request, const sys::File & response, const sys::ProgressCallback * progress_callback){
	return query("POST", url, &request, &response, progress_callback);
}

int HttpClient::put(const var::ConstString & url, const var::ConstString & request, const sys::File & response, const sys::ProgressCallback * progress_callback){
	DataFile request_file;
	request_file.data().copy_cstring(request.cstring());
	return put(url, request_file, response, progress_callback);
}

int HttpClient::put(const var::ConstString & url, const sys::File & data, const sys::File & response, const sys::ProgressCallback * progress_callback){
	return query("PUT", url, &data, &response, progress_callback);
}

int HttpClient::patch(const var::ConstString & url, const var::ConstString & request, const sys::File & response, const sys::ProgressCallback * progress_callback){
	DataFile request_file;
	request_file.data().copy_cstring(request.cstring());
	return patch(url, request_file, response, progress_callback);
}

int HttpClient::patch(const var::ConstString & url, const sys::File & data, const sys::File & response, const sys::ProgressCallback * progress_callback){
	return query("PATCH", url, &data, &response, progress_callback);
}

int HttpClient::head(const var::ConstString & url){

	return 0;
}

int HttpClient::remove(const var::ConstString & request, const var::String & data){

	return 0;
}


int HttpClient::query(const var::ConstString & command,
							 const var::ConstString & url,
							 const sys::File * send_file,
							 const sys::File * get_file,
							 const sys::ProgressCallback * progress_callback){
	m_status_code = -1;

	m_content_length = -1;
	int result;
	Url u(url);

	result = connect_to_server(u.domain_name(), u.port());
	if( result < 0 ){
		return result;
	}

	result = send_header(command, u.domain_name(), u.path(), send_file, progress_callback);
	if( result < 0 ){
		return result;
	}

	if( send_file ){
		//send the file on the socket
		socket().write(*send_file, m_transfer_size);
	}

	listen_for_header();
	if( get_file ){
		listen_for_data(*get_file, progress_callback);
	}

	if( is_keep_alive() == false ){
		close_connection();
	}

	return 0;

}


int HttpClient::send_string(const var::ConstString & str){
	if( !str.is_empty() ){
		return socket().write(str.str(), str.length());
	}
	return 0;
}


int HttpClient::close_connection(){
	return socket().close();
}


int HttpClient::connect_to_server(const var::ConstString & domain_name, u16 port){
	SocketAddressInfo address_info;

	if( (socket().fileno() >= 0) && is_keep_alive() ){
		//already connected
		if( m_alive_domain == domain_name ){
			return 0;
		} else {
			m_header.format("socket is 0x%X, domain is %s", socket().fileno(), m_alive_domain.cstring());
			set_error_number(FAILED_WRONG_DOMAIN);
			return -1;
		}
	}

	m_alive_domain.clear();
	var::Vector<SocketAddressInfo> address_list = address_info.fetch_node(domain_name);
	if( address_list.count() > 0 ){
		m_address = address_list.at(0);
		m_address.set_port(port);

		if( socket().create(m_address)  < 0 ){
			set_error_number(FAILED_TO_CREATE_SOCKET);
			return -1;
		}

		if( socket().connect(m_address) < 0 ){
			set_error_number(FAILED_TO_CONNECT_TO_SOCKET);
			socket().close();
			return -1;
		}
		m_alive_domain = domain_name;
		return 0;
	}

	set_error_number(FAILED_TO_FIND_ADDRESS);
	return -1;
}

int HttpClient::build_header(const var::ConstString & method, const var::ConstString & host, const var::ConstString & path, u32 length){
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
		m_header << "Content-Length: " << String().format(F32U, length) << "\r\n";
	}
	m_header << "\r\n";

	return 0;
}

int HttpClient::send_header(const var::ConstString & method,
									 const var::ConstString & host,
									 const var::ConstString & path,
									 const sys::File * file,
									 const sys::ProgressCallback * progress_callback){


	u32 data_length = file != 0 ? file->size() : 0;

	build_header(method, host, path, data_length);

#if SHOW_HEADERS
	printf(">> %s", m_header.str());
#endif

	if( socket().write(m_header) != (int)m_header.length() ){
		set_error_number(FAILED_TO_WRITE_HEADER);
		return -1;
	}

	if( file ){
		if( socket().write(*file, m_transfer_size, file->size(), progress_callback) < 0 ){
			set_error_number(FAILED_TO_WRITE_DATA);
			return -1;
		}
	}

	return 0;
}


int HttpClient::listen_for_header(){

	var::String line;
	m_header_response_pairs.clear();
	do {
		line = socket().gets('\n');
		if( line.length() > 2 ){

			m_header << line;
#if SHOW_HEADERS
			printf("> %s", line.str());
#endif

			HttpHeaderPair pair = HttpHeaderPair::from_string(line);
			m_header_response_pairs.push_back(pair);

			String title = pair.key();
			title.to_upper();

			if( title == "HTTP/1.1" ){
				m_status_code = pair.value().atoi();
				if( (m_status_code >= 300) && (m_status_code < 400) ){
					//redirect
				}
			}

			if( title == "CONTENT-LENGTH" ){
				m_content_length = pair.value().atoi();
			}
			if( title == "TRANSFER-ENCODING" ){
				m_transfer_encoding = pair.value();
				m_transfer_encoding.to_upper();
			}
		}


	} while( line.length() > 2 ); //while reading the header

	return -1;
}

int HttpClient::listen_for_data(const sys::File & file, const sys::ProgressCallback * progress_callback){
	if( m_transfer_encoding == "CHUNKED" ){
		u32 bytes_incoming = 0;
		do {
			String line = socket().gets();
			//convert line from hex
			bytes_incoming = line.to_unsigned_long(16);

			//read bytes_incoming from the socket and write it to the output file
			if( file.write(socket(), bytes_incoming, bytes_incoming) != (int)bytes_incoming ){
				set_error_number(FAILED_TO_WRITE_INCOMING_DATA_TO_FILE);
				return -1;
			}

			//need to call the progress callback -- what is the total?

		} while( bytes_incoming > 0 );

	} else {
		//read the response from the socket
		file.write(socket(), m_transfer_size, m_content_length, progress_callback);
	}
	return 0;
}

HttpHeaderPair HttpHeaderPair::from_string(const var::ConstString & string){
	var::Tokenizer tokens(string, ": \t\r\n");
	if( tokens.count() >= 2 ){
		return HttpHeaderPair(tokens.at(0), tokens.at(1));
	}
	return HttpHeaderPair();
}

