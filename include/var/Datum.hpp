#ifndef DATUM_HPP
#define DATUM_HPP


#include "String.hpp"
#include "Json.hpp"
#include "../chrono/ClockTime.hpp"
#include "../chrono/Time.hpp"

namespace var {
class Datum : public api::VarInfoObject
{
public:
	Datum();
	Datum(const var::ConstString & key, const var::ConstString & value);

	String & key(){ return m_key; }
	const String & key() const { return m_key; }

	String & value(){ return m_value; }
	const String & value() const { return m_value; }

	String & origin(){ return m_origin; }
	const String & origin() const { return m_origin; }

	JsonObject & metadata(){ return m_metadata; }
	const JsonObject & metadata() const { return m_metadata; }

	const chrono::ClockTime & clock_time() const { return m_clocktime; }
	const chrono::Time & time() const { return m_time; }

	void refresh_timestamp();

	var::JsonObject to_json() const;

private:
	String m_key;
	String m_origin;
	String m_value;
	JsonObject m_metadata;
	chrono::ClockTime m_clocktime;
	chrono::Time m_time;

	String m_id;
};




}



#endif // DATUM_HPP
