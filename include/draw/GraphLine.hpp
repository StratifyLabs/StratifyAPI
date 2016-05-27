/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_GRAPHLINE_HPP_
#define DRAW_GRAPHLINE_HPP_

#include <draw/GraphObject.hpp>

namespace draw {

/*! \brief Line Graph Element
 * \ingroup graph
 */
class GraphLine : public GraphObject {
public:
	GraphLine();

private:
	virtual void draw_to_scale(const DrawingScaledAttr & attr);
};

};

#endif /* DRAW_GRAPHLINE_HPP_ */
