
#include <cstdio>
#include <mcu/types.h>

#include "Test.hpp"
#include "crypto.hpp"
#include "fs.hpp"
#include "hal/Spi.hpp"
#include "var/View.hpp"

using namespace hal;

int main(int argc, char *argv[]) {

  Device spi("/dev/spi0");

  spi.ioctl(Spi::set_attributes(
    Spi::Attributes().set_flags(Spi::Flags::set_master).set_width(8)));

  int swapped = Spi::swap(spi, 32);

  return 0;
}
