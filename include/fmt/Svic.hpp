#ifndef SAPI_FMT_SVIC_HPP
#define SAPI_FMT_SVIC_HPP

#include "../sys/File.hpp"
#include "../api/FmtObject.hpp"
#include "../sgfx/Vector.hpp"
#include "../var/Vector.hpp"

namespace fmt {



/*! \brief Svc (Stratify Vector Collection) Class
 * \details The Svc class manages files that
 * contain collections of Stratify Vector icons.
 *
 */
class Svic : public api::FmtWorkObject {
public:
	Svic(sys::File & file);

	u32 count() const { return m_icons.count(); }

	var::String name_at(u32 i);

	int append(const var::ConstString & name, const var::Vector<sg_vector_path_description_t> & list);

	sgfx::VectorPath get(const var::ConstString & name);
	sgfx::VectorPath at(u32 i);

	sys::File & file(){ return m_file; }
	const sys::File & file() const { return m_file; }

private:

	int parse_icons();

	sys::File & m_file;

	var::Vector<sg_vector_path_icon_header_t> m_icons;
	var::Vector<sg_vector_path_description_t> m_current_icon;

};

}

#endif // SAPI_FMT_SVIC_HPP
