/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#include <cstdarg>
#if defined __win32
#include <windows.h>
#endif

#include <unistd.h>

#include "var/String.hpp"

#include "printer/Printer.hpp"

#if defined __win32
unsigned int sys::Printer::m_default_color = static_cast<unsigned int>(-1);
#endif

using namespace printer;

Printer::Printer() {
  m_progress_callback.set_callback(Printer::update_progress_callback)
    .set_context(this);
  m_print_flags = PrintFlags::width_8 | PrintFlags::hex;
  m_indent = 0;
  m_progress_width = 50;
  m_progress_state = 0;
  m_verbose_level = Level::info;
  m_progress_key = "progress";
#if defined __win32
  if (m_default_color == static_cast<unsigned int>(-1)) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    m_default_color = info.wAttributes;
  }
#endif

#if defined __link
  m_is_bash = false;
#endif
}

void Printer::set_format_code(u32 code) {
#if defined __link
  if (api::ApiInfo::is_macosx() || is_bash()) {
    print_final("\033[1;%dm", code);
  }
#endif
}

void Printer::clear_format_code(u32 code) {
#if defined __link
  if (api::ApiInfo::is_macosx() || is_bash()) {
    print_final(var::String::number(code, "\033[1;2%dm"));
  }
#endif
}

void Printer::set_color_code(u32 code) {

#if defined __link
  if (api::ApiInfo::is_macosx() || is_bash()) {
    print_final(var::String::number(code, "\033[1;%dm"));
  }
#endif

#if defined __link && defined __win32
  WORD color = static_cast<WORD>(m_default_color);
  switch (code) {
  case ColorCode::default:
    color = static_cast<WORD>(m_default_color);
    break;
  case ColorCode::black:
    color = 0;
    break;
  case ColorCode::red:
    color = FOREGROUND_RED;
    break;
  case ColorCode::green:
    color = FOREGROUND_GREEN;
    break;
  case ColorCode::yellow:
    color = FOREGROUND_RED | FOREGROUND_GREEN;
    break;
  case ColorCode::blue:
    color = FOREGROUND_BLUE;
    break;
  case ColorCode::magenta:
    color = FOREGROUND_BLUE | FOREGROUND_RED;
    break;
  case ColorCode::cyan:
    color = FOREGROUND_BLUE | FOREGROUND_GREEN;
    break;
  case ColorCode::light_gray:
    color = FOREGROUND_INTENSITY;
    break;
  case ColorCode::dark_gray:
    color = 0;
    break;
  case ColorCode::light_red:
    color = FOREGROUND_RED | FOREGROUND_INTENSITY;
    break;
  case ColorCode::light_green:
    color = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    break;
  case ColorCode::light_yellow:
    color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    break;
  case ColorCode::light_blue:
    color = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    break;
  }
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
#endif
}

void Printer::print(
  Level verbose_level,
  var::StringView key,
  var::StringView value,
  Newline is_newline) {
  // default flat printer behavior
  if (verbose_level > this->verbose_level()) {
    return;
  }

  for (u32 indent = 0; indent < m_indent; indent++) {
    print_final("  ");
  }

  if (key != nullptr) {
    if (m_print_flags & PrintFlags::bold_keys) {
      set_format_code(FormatType::bold);
    }
    if (m_print_flags & PrintFlags::cyan_keys) {
      set_color_code(static_cast<int>(ColorCode::cyan));
    }
    if (m_print_flags & PrintFlags::yellow_keys) {
      set_color_code(static_cast<int>(ColorCode::yellow));
    }
    if (m_print_flags & PrintFlags::magenta_keys) {
      set_color_code(static_cast<int>(ColorCode::magenta));
    }
    if (m_print_flags & PrintFlags::red_keys) {
      set_color_code(static_cast<int>(ColorCode::red));
    }
    if (m_print_flags & PrintFlags::key_quotes) {
      print_final(var::String("\"") + key + "\"");
    } else {
      print_final(var::String(key) + ": ");
    }
    if (m_print_flags & PrintFlags::bold_keys) {
      clear_format_code(static_cast<int>(FormatType::bold));
    }
    if (
      m_print_flags
      & (PrintFlags::cyan_keys | PrintFlags::yellow_keys | PrintFlags::magenta_keys | PrintFlags::red_keys)) {
      clear_color_code();
    }
  }

  if (value != nullptr) {
    if (m_print_flags & PrintFlags::bold_values) {
      set_format_code(static_cast<int>(FormatType::bold));
    }
    if (m_print_flags & PrintFlags::green_values) {
      set_color_code(static_cast<int>(ColorCode::green));
    }
    if (m_print_flags & PrintFlags::yellow_values) {
      set_color_code(static_cast<int>(ColorCode::yellow));
    }
    if (m_print_flags & PrintFlags::red_values) {
      set_color_code(static_cast<int>(ColorCode::red));
    }
    if (m_print_flags & PrintFlags::cyan_values) {
      set_color_code(static_cast<int>(ColorCode::cyan));
    }
    if (m_print_flags & PrintFlags::value_quotes) {
      print_final(var::String("\"") + value + "\"");
    } else {
      print_final(value);
    }
    if (m_print_flags & PrintFlags::bold_values) {
      clear_format_code(static_cast<int>(FormatType::bold));
    }
    if (
      m_print_flags
      & (PrintFlags::green_values | PrintFlags::yellow_values | PrintFlags::red_values | PrintFlags::cyan_values)) {
      clear_color_code();
    }
  }

  if (is_newline == Newline::yes) {
    print_final("\n");
  }
}

