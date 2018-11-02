#ifndef HTTP_HPP
#define HTTP_HPP

#include "../api/InetObject.hpp"
#include "../var/String.hpp"
#include "Socket.hpp"

namespace inet {

class Http : public api::InetWorkObject {
public:
	Http(Socket & socket);


protected:
	Socket & socket(){ return m_socket; }

private:
	//server name
	//port

	//socket to use
	Socket & m_socket;

};


class HttpClient : public Http {
public:
	HttpClient(Socket & socket);

	int head(const var::ConstString & url);
	int get(const var::ConstString & url);
	int post(const var::ConstString & url, const var::String & data);
	int put(const var::ConstString & url, const var::String & data);
	int patch(const var::ConstString & url, const var::String & data);

	//http delete
	int remove(const var::ConstString & url, const var::String & data);

	int options(const var::ConstString & url);
	int trace(const var::ConstString & url);
	int connect(const var::ConstString & url);


	/*! \details Returns a reference to the header that is returned
	  * by the request.
	  *
	  */
	const var::String & header() const { return m_header; }

	/*! \details Returns a reference to the response that is returned
	  * by the request.
	  *
	  *
	  */
	const var::String & response() const { return m_response; }


	/*! \details Handles incoming data if the response size exceeds maximum_response_size().
	  *
	  * @return Return true to continue the request and false to abort.
	  *
	  */
	virtual bool handle_incoming_data(){ return true; }

	/*! \details Sets the maximum amount of data that can be allocated
	  * for the response.
	  *
	  * If the response exceeds maximum data size, handle_incoming_data() will
	  * be called. The application should implement handle_incoming_data() to
	  * store the data (e.g. to a file). If handle_incoming_data() returns false,
	  * the request will be aborted.
	  *
	  */
	void set_maximum_response_size(u32 value){
		m_maximum_response_size = value;
	}

	/*! \details Returns the maximum number of bytes that can be allocated for the response
	  * to any request.
	  *
	  * See set_maximum_response_size() for details.
	  *
	  */
	u32 maximum_response_size() const { return m_maximum_response_size; }

	int close_connection();

private:

	int connect_to_server(const var::ConstString & domain_name, u16 port);

	int query(const var::ConstString & command,
				 const var::ConstString & url,
				 const var::String & data = var::String());

	int send_string(const var::ConstString & str);

	int send_header(const var::ConstString & method,
						 const var::ConstString & host,
						 const var::ConstString & path,
						 const var::String & data);

	int listen_for_header(var::String & response);
	int listen_for_data(var::String & response);

	SocketAddress m_address;

	u32 m_maximum_response_size;
	var::String m_header;
	var::String m_response;
	int m_reply;

	int m_content_length;
	bool m_is_keep_alive;

};

class HttpServer : public Http {
public:
	HttpServer();


private:

	//path to filesystem
	//callback for dynamic content

	//design to be easily used in a thread


};

}

#endif // HTTP_HPP
