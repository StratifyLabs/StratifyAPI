/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_INET_HTTP_HPP_
#define SAPI_INET_HTTP_HPP_

#include "Socket.hpp"
#include "../api/InetObject.hpp"
#include "../var/String.hpp"
#include "../var/Array.hpp"
#include "../sys/ProgressCallback.hpp"

namespace inet {

class Http : public api::WorkObject {
public:

	using RequestFile = arg::Argument<const fs::File &, struct HttpSendRequestTag>;
	using ResponseFile = arg::Argument<const fs::File &, struct HttpGetResponseTag>;
	using UrlEncodedString = arg::Argument<const var::String &, struct HttpGetResponseUrlEncodedStringTag>;
	using RequestString = arg::Argument<const var::String &, struct HttpRequestStringTag>;

	Http(Socket & socket);

protected:
	Socket & socket(){ return m_socket; }

private:
	/*! \cond */
	Socket & m_socket;
	/*! \endcond */

};

class HttpHeaderPair : public var::Pair<var::String> {
public:
	HttpHeaderPair(){}
	HttpHeaderPair(const var::String & key, const var::String & value) : var::Pair<var::String>(key, value){}

	static HttpHeaderPair from_string(const var::String & string);

	var::String to_string() const {
		return var::String() << key() << ": " << value();
	}

};

/*!
 * \brief The HTTP Client class
 * \details The HTTP client class
 * can be used to execute HTTP requests
 * to HTTP servers.
 *
 *
 *	This first example will download a file from
 * the internet.
 *
 * ```
 * #include <sapi/inet.hpp>
 * #include <sapi/sys.hpp>
 *
 * SecureSocket socket; //or just use Socket for http
 * HttpClient http_client(socket);
 * File response;
 * response.create("/home/response.html");
 * http.get("https://stratifylabs.co", response);
 * response.close();
 * ```
 *
 *
 */
class HttpClient : public Http {
public:

	/*! \details Constructs a new HttpClient object.
	 *
	 * @param socket A reference to the socket to use
	 *
	 * The socket can be Socket for http or SecureSocket for https
	 * connections. The OS build must support https for
	 * SecureSocket to work correctly.
	 *
	 */
	HttpClient(Socket & socket);

	/*! \details Keeps the connection alive between requests.
	 *
	 * @param value If true, the connection is kept alive between requests
	 *
	 *
	 */
	void set_keep_alive(bool value = true){ m_is_keep_alive = value; }

	/*! \details Returns true if the connection should be kept alive. */
	bool is_keep_alive() const { return m_is_keep_alive; }

	void set_follow_redirects(bool value = true){ m_is_follow_redirects = value; }
	bool is_follow_redirects() const { return m_is_follow_redirects; }

	/*! \details Executes a HEAD request.
	 *
	 * @param url target URL for request.
	 */
	int head(UrlEncodedString url);

	/*! \details Executes an HTTP GET request.
	 *
	 * @param url The URL to get (like https://stratifylabs.co)
	 * @param response An open file that can accept the data that is returned
	 * @param progress_callback An optional callback that can be used to update the user on the progress of the download
	 *
	 * If the URL uses https, then this object should be constructed
	 * with a SecureSocket rather than a simple Socket.
	 *
	 * ```
	 * #include <sapi/inet.hpp>
	 * #include <sapi/sys.hpp>
	 *
	 * SecureSocket socket; //or just use Socket for http
	 * HttpClient http_client(socket);
	 * DataFile response(File::APPEND);
	 * http.get("https://stratifylabs.co", response);
	 * ```
	 *
	 */
	int get(
			const var::String & url,
			ResponseFile response,
			const sys::ProgressCallback * progress_callback = nullptr
			);

	int get(
			UrlEncodedString url,
			ResponseFile response,
			const sys::ProgressCallback * progress_callback = nullptr
			){
		return get(url.argument(), response, progress_callback);
	}

	int post(
			const var::String& url,
			RequestString request,
			ResponseFile response,
			const sys::ProgressCallback * progress_callback = nullptr
			);

	int post(
			UrlEncodedString url,
			RequestString request,
			ResponseFile response,
			const sys::ProgressCallback * progress_callback = nullptr
			){
		return post(url.argument(), request, response, progress_callback);
	}

	int post(
			const var::String & url,
			RequestFile request,
			ResponseFile response,
			const sys::ProgressCallback * progress_callback = nullptr
			);

	int post(
			UrlEncodedString url,
			RequestFile request,
			ResponseFile response,
			const sys::ProgressCallback * progress_callback = nullptr
			){
		return post(url.argument(), request, response, progress_callback);
	}