void Printer::print_final(var::StringView view) {
#if defined __link
  fwrite(view.to_const_void(), view.size(), 1, stdout);
#else
  ::write(stdout->_file, view.cstring(), view.length());
#endif
}

Printer &Printer::open_object(var::StringView key, Level level) {
  print_open_object(level, key.cstring());
  return *this;
}

Printer &Printer::close_object() {
  print_close_object();
  return *this;
}

Printer &Printer::open_array(var::StringView key, Level level) {
  print_open_array(level, key.cstring());
  return *this;
}

Printer &Printer::close_array() {
  print_close_array();
  return *this;
}

void Printer::print_open_object(
  Level verbose_level,
  var::StringView key) {
  print(verbose_level, key.cstring(), "");
  m_indent++;
}

void Printer::print_close_object() {
  if (m_indent) {
    m_indent--;
  }
}

void Printer::print_open_array(
  Level verbose_level,
  var::StringView key) {
  print(verbose_level, key.cstring(), "");
  m_indent++;
}

void Printer::print_close_array() {
  if (m_indent) {
    m_indent--;
  }
}

void Printer::clear_color_code() {
#if defined __link
  if (api::ApiInfo::is_macosx() || is_bash()) {
    print_final("\033[0m");
  } else {
    set_color_code(ColorCode::default);
  }
#endif
}

Printer::ColorCode Printer::color_code(var::StringView color) {
  var::String color_upper(color);
  color_upper.to_upper();
  if (color_upper == "BLACK") {
    return ColorCode::black;
  }
  if (color_upper == "RED") {
    return ColorCode::red;
  }
  if (color_upper == "GREEN") {
    return ColorCode::green;
  }
  if (color_upper == "YELLOW") {
    return ColorCode::yellow;
  }
  if (color_upper == "BLUE") {
    return ColorCode::blue;
  }
  if (color_upper == "MAGENTA") {
    return ColorCode::magenta;
  }
  if (color_upper == "CYAN") {
    return ColorCode::cyan;
  }
  if (color_upper == "LIGHT GRAY") {
    return ColorCode::light_gray;
  }
  if (color_upper == "DARK GRAY") {
    return ColorCode::dark_gray;
  }
  if (color_upper == "LIGHT RED") {
    return ColorCode::light_red;
  }
  if (color_upper == "LIGHT GREEN") {
    return ColorCode::light_green;
  }
  if (color_upper == "LIGHT YELLOW") {
    return ColorCode::light_yellow;
  }
  if (color_upper == "LIGHT BLUE") {
    return ColorCode::light_blue;
  }

  return ColorCode::normal;
}

Printer::~Printer() {}

#if 0
void Printer::vprint(const char * fmt, va_list list){
	vprintf(fmt, list);
	fflush(stdout);
}

void Printer::print(const char * fmt, ...){
	va_list list;
	va_start(list, fmt);
	vprint(fmt, list);
	va_end(list);
}
#endif

Printer &Printer::operator<<(const api::Status &a) {
  key("lineNumber", var::String::number(a.line_number(), "%d"));
  key("error", var::StringView(a.error_code_description()));
  return *this;
}

Printer &Printer::operator<<(var::StringView a) {
  return key(nullptr, a);
}

Printer &Printer::set_verbose_level(var::StringView level) {
  if (level == "debug") {
    set_verbose_level(Level::debug);
  } else if (level == "info") {
    set_verbose_level(Level::info);
  } else if (level == "message") {
    set_verbose_level(Level::message);
  } else if (level == "warning") {
    set_verbose_level(Level::warning);
  } else if (level == "error") {
    set_verbose_level(Level::error);
  } else if (level == "fatal") {
    set_verbose_level(Level::fatal);
  } else if (level == "trace") {
    set_verbose_level(Level::trace);
  }
  return *this;
}

char Printer::get_bitmap_pixel_character(u32 color, u8 bits_per_pixel) {
  if (color == 0) {
    return ' ';
  } else {
    if (bits_per_pixel == 2) {
      switch (color) {
      case 1:
        return '.';
      case 2:
        return '+';
      case 3:
        return '@';
      }
    } else if (bits_per_pixel < 8) {
      switch (color) {
      case 1:
        return '.';
      case 2:
        return ',';
      case 3:
        return '-';
      case 4:
        return '+';
      case 5:
        return '=';
      case 6:
        return '^';
      case 7:
        return 'x';
      case 8:
        return 'o';
      case 9:
        return '*';
      case 10:
        return '#';
      case 11:
        return '%';
      case 12:
        return '$';
      case 13:
        return 'O';
      case 14:
        return 'X';
      case 15:
        return '@';
      }
    } else {
      return '#';
    }
  }
  return '?';
}

