#ifndef SAPI_UX_COMPOUNDCOMPONENT_HPP
#define SAPI_UX_COMPOUNDCOMPONENT_HPP

#include "Component.hpp"
#include "Scene.hpp"

namespace ux {

class CompoundComponent : public Component
{
public:
	virtual void handle_event(const ux::Event & event);

protected:
	CompoundComponent& add_sub_component(
			const var::String & name,
			Component& component){
		component.set_name(name);
		m_sub_component_list.push_back(&component);
		return *this;
	}

private:
	var::Vector<Component*> m_sub_component_list;
	bool m_is_initialized = false;
	void initialize();

};

}

#endif // SAPI_UX_COMPONENTGROUP_HPP
