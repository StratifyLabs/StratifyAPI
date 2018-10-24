#include "var.hpp"
#include "inet/Http.hpp"
#include "inet/Url.hpp"

using namespace inet;

Http::Http(Socket & socket) : m_socket(socket){

}

HttpClient::HttpClient(Socket & socket) : Http(socket){

}

int HttpClient::get(const var::ConstString & url){
	printf("Get %s\n", url.str());
	return query("GET", url);
}

int HttpClient::post(const var::ConstString & url, const var::Data & data){
	return query("POST", url, data);
}

int HttpClient::put(const var::ConstString & url, const var::Data & data){
	return query("PUT", url, data);
}

int HttpClient::patch(const var::ConstString & url, const var::Data & data){
	return query("PATCH", url, data);
}

int HttpClient::head(const var::ConstString & url){

	return 0;
}

int HttpClient::remove(const var::ConstString & request, const var::Data & data){

	return 0;
}


int HttpClient::query(const var::ConstString & command, const var::ConstString & url, const var::Data & data){
	int result;

	Url u(url);

	printf("Connect to server %s %d\n", u.domain_name().str(), u.port());
	result = connect_to_server(u.domain_name(), u.port());
	if( result < 0 ){ return result; }

	printf("Send header\n");
	result = send_header(command, u.domain_name(), u.path(), data);
	if( result < 0 ){
		printf("Failed to send header\n");
		return result;
	}

	m_response.fill(0);
	printf("Listen for header\n");
	listen_for_header(m_response);



	printf("Listen for data\n");
	listen_for_data(m_response);
	printf("Close connection\n");
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

int HttpClient::send_header(const var::ConstString & method, const var::ConstString & host, const var::ConstString & path, const var::Data & data){

	m_header.clear();
	m_header << method << " " << path << " HTTP/1.1\r\n";
	m_header << "Host: " << host << "\r\n";
	m_header << "User-Agent: StratifyOS\r\n";
	m_header << "Accept: */*\r\n";
	if( data.size() > 0 ){
		m_header << "Content-Length: " << String().format(F32U, data.size()) << "\r\n";
	}
	m_header << "\r\n";

	if( data.size() > 0 ){
		m_header << String(data.to_char());
	}

	printf(">> %s\n", m_header.str());

	if( socket().write(m_header) != (int)m_header.length() ){
		socket().close();
		return -1;
	}

	return 0;
}


int HttpClient::listen_for_header(var::String & response){
	m_header.set_size(1024);

	int response_size = socket().read(m_header);

	if( (response_size > 0) && (response_size < (int)m_header.capacity()) ){
		m_header.cdata()[response_size] = 0; //make sure m_header is null terminated


		u32 pos = 0;
		var::String line;
		do {
			u32 end_of_line = m_header.find("\r\n", pos);
			line = m_header.substr(pos, end_of_line - pos);
			pos = end_of_line + 2;

			if( !line.is_empty() ){

				printf("> %s\n", line.str());

				var::Token line_tokens(line, ": \t");
				var::String title = line_tokens.at(0);
				var::String value;
				if( line_tokens.count() > 1 ){
					value = line_tokens.at(1);
				}

				title.to_upper();
				value.to_upper();

				if( title == "HTTP/1.1" ){
					m_reply = value.atoi();
				}
				if( title == "CONTENT-LENGTH" ){ m_content_length = value.atoi(); }
				if( title == "CONNECTION" ){ value == "CLOSE" ? m_is_keep_alive = false : m_is_keep_alive = true; }
			} else {
				response.set_size(response_size - pos);
				memcpy(response.data(), (char*)m_header.data() + pos, response.size());
				pos = var::String::npos;
			}

		} while( pos != var::String::npos );

		return pos;
	}

	return -1;
}

int HttpClient::listen_for_data(var::String & response){
	int result = 0;
	u32 last_size;
	while( (response.size() < m_content_length) && (result > 0) ){
		last_size = response.size();
		//make room for a packet
		response.set_size( response.size() + 256 );
		int result = socket().read(response.cdata() + last_size, response.capacity() - last_size);
		if( result > 0 ){
			response.set_size( response.size() + result );
		}
	}

	return 0;
}