u32 Printer::get_bitmap_pixel_color(char c, u8 bits_per_pixel) {

  if (bits_per_pixel == 1) {
    if (c == ' ') {
      return 0;
    }
    return 1;
  }

  if (bits_per_pixel == 2) {
    switch (c) {
    case ' ':
      return 0;
    case '.':
      return 1;
    case ',':
      return 2;
    case '-':
    default:
      return 3;
    }
  }

  if (bits_per_pixel == 4) {
    switch (c) {
    case ' ':
      return 0;
    case '.':
      return 1;
    case ',':
      return 2;
    case '-':
      return 3;
    case '+':
      return 4;
    case '=':
      return 5;
    case '^':
      return 6;
    case 'x':
      return 7;
    case 'o':
      return 8;
    case '*':
      return 9;
    case '#':
      return 10;
    case '%':
      return 11;
    case '$':
      return 12;
    case 'O':
      return 13;
    case 'X':
      return 14;
    case '@':
    default:
      return 15;
    }
  }

  return 255;
}

bool Printer::update_progress(int progress, int total) {
  const u32 width = m_progress_width;

  if (verbose_level() >= Level::info) {

    if ((m_progress_state == 0) && total) {

      // only print the key once with total == -1
      print(Level::info, m_progress_key.cstring(), nullptr);
      if (total != -1) {
        if (m_print_flags & PrintFlags::value_quotes) {
          print_final("\"");
        }
        if ((m_print_flags & PrintFlags::simple_progress) == 0) {
          for (u32 i = 0; i < width; i++) {
            print_final(".");
          }
          for (u32 i = 0; i < width; i++) {
            print_final("\b"); // backspace
          }
        }
      }
      m_progress_state++;
      fflush(stdout);
    }

    if (m_progress_state > 0) {

      if (total == api::ProgressCallback::indeterminate_progress_total()) {
        var::String output;
        var::StringView  animation = "-\\|/";
        if ((m_print_flags & PrintFlags::value_quotes) && (m_progress_state == 1)) {
          print_final("\"");
        }
        m_progress_state++;

        if ((m_print_flags & PrintFlags::simple_progress) == 0) {
          output.format(
            "%c" F32U,
            animation.at(m_progress_state % animation.length()),
            progress);
          print_final(output.cstring());
          for (u32 i = 0; i < output.length(); i++) {
            print_final("\b"); // backspace
          }
        } else {
          print_final(var::String::number(m_progress_state - 1));
        }

      } else {

        while (
          (total != 0)
          && (m_progress_state <= (progress * width + total / 2) / total)) {
          print_final("#");
          m_progress_state++;
          fflush(stdout);
        }

        if ((progress >= total) || (total == 0)) {
          m_progress_state = 0;
        }
      }
    }
    if (total == 0) {
      if ((m_print_flags & PrintFlags::no_progress_newline) == 0) {
        print_final("\n");
      }
      if (m_print_flags & PrintFlags::value_quotes) {
        print_final("\"");
      }
    }
  }

  return false;
}

Printer &Printer::key(var::StringView key, bool value) {
  print(Level::info, key.cstring(), value ? "true" : "false");
  return *this;
}

Printer &Printer::key(var::StringView key, var::StringView a) {
  print(Level::info, key, a);
  return *this;
}

Printer &Printer::debug(var::StringView a) {
  print(Level::debug, "debug", a);
  return *this;
}

Printer &Printer::info(var::StringView a) {
  print(Level::info, "info", a);
  return *this;
}

Printer &Printer::message(var::StringView a) {
  print(Level::message, "message", a);
  return *this;
}

Printer &Printer::warning(var::StringView a) {
  if (flags() & PrintFlags::yellow_warnings) {
    set_color_code(ColorCode::yellow);
  }
  print(Level::warning, "warning", a);
  if (flags() & PrintFlags::yellow_warnings) {
    clear_color_code();
  }
  return *this;
}

Printer &Printer::error(var::StringView a) {
  if (flags() & PrintFlags::red_errors) {
    set_color_code(ColorCode::red);
  }
  print(Level::error, "error", a);
  if (flags() & PrintFlags::red_errors) {
    clear_color_code();
  }
  return *this;
}

Printer &Printer::fatal(var::StringView a) {
  if (flags() & PrintFlags::red_errors) {
    set_color_code(ColorCode::red);
  }
  print(Level::fatal, "fatal", a);
  if (flags() & PrintFlags::red_errors) {
    clear_color_code();
  }
  return *this;
}

Printer &
Printer::trace(const char *function, int line, var::StringView message) {

  if (verbose_level() == Level::trace) {
    print_final(
      var::String()
        .format(">> trace %s:%d %s\n", function, line, message.cstring()));
  }
  return *this;
}