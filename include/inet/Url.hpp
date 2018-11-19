/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef URL_HPP
#define URL_HPP

#include "../api/InetObject.hpp"
#include "../var/String.hpp"

namespace inet {

class Url : public api::InetInfoObject {
public:
	enum {
		PROTOCOL_HTTPS,
		PROTOCOL_HTTP
	};

	Url(const var::ConstString & url = "");

	int set(const var::ConstString & url);
	var::String to_string() const;

	u16 port() const { return m_port; }
	u8 protocol() const { return m_protocol; }
	const var::String & domain_name() const { return m_domain_name; }
	const var::String & path() const { return m_path; }

	static var::String encode(const var::ConstString & input);
	static var::String decode(const var::ConstString & input);

private:
	var::String m_domain_name;
	var::String m_path;
	u8 m_protocol;
	u16 m_port;

};

}

#endif // URL_HPP
