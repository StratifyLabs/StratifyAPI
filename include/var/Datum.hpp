/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_VAR_DATUM_HPP_
#define SAPI_VAR_DATUM_HPP_


#include "String.hpp"
#include "Json.hpp"
#include "../fs/File.hpp"
#include "../chrono/ClockTime.hpp"
#include "../chrono/Time.hpp"

namespace var {
class Datum {
public:
	Datum();
	Datum(const var::String & type, const var::String & key, const var::String & value);
	explicit Datum(const JsonObject & json_object);

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
	API_ACCESS_COMPOUND(Datum, String, type);
	API_ACCESS_COMPOUND(Datum, String, key);
	API_ACCESS_COMPOUND(Datum, String, value);

	JsonObject m_metadata;
	chrono::ClockTime m_clocktime;
	chrono::Time m_time;

	String m_id;
};




}

namespace sys {
class Printer;
Printer& operator << (Printer& printer, const var::Datum & a);
}



#endif // SAPI_VAR_DATUM_HPP_
