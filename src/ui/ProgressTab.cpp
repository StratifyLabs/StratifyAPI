//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "draw.hpp"
#include "ui/Tab.hpp"
#include "ui/ProgressTab.hpp"
using namespace ui;

ProgressTab::ProgressTab() {
	// TODO Auto-generated constructor stub

}

void ProgressTab::draw(const DrawingAttr & attr){
	DrawingAttr da;
	drawing_point_t p = attr.point();
	drawing_dim_t d = attr.dim();
	Tab::calc_square(p, d);
	da = attr;
	da.set_point(p);
	da.set_dim(d);
	progress().draw(da);
}
