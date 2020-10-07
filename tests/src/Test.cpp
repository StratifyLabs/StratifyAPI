#include "Test.hpp"

Test &Test::clear_view(View view) {
  view.fill<u8>(0);
  return *this;
}
