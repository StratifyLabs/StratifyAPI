#include "var.hpp"
#include "inet/Http.hpp"
#include "inet/Url.hpp"

using namespace inet;

Http::Http(Socket & socket) : m_socket(socket){

}

HttpClient::HttpClient(Socket & socket) : Http(socket){

}

int HttpClient::get(const var::ConstString & url){
	return query("GET", url);
}

int HttpClient::post(const var::ConstString & url, const var::String & data){
	return query("POST", url, data);
}

int HttpClient::post_file(const var::ConstString & url, const var::ConstString & path){
	return query_with_file("POST", url, path);
}

int HttpClient::put(const var::ConstString & url, const var::String & data){
	return query("PUT", url, data);
}

int HttpClient::patch(const var::ConstString & url, const var::String & data){
	return query("PATCH", url, data);
}

int HttpClient::head(const var::ConstString & url){

	return 0;
}

int HttpClient::remove(const var::ConstString & request, const var::String & data){

	return 0;
}


int HttpClient::query(const var::ConstString & command, const var::ConstString & url, const var::String & data){
	int result;
	Url u(url);
	result = connect_to_server(u.domain_name(), u.port());
	if( result < 0 ){
		return result;
	}

	result = send_header(command, u.domain_name(), u.path(), data);
	if( result < 0 ){
		return result;
	}

	m_response.fill(0);
	listen_for_header(m_response);
	listen_for_data(m_response);
	close_connection();

	return 0;

}

int HttpClient::query_with_file(const var::ConstString & method,
										  const var::ConstString & url,
										  const var::ConstString & file_path){

	sys::File f;
	if( f.open(file_path, sys::File::READONLY) < 0 ){ return -1; }

	u32 file_size = f.size();

	int result;
	Url u(url);
	result = connect_to_server(u.domain_name(), u.port());
	if( result < 0 ){
		f.close();
		return result;
	}

	build_header(method, u.domain_name(), u.path(), file_size);
	if( socket().write(m_header) != m_header.length() ){
		f.close();
		return -1;
	}

	//write the file in chunks
	var::Data data(1024); //what is the best chunk size??
	while( f.read(data) > 0 ){
		//if chunked socket().write(String().format("%x", data.size());
		socket().write(data);
		//if chunked socket().write("\r\n");
	}
	f.close();

	m_response.fill(0);
	listen_for_header(m_response);
	listen_for_data(m_response);
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

int HttpClient::send_header(const var::ConstString & method, const var::ConstString & host, const var::ConstString & path, const var::String & data){

	build_header(method, host, path, data.length());

	if( data.length() > 0 ){ m_header << data; }

	printf(">> %s\n", m_header.str());

	return socket().write(m_header);
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

int HttpClient::listen_for_data(var::String & response){
	if( m_transfer_encoding == "CHUNKED" ){
		u32 bytes_incoming = 0;
		do {
			String line = socket().gets();
			//convert line from hex
			bytes_incoming = line.to_unsigned_long(16);
			String chunk;
			if( chunk.set_size(bytes_incoming) < 0 ){
				return -1;
			}
			int result;
			int page_size = bytes_incoming;
			if( page_size ){
				do {
					chunk.clear();
					if( chunk.set_size(page_size) < 0 ){
						return -1;
					}
					result = socket().read(chunk);
					if( result > 0 ){
						page_size -= result;
						response << chunk;
					}
				} while( page_size > 0 );
			}

		} while( bytes_incoming > 0 );

	} else {
		if( response.set_size( m_content_length ) < 0 ){
			//memory allocation failed
			return -1;
		}
		return socket().read(response);
	}
	return 0;
}

