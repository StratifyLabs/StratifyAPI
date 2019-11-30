#ifndef SAPI_VAR_DATUM_HPP_
#define SAPI_VAR_DATUM_HPP_


#include "String.hpp"
#include "Json.hpp"
#include "../fs/File.hpp"
#include "../chrono/ClockTime.hpp"
#include "../chrono/Time.hpp"

namespace var {
class Datum : public api::InfoObject
{
public:
	Datum();
	Datum(const var::String & type, const var::String & key, const var::String & value);
	Datum(const JsonObject & json_object);

	String & type(){ return m_type; }
	const String & type() const { return m_type; }

	String & key(){ return m_key; }
	const String & key() const { return m_key; }

	String & value(){ return m_value; }
	const String & value() const { return m_value; }

	JsonObject & metadata(){ return m_metadata; }
	const JsonObject & metadata() const { return m_metadata; }

	chrono::ClockTime & clock_time(){ return m_clocktime; }
	const chrono::ClockTime & clock_time() const { return m_clocktime; }
	const chrono::Time & time() const { return m_time; }

	void refresh_timestamp();

	var::JsonObject to_json() const;

	String stringify() const;

	int save(const fs::File & f) const;

private:
	String m_type;
	String m_key;
	String m_value;
	JsonObject m_metadata;
	chrono::ClockTime m_clocktime;
	chrono::Time m_time;

	String m_id;
};




}



#endif // SAPI_VAR_DATUM_HPP_
