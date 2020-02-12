/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#include "sgfx.hpp"
#include "sys/Assets.hpp"
#include "ux/Icon.hpp"

#include "sys/Printer.hpp"

using namespace ux;
using namespace sgfx;

Icon::Icon(){
	m_rotation = rotation_right;
}

void Icon::draw_to_scale(const DrawingScaledAttributes & attr){

}
