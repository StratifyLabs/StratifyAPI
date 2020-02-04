/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "fmt/Png.hpp"

using namespace fmt;

Png::Png(const var::String & name){
   open(name, fs::OpenFlags::read_only());
}


