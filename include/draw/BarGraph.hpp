/*! \file */ //Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#ifndef SAPI_DRAW_BARGRAPH_HPP_
#define SAPI_DRAW_BARGRAPH_HPP_

#include "Graph.hpp"

namespace draw {

/*! \brief Bar Graph Element
 * \ingroup graph
 * \details This class can be used to draw a scalable bar graph.
 */
class BarGraph : public Graph {
public:
	BarGraph();

private:
	virtual void draw_to_scale(const DrawingScaledAttr & attr);

};

}

#endif /* SAPI_DRAW_BARGRAPH_HPP_ */
