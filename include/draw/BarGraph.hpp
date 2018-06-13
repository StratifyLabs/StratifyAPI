/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_BARGRAPH_HPP_
#define DRAW_BARGRAPH_HPP_

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

#endif /* DRAW_GRAPHBAR_HPP_ */
