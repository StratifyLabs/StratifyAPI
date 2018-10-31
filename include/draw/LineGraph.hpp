/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_LINEGRAPH_HPP_
#define DRAW_LINEGRAPH_HPP_

#include "Graph.hpp"

namespace draw {

/*! \brief Line Graph Drawing
 * \ingroup graph
 */
class LineGraph : public Graph {
public:
	LineGraph();

private:
	virtual void draw_to_scale(const DrawingScaledAttr & attr);
};

}

#endif /* DRAW_LINEGRAPH_HPP_ */
