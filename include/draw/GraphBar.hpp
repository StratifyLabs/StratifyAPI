/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_GRAPHBAR_HPP_
#define DRAW_GRAPHBAR_HPP_

#include "Graph.hpp"

namespace draw {

/*! \brief Bar Graph Element
 * \ingroup graph
 * \details This class can be used to draw a scalable bar graph.
 */
class GraphBar : public Graph {
public:
	GraphBar();

private:
	virtual void draw_to_scale(const DrawingScaledAttr & attr);

};

};

#endif /* DRAW_GRAPHBAR_HPP_ */
