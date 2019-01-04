#include "fmt/Png.hpp"

using namespace fmt;

Png::Png(const var::ConstString & name){
	open_readonly(name);
}


