#ifndef SAPI_UX_SEPARATOR_HPP
#define SAPI_UX_SEPARATOR_HPP

#include "Component.hpp"

namespace	ux {



class Separator : public ComponentAccess<Separator>{
public:
	COMPONENT_PREFIX(Separator)
	Separator(const var::String & name)
		: ComponentAccess(
				prefix() + name
				){}
	void draw(const DrawingScaledAttributes & attributes);
};

}

#endif // SEPARATOR_HPP
