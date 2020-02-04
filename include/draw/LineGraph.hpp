/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_DRAW_LINEGRAPH_HPP_
#define SAPI_DRAW_LINEGRAPH_HPP_

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

#endif /* SAPI_DRAW_LINEGRAPH_HPP_ */
