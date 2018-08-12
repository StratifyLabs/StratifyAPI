#ifndef HTTP_HPP
#define HTTP_HPP

#include "../api/InetObject.hpp"
#include "../var/String.hpp"
#include "Socket.hpp"

namespace inet {





class Http : public api::InetWorkObject {
public:
    Http();


private:
    //server name
    //port

    //socket to use

};


class HttpClient : public Http {
public:
    HttpClient();


    int head(const var::ConstString & url, var::String & response);
    int get(const var::ConstString & url, var::String & response);
    int post(const var::ConstString & url, const var::ConstString & data, var::String & response);
    int put(const var::ConstString & url, const var::ConstString & data, var::String & response);
    int patch(const var::ConstString & url, const var::ConstString & data, var::String & response);

    //http delete
    int remove(const var::ConstString & url, const var::ConstString & data, var::String & response);

    int options(const var::ConstString & url);
    int trace(const var::ConstString & url);
    int connect(const var::ConstString & url);


    const var::String & header() const { return m_header; }

private:


    var::String m_header;

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