	int put(
			const var::String& url,
			RequestString request,
			ResponseFile response,
			const sys::ProgressCallback * progress_callback = nullptr
			);

	int put(
			UrlEncodedString url,
			RequestString request,
			ResponseFile response,
			const sys::ProgressCallback * progress_callback = nullptr
			){
		return put(url.argument(), request, response, progress_callback);
	}

	int put(
			const var::String& url,
			RequestFile request,
			ResponseFile response,
			const sys::ProgressCallback * progress_callback = nullptr
			);

	int put(
			UrlEncodedString url,
			RequestFile request,
			ResponseFile response,
			const sys::ProgressCallback * progress_callback = nullptr
			){
		return put(url.argument(), request, response, progress_callback);
	}

	int patch(
			const var::String& url,
			RequestString request,
			ResponseFile response,
			const sys::ProgressCallback * progress_callback = nullptr
			);

	int patch(
			UrlEncodedString url,
			RequestString request,
			ResponseFile response,
			const sys::ProgressCallback * progress_callback = nullptr
			){
		return patch(url.argument(), request, response, progress_callback);
	}

	int patch(
			const var::String& url,
			RequestFile request,
			ResponseFile response,
			const sys::ProgressCallback * progress_callback = nullptr
			);

	int patch(
			UrlEncodedString url,
			RequestFile request,
			ResponseFile response,
			const sys::ProgressCallback * progress_callback = nullptr
			){
		return patch(url.argument(), request, response, progress_callback);
	}

	//http delete
	/*! \cond */
	int remove(const var::String & url, const var::String & data);
	int options(const var::String & url);
	int trace(const var::String & url);
	int connect(const var::String & url);
	/*! \endcond */

	/*! \details List of values for error_number() when using the HttpClient class. */
	enum error {
		error_none /*! No Errors */,
		error_failed_to_create_socket /*! Failed to create a socket (1) */,
		error_failed_to_connect_to_socket /*! Failed to connect to socket (2) */,
		error_failed_to_write_header /*! Failed to write request header (3) */,
		error_failed_to_write_data /*! Failed to write data (4) */,
		error_failed_to_write_incoming_data_to_file /*! Failed  write incoming data to file provided (5) */,
		error_failed_to_find_address /*! Failed to find IP address of URL (6) */,
		error_failed_to_get_status_code /*! Failed to get a status code in the HTTP response (7) */,
		error_failed_to_get_header /*! Failed to receive the header (8) */,
		error_failed_wrong_domain
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
	Http & set_transfer_size(u32 value){
		m_transfer_size = value;
		return *this;
	}

	/*! \details Sets the transfer encoding to chunked.
	 *
	 * @param value true if the transfer is to be chunked.
	 *
	 */
	Http & set_chunked_transfer_encoding_enabled(bool value = true){
		m_is_chunked_transfer_encoding = value;
		return *this;
	}

	/*! \details Closes the socket.
	 *
	 * The connection is automatically closed unless set_keep_alive()
	 * has been executed.
	 *
	 */
	int close_connection();

	var::Vector<HttpHeaderPair> & header_request_pairs(){ return m_header_request_pairs; }
	const var::Vector<HttpHeaderPair> & header_request_pairs() const { return m_header_request_pairs; }

	var::Vector<HttpHeaderPair> & header_response_pairs(){ return m_header_response_pairs; }
	const var::Vector<HttpHeaderPair> & header_response_pairs() const { return m_header_response_pairs; }

	const var::String & traffic() const {
		return m_traffic;
	}

private:

	/*! \cond */

	using SendFile = arg::Argument<const fs::File *, struct HttpClientSendFile>;
	using GetFile = arg::Argument<const fs::File *, struct HttpClientSendFile>;

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
	var::String m_traffic;

	int connect_to_server(
			const var::String & domain_name,
			u16 port
			);

	int query(
			const var::String & command,
			const var::String & url,
			SendFile send_file,
			GetFile get_file,
			const sys::ProgressCallback * progress_callback
			);


	int send_string(const var::String & str);

	int build_header(const var::String & method,
						  const var::String & host,
						  const var::String & path,
						  u32 length
						  );

	int send_header(const var::String & method,
						 const var::String & host,
						 const var::String & path,
						 const fs::File * file,
						 const sys::ProgressCallback * progress_callback
						 );

	int listen_for_header();
	int listen_for_data(
			const fs::File & data,
			const sys::ProgressCallback * progress_callback
			);
	/*! \endcond */

};

/*! \cond */
class HttpServer : public Http {
public:
	HttpServer();


private:

	//path to filesystem
	//callback for dynamic content

	//design to be easily used in a thread


};
/*! \endcond */


}

#endif // SAPI_INET_HTTP_HPP_
