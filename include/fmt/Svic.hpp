#ifndef SAPI_FMT_SVIC_HPP_
#define SAPI_FMT_SVIC_HPP_

#include "../fs/File.hpp"
#include "../api/FmtObject.hpp"
#include "../sgfx/Vector.hpp"
#include "../var/Vector.hpp"

namespace fmt {

/*! \brief Svc (Stratify Vector Collection) Class
 * \details The Svc class manages files that
 * contain collections of Stratify Vector icons.
 *
 */
class Svic : public api::FmtFileObject {
public:
	Svic(const arg::SourceFilePath & path = arg::SourceFilePath(""));

	u32 count() const { return m_icons.count(); }

	var::String name_at(u32 i) const;

	int append(
			const var::ConstString & name,
			const var::Vector<sg_vector_path_description_t> & list
			);

	sgfx::VectorPath get(const var::ConstString & name) const;
	sgfx::VectorPath at(u32 i) const;

private:
	int parse_icons();
	var::Vector<sg_vector_icon_header_t> m_icons;

	//these track internal state used for caching
	mutable u32 m_current_icon_at;
	mutable var::Vector<sg_vector_path_description_t> m_current_icon;

};

}

#endif // SAPI_FMT_SVIC_HPP_
