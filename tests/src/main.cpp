
#include <cstdio>
#include <mcu/types.h>

#include "Test.hpp"
#include "var/View.hpp"

int main(int argc, char *argv[]) {

  Test t;

  u32 value = 0x12345678;

  printf("Value is %d\n", value);
  t.clear_view(View(value));
  printf("value is %d\n", value);

  return 0;
}
