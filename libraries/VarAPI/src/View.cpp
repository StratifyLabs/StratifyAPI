/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
// Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <errno.h>

#include "var/Data.hpp"
#include "var/String.hpp"

#if !defined __link
#include <mcu/arch.h>
#include <reent.h>
#endif

#include "var/View.hpp"

using namespace var;

// This is here so that by default that data can point to a null value rather
// than be a null value
const int View::m_zero_value MCU_ALIGN(4) = 0;

// this value corresponds to the malloc chunk size used in Stratify OS
// this may be something that could be determined through a system call

void View::set_view(const Construct &options) {
  if (options.size()) {
    m_size_read_only = options.size();
    if (options.write_buffer()) {
      m_data = options.write_buffer();
    } else {
      m_data = (void *)options.read_buffer();
      m_size_read_only |= m_size_read_only_flag;
    }
  } else {
    m_data = nullptr;
    m_size_read_only = 0;
  }
}

View::View(const Data &data) {
  set_view(Construct().set_read_buffer(data.data()).set_size(data.size()));
}

View::View(Data &data) {
  set_view(Construct().set_write_buffer(data.data()).set_size(data.size()));
}

View &View::swap_byte_order(SwapBy swap) {
  API_RETURN_VALUE_IF_ERROR(*this);
  if (write_data()) {
    if (swap == SwapBy::word) {
      u32 *p = to_u32();
      if (p == nullptr) {
        API_RETURN_VALUE_ASSIGN_ERROR(*this, "read only data", EINVAL);
      }
      u32 i;
      for (i = 0; i < this->size() / 4; i++) {
#if !defined __link
        p[i] = __REV(p[i]);
#else
        // swap manually
        API_RETURN_VALUE_ASSIGN_ERROR(*this, "swap not supported", ENOTSUP);
#endif
      }
    } else {
      u16 *p = to_u16();
      if (p == nullptr) {
        API_RETURN_VALUE_ASSIGN_ERROR(*this, "read only data", EINVAL);
      }
      u16 i;
      for (i = 0; i < this->size() / 2; i++) {
#if !defined __link
        p[i] = __REV16(p[i]);
#else
        // swap manually
#endif
      }
    }
  }
  return *this;
}

var::String View::to_string() const {
  var::String result = var::String().reserve(size() * 2);
  for (u32 i = 0; i < size(); i++) {
    result += Ntos(to_const_u8()[i], "%02X");
  }
  return result;
}

#if USE_PRINTER
sys::Printer &sys::operator<<(sys::Printer &printer, const var::View &a) {
  u32 o_flags = printer.o_flags();
  const s8 *ptrs8 = a.to_const_s8();
  const s16 *ptrs16 = a.to_const_s16();
  const s32 *ptrs32 = a.to_const_s32();
  const u8 *ptru8 = a.to_const_u8();
  const u16 *ptru16 = a.to_const_u16();
  const u32 *ptru32 = a.to_const_u32();

  int s;
  if (o_flags & sys::Printer::print_32) {
    s = a.size() / 4;
  } else if (o_flags & sys::Printer::print_16) {
    s = a.size() / 2;
  } else if (o_flags & sys::Printer::print_blob) {
    s = (a.size() + 15) / 16;
  } else {
    s = a.size();
  }

  int i;
  u32 bytes_printed = 0;
  var::String data_string;

  for (i = 0; i < s; i++) {
    if (o_flags & sys::Printer::print_hex) {
      if (o_flags & sys::Printer::print_32) {
        data_string << var::String().format(F32X, ptru32[i]);
      } else if (o_flags & sys::Printer::print_16) {
        data_string << var::String().format("%X", ptru16[i]);
      } else if (o_flags & sys::Printer::print_blob) {
        for (u32 j = 0; j < 16; j++) {
          data_string << var::String().format("%02X ", ptru8[i * 16 + j]);
          if (j < 15) {
            data_string << " ";
          }
          bytes_printed++;
          if (bytes_printed == a.size()) {
            break;
          }
        }
      } else {
        data_string << var::String().format("%X", ptru8[i]);
      }
      data_string << " ";
    }
    if (o_flags & sys::Printer::print_unsigned) {
      if (o_flags & sys::Printer::print_32) {
        data_string << var::String().format(F32U, ptru32[i]);
      } else if (o_flags & sys::Printer::print_16) {
        data_string << var::String().format("%u", ptru16[i]);
      } else if (o_flags & sys::Printer::print_blob) {
        for (u32 j = 0; j < 16; j++) {
          data_string << var::String().format("%u", ptru8[i * 16 + j]);
          if (j < 15) {
            data_string << " ";
          }
        }
      } else {
        data_string << var::String().format("%u", ptru8[i]);
      }
      data_string << " ";
    }
    if (o_flags & sys::Printer::print_signed) {
      if (o_flags & sys::Printer::print_32) {
        data_string << var::String().format(F32D, ptrs32[i]);
      } else if (o_flags & sys::Printer::print_16) {
        data_string << var::String().format("%d", ptrs16[i]);
      } else if (o_flags & sys::Printer::print_blob) {
        for (u32 j = 0; j < 16; j++) {
          data_string << var::String().format("%d", ptru8[i * 16 + j]);
          if (j < 15) {
            data_string << " ";
          }
        }
      } else {
        data_string << var::String().format("%d", ptrs8[i]);
      }
      data_string << " ";
    }
    if (o_flags & sys::Printer::print_char) {
      if (ptru8[i] == '\n') {
        data_string << (" \\n");
      } else if (ptru8[i] == '\r') {
        data_string << (" \\r");
      } else if (ptru8[i] == 0) {
        data_string << (" null");
      } else if (ptru8[i] < 128) {
        data_string << var::String().format(" %c", ptru8[i]);
      }
    }

    printer.print(
      printer.verbose_level(),
      var::String().format("[%04d]", i).cstring(),
      data_string.cstring());
    data_string.clear();
  }

  return printer;
}
#endif
