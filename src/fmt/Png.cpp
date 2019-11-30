#include "fmt/Png.hpp"

using namespace fmt;

Png::Png(const var::String & name){
   open(name, fs::OpenFlags::read_only());
}


