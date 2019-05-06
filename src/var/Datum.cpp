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


void Datum::refresh_timestamp(){
	m_clocktime = chrono::Clock::get_time();
	m_time = chrono::Time::get_time_of_day();
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
