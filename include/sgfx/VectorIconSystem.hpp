/*
 * GfxSystem.hpp
 *
 *  Created on: May 29, 2016
 *      Author: tgil
 */

#ifndef SGFX_VECTORICONSYSTEM_HPP_
#define SGFX_VECTORICONSYSTEM_HPP_

#include <sapi/sg_types.h>

namespace sgfx {


/*! \brief Icon System Class
 * \details This class manages icon data (sg_vector_icon_t) that is installed
 * in the system.
 */
class VectorIconSystem {
public:

	/*! \details This initializes the system graphics table.
	 *
	 * @param icons A pointer to a table that holds the system icons
	 * @param count The total number of system icons available
	 * @return True if all fonts loaded successfully
	 */
	static bool load_icons(const sg_vector_icon_t * icons, u32 count);


	/*! \details Load the system icon specified by the location in the icon table.
	 *
	 * @param icon The icon to load
	 * @param bold Whether or not to use a bold font
	 * @return
	 */
	static const sg_vector_icon_t * get_icon(u32 icon);

private:
	static const sg_vector_icon_t * m_icons;
	static u16 m_count;

};

};

#endif /* SGFX_VECTORICONSYSTEM_HPP_ */
