
#include <cstdio>
#include <mcu/types.h>

#include "Test.hpp"
#include "crypto.hpp"
#include "fs.hpp"
#include "var/View.hpp"

int main(int argc, char *argv[]) {

  Test t;

  u32 value = 0x12345678;

  printf("Value is %d\n", value);
  t.clear_view(View(value));
  printf("value is %d\n", value);

  File encrypted_file("/home/file.crypto", OpenMode::read_only());
  DataFile df = DataFile(OpenMode::append_read_write());

  Aes()
    .set_key(var::View("abcd"))
    .decrypt_cbc(
      Aes::CryptOptions().set_cipher(&encrypted_file).set_plain(&df));

  return 0;
}
