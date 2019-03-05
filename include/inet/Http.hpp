/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef HTTP_HPP
#define HTTP_HPP

#include "Socket.hpp"
#include "../api/InetObject.hpp"
#include "../var/String.hpp"
#include "../var/Array.hpp"
#include "../sys/ProgressCallback.hpp"

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

class HttpHeaderPair : public var::Pair<var::String> {
public:
	HttpHeaderPair(){}
	HttpHeaderPair(const var::ConstString & key, const var::ConstString & value) : var::Pair<var::String>(key, value){}

	static HttpHeaderPair from_string(const var::ConstString & string);

	var::String to_string() const {
		return var::String() << key() << ": " << value();
	}

};


class HttpClient : public Http {
public:
	HttpClient(Socket & socket);

	/*! \details Keeps the connection alive between requests.
	 *
	 * @param value If true, the connection is kept alive between requests
	 *
	 *
	 */
	void set_keep_alive(bool value = true){ m_is_keep_alive = value; }

	bool is_keep_alive() const { return m_is_keep_alive; }

	void set_follow_redirects(bool value = true){ m_is_follow_redirects = value; }
	bool is_follow_redirects() const { return m_is_follow_redirects; }

	/*! \details Executes a HEAD request.
	 *
	 * @param url target URL for request.
	 */
	int head(const var::ConstString & url);
	int get(const var::ConstString & url, const sys::File & response, const sys::ProgressCallback * progress_callback = 0);

	int post(const var::ConstString & url, const var::ConstString & request, const sys::File & response, const sys::ProgressCallback * progress_callback = 0);
	int post(const var::ConstString & url, const sys::File & request, const sys::File & response, const sys::ProgressCallback * progress_callback = 0);

	int put(const var::ConstString & url, const var::ConstString & request, const sys::File & response, const sys::ProgressCallback * progress_callback = 0);
	int put(const var::ConstString & url, const sys::File & request, const sys::File & response, const sys::ProgressCallback * progress_callback = 0);

	int patch(const var::ConstString & url, const var::ConstString & request, const sys::File & response, const sys::ProgressCallback * progress_callback = 0);
	int patch(const var::ConstString & url, const sys::File & request, const sys::File & response, const sys::ProgressCallback * progress_callback = 0);

	//http delete
	int remove(const var::ConstString & url, const var::String & data);
	int options(const var::ConstString & url);
	int trace(const var::ConstString & url);
	int connect(const var::ConstString & url);

	/*! \details List of values for error_number() when using the HttpClient class. */
	enum {
		NONE /*! No Errors */,
		FAILED_TO_CREATE_SOCKET /*! Failed to create a socket (1) */,
		FAILED_TO_CONNECT_TO_SOCKET /*! Failed to connect to socket (2) */,
		FAILED_TO_WRITE_HEADER /*! Failed to write request header (3) */,
		FAILED_TO_WRITE_DATA /*! Failed to write data (4) */,
		FAILED_TO_WRITE_INCOMING_DATA_TO_FILE /*! Failed  write incoming data to file provided (5) */,
		FAILED_TO_FIND_ADDRESS /*! Failed to find IP address of URL (6) */,
		FAILED_TO_GET_STATUS_CODE /*! Failed to get a status code in the HTTP response (7) */,
		FAILED_TO_GET_HEADER /*! Failed to receive the header (8) */,
		FAILED_WRONG_DOMAIN
	};

	/*! \details Returns a reference to the header that is returned
	  * by the request.
	  *
	  */
	const var::String & header() const { return m_header; }

	/*! \details Returns the status code of the last request.
	 *
	 * The status code will be 200 for a successful request.
	 *
	 */
	int status_code() const { return m_status_code; }

	/*! \details Returns the current transfer chunk size. */
	u32 transfer_size() const {
		return m_transfer_size;
	}

	/*! \details Sets the transfer chunk size.
	 *
	 * @param value Transfer size in bytes
	 *
	 * This sets the maximum chunk size used when downloading a file. This amount
	 * will be read from the socket then written to the file before
	 * another chunkc is ready from the socket.
	 *
	 */
	void set_transfer_size(u32 value){
		m_transfer_size = value;
	}

	/*! \details Sets the transfer encoding to chunked.
	 *
	 */
	void set_chunked_transfer_encoding_enabled(bool value = true){
		m_is_chunked_transfer_encoding = value;
	}

	/*! \details Closes the socket. */
	int close_connection();

	var::Vector<HttpHeaderPair> & header_request_pairs(){ return m_header_request_pairs; }
	const var::Vector<HttpHeaderPair> & header_request_pairs() const { return m_header_request_pairs; }

	var::Vector<HttpHeaderPair> & header_response_pairs(){ return m_header_response_pairs; }
	const var::Vector<HttpHeaderPair> & header_response_pairs() const { return m_header_response_pairs; }

private:

	/*! \cond */
	int connect_to_server(const var::ConstString & domain_name, u16 port);

	int query(const var::ConstString & command,
				 const var::ConstString & url,
				 const sys::File * send_file,
				 const sys::File * get_file,
				 const sys::ProgressCallback * progress_callback);


	int send_string(const var::ConstString & str);

	int build_header(const var::ConstString & method, const var::ConstString & host, const var::ConstString & path, u32 length);

	int send_header(const var::ConstString & method,
						 const var::ConstString & host,
						 const var::ConstString & path,
						 const sys::File * file,
						 const sys::ProgressCallback * progress_callback);

	int listen_for_header();
	int listen_for_data(const sys::File & file, const sys::ProgressCallback * progress_callback);
	/*! \endcond */

	SocketAddress m_address;

	var::String m_transfer_encoding;
	var::Vector<HttpHeaderPair> m_header_request_pairs;
	var::Vector<HttpHeaderPair> m_header_response_pairs;
	var::String m_header;
	int m_status_code;

	u32 m_content_length;
	bool m_is_keep_alive;
	var::String m_alive_domain;
	bool m_is_follow_redirects;

	bool m_is_chunked_transfer_encoding;
	u32 m_transfer_size;

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
