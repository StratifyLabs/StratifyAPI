/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
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
	drawing_area_t d = attr.area();
	Tab::calc_square(p, d);
	da = attr;
	da.set_point(p);
	da.set_area(d);
	progress().draw(da);
}
