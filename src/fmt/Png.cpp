#include "fmt/Png.hpp"

using namespace fmt;

Png::Png(const arg::SourceFilePath & name){
	open(arg::FilePath(name.argument()), fs::OpenFlags::read_only());
}


