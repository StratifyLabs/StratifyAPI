#ifndef CSV_HPP
#define CSV_HPP

#include "../fs/File.hpp"
#include "../var/Matrix.hpp"
#include "var/String.hpp"
#include "../var/Tokenizer.hpp"

namespace fmt {

class Csv : public api::WorkObject{
public:

	using Delimeters = var::Tokenizer::Delimeters;

	Csv(fs::File & file, const var::String& delimeters);

	static var::Matrix<var::String> load(
			const var::String & file_path,
			Delimeters delimeters = Delimeters(",")
			);

	static int save(
			const var::String & file_path,
			const var::Matrix<var::String> & m_matrix,
			Delimeters delimeters = Delimeters(",")
			);

	var::StringList read_line(bool is_header = false);

	const var::StringList & header() const {
		return m_header;
	}

private:

	const fs::File& file() const { return m_file; }
	fs::File& file(){ return m_file; }

	fs::File & m_file;
	var::StringList	m_header;
	var::String m_delimeters = ",";

};

}

#endif // CSV_HPP
