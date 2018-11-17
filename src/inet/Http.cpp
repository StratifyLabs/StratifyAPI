/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "var.hpp"
#include "sys.hpp"
#include "inet/Http.hpp"
#include "inet/Url.hpp"

using namespace inet;

Http::Http(Socket & socket) : m_socket(socket){

}

HttpClient::HttpClient(Socket & socket) : Http(socket){
	m_transfer_size = 1024;
	m_is_chunked_transfer_encoding = false;
}

int HttpClient::get(const var::ConstString & url, const sys::File & response){
	return query("GET", url, 0, &response);
}

int HttpClient::post(const var::ConstString & url, const var::ConstString & request, const sys::File & response){
	DataFile request_file;
	request_file.data().copy_cstring(request.cstring());
	return post(url, request_file, response);
}



int HttpClient::post(const var::ConstString & url, const sys::File & data, const sys::File & response){
	return query("POST", url, &data, &response);
}

int HttpClient::put(const var::ConstString & url, const var::ConstString & request, const sys::File & response){
	DataFile request_file;
	request_file.data().copy_cstring(request.cstring());
	return put(url, request_file, response);
}

int HttpClient::put(const var::ConstString & url, const sys::File & data, const sys::File & response){
	return query("PUT", url, &data, &response);
}

int HttpClient::patch(const var::ConstString & url, const var::ConstString & request, const sys::File & response){
	DataFile request_file;
	request_file.data().copy_cstring(request.cstring());
	return patch(url, request_file, response);
}

int HttpClient::patch(const var::ConstString & url, const sys::File & data, const sys::File & response){
	return query("PATCH", url, &data, &response);
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
							 const sys::File * get_file){
	int result;
	Url u(url);
	result = connect_to_server(u.domain_name(), u.port());
	if( result < 0 ){
		return result;
	}

	result = send_header(command, u.domain_name(), u.path(), send_file);
	if( result < 0 ){
		return result;
	}

	if( send_file ){
		//send the file on the socket
		socket().write(*send_file, m_transfer_size);
	}

	listen_for_header(m_header_response);
	if( get_file ){
		listen_for_data(*get_file);
	}
	close_connection();

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

	var::Vector<SocketAddressInfo> address_list = address_info.fetch_node(domain_name);
	if( address_list.count() > 0 ){
		m_address = address_list.at(0);
		m_address.set_port(port);
		if( socket().create(m_address)  < 0 ){
			return -1;
		}

		if( socket().connect(m_address) < 0 ){
			return -1;
		}

		return 0;
	}


	return -1;
}

int HttpClient::build_header(const var::ConstString & method, const var::ConstString & host, const var::ConstString & path, u32 length){
	bool is_user_agent_present = false;
	bool is_accept_present = false;
	m_header.clear();
	m_header << method << " " << path << " HTTP/1.1\r\n";
	m_header << "Host: " << host << "\r\n";

	for(u32 i = 0; i < header_request_fields().count(); i++){
		m_header << header_request_fields().at(i) << "\r\n";
		if( header_request_fields().at(i).find("User-Agent:") == 0 ){ is_user_agent_present = true; }
		if( header_request_fields().at(i).find("Accept:") == 0 ){ is_accept_present = true; }
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
									 const sys::File * file){


	u32 data_length = file != 0 ? file->size() : 0;

	build_header(method, host, path, data_length);

	printf(">> %s", m_header.str());

	if( socket().write(m_header) != m_header.length() ){
		return -1;
	}

	if( file ){
		if( socket().write(*file, m_transfer_size) < 0 ){
			return -1;
		}
	}

	return 0;
}


int HttpClient::listen_for_header(var::String & response){

	var::String line;
	do {
		line = socket().gets('\n');
		if( line.length() > 2 ){

			m_header << line;

			printf("> %s\n", line.str());

			var::Token line_tokens(line, ": \t\r\n");
			var::String title = line_tokens.at(0);
			var::String value;
			if( line_tokens.count() > 1 ){
				value = line_tokens.at(1);
			}

			var::String rebuild_line;
			rebuild_line << title << ": " << value;
			m_header_response_fields.push_back(rebuild_line);

			title.to_upper();
			value.to_upper();

			if( title == "HTTP/1.1" ){ m_status_code = value.atoi(); }
			if( title == "CONTENT-LENGTH" ){ m_content_length = value.atoi(); }
			if( title == "CONNECTION" ){ value == "CLOSE" ? m_is_keep_alive = false : m_is_keep_alive = true; }
			if( title == "TRANSFER-ENCODING" ){ m_transfer_encoding = value; }
		}


	} while( line.length() > 2 ); //while reading the header

	return -1;
}

int HttpClient::listen_for_data(const sys::File & file){
	if( m_transfer_encoding == "CHUNKED" ){
		u32 bytes_incoming = 0;
		do {
			String line = socket().gets();
			//convert line from hex
			bytes_incoming = line.to_unsigned_long(16);

			//read bytes_incoming from the socket and write it to the output file
			if( file.write(socket(), bytes_incoming, bytes_incoming) != bytes_incoming ){
				printf("failed to transfer %d\n", bytes_incoming);
				return -1;
			}

		} while( bytes_incoming > 0 );

	} else {
		//read the response from the socket
		file.write(socket(), m_content_length, m_content_length);
	}
	return 0;
}

