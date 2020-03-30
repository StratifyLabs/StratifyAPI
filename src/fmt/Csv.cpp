#include "fmt/Csv.hpp"

using namespace fmt;

Csv::Csv(fs::File& file, const var::String & delimeters) :
	m_file(file),
	m_delimeters(delimeters){
	m_header = read_line(true);
}

var::StringList Csv::read_line(bool is_header){
	var::StringList result;

	var::String line = m_file.gets();
	for(int i=0; i < line.length(); i++){
		if( (u8)line.at(i) > 127 ){
			line.erase(
						var::String::Position(i),
						var::String::Length(1)
						);
			i--;
		}
	}

	line
			.replace(
				var::String::ToErase("\n"),
				var::String::ToInsert("")
				)
			.replace(
				var::String::ToErase("\r"),
				var::String::ToInsert("")
				);

	if( line.length() == 0 ){
		return result;
	}
	var::Tokenizer line_tokens(
				line,
				var::Tokenizer::Delimeters(m_delimeters),
				var::Tokenizer::IgnoreBetween("\""),
				var::Tokenizer::IsCountEmpty(true)
				);

	result = line_tokens.to_list();

	if( !is_header ){
		if( (result.count() > 0) && (result.count() != header().count()) ){
			result.resize( header().count() );
		}
	}
	return result;
}

var::Matrix<var::String> Csv::load(
		const var::String & file_path,
		Delimeters delimeters
		){
	var::Matrix<var::String> result;

	fs::DataFile data_file = fs::DataFile(
				fs::File::Path(file_path)
				);

	Csv csv(data_file, delimeters.argument());

	var::StringList row;
	result.append( csv.header() );
	do {
		row = csv.read_line();
		if( row.count() > 0 ){
			result.append( row );
		}
	} while( row.count() > 0 );

	return result;
}

int Csv::save(
		const var::String & file_path,
		const var::Matrix<var::String> & m_matrix,
		Delimeters delimeters
		){

	return 0;
}
