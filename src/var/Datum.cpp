#include "var/Datum.hpp"
#include "chrono/Clock.hpp"
#include "chrono/Time.hpp"

using namespace var;

Datum::Datum(){
	//grab a valid timestamp
	refresh_timestamp();
}

Datum::Datum(const var::ConstString & type, const var::ConstString & key, const var::ConstString & value){
	refresh_timestamp();
	this->type() = type;
	this->key() = key;
	this->value() = value;
}

Datum::Datum(const JsonObject & json_object){
	type() = json_object.at("type").to_string();
	key() = json_object.at("key").to_string();
	value() = json_object.at("value").to_string();

	u32 seconds;
	u32 nanoseconds;
	String clock_time_value = json_object.at("clockTime").to_string();
	sscanf(clock_time_value.cstring(), F32U "." F32U, &seconds, &nanoseconds);

	m_clocktime.set(
				chrono::Seconds(seconds),
				chrono::Nanoseconds(nanoseconds)
				);

	this->m_time = json_object.at("deviceTime").to_integer();
	this->metadata() = json_object.at("metadata").to_object();
}

int Datum::save(const fs::File & f) const {

	String value = stringify();
	value << "\n";
	return f.write(value);
}


void Datum::refresh_timestamp(){
	m_clocktime = chrono::Clock::get_time();
	m_time = chrono::Time::get_time_of_day();
}

String Datum::stringify() const {
	JsonDocument document;
	document.set_flags(JsonDocument::COMPACT);
	//compact object on one line
	return document.stringify(to_json()) << "\n";
}

var::JsonObject Datum::to_json() const {
	JsonObject result;
	result.insert("type", JsonString( type() ));
	result.insert("key", JsonString( key() ));
	result.insert("value", JsonString( value() ));
	result.insert("clockTime", JsonString( String().format("%ld.%09ld", clock_time().seconds(), clock_time().nanoseconds()) ));
	result.insert("deviceTime", JsonInteger( time().time() ));
	result.insert("metadata", metadata());
	return result;
}
